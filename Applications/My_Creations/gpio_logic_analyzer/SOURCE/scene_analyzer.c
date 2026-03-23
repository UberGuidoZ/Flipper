// #########################################################
// #  scene_analyzer.c - capture, render, and CSV logging  #
// #  By: UberGuidoZ | https://github.com/UberGuidoZ/      #
// #  v2.01                                                #
// #########################################################
//
//  GPIO capture runs on a FuriTimer (FreeRTOS timer service task).
//  The draw callback runs on the GUI thread. A FuriMutex guards
//  all shared channel and CSV state.
//
//  CONTROLS (while analyzer is on screen):
//    OK (short) - toggle capture on / off
//    OK (long)  - clear waveform history
//    UP         - increase sample rate
//    DOWN       - decrease sample rate
//    BACK       - stop capture, close CSV, return to menu
//
// ##############################################################

#include "gpio_analyzer.h"
#include <string.h>
#include <stdio.h>

// Enforce 1:1 pixel-to-sample mapping required by the renderer.
_Static_assert(
    GA_HISTORY_SIZE == GA_WAVE_W,
    "GA_HISTORY_SIZE must equal GA_WAVE_W for 1:1 pixel mapping");

// ============================================================
// GPIO pin table  (rows displayed top-to-bottom)
// ============================================================
static const GpioPin* const k_pins[GA_PIN_COUNT] = {
    &gpio_ext_pa7,
    &gpio_ext_pa6,
    &gpio_ext_pa4,
    &gpio_ext_pb3,
    &gpio_ext_pb2,
    &gpio_ext_pc3,
    &gpio_ext_pc1,
    &gpio_ext_pc0,
};

static const char* const k_pin_labels[GA_PIN_COUNT] = {
    "A7", "A6", "A4", "B3", "B2", "C3", "C1", "C0",
};

// CSV column header names (must match k_pin_labels order)
static const char* const k_csv_headers[GA_PIN_COUNT] = {
    "A7", "A6", "A4", "B3", "B2", "C3", "C1", "C0",
};

// Pre-computed top-pixel y for each channel row.
static const uint8_t k_row_top[GA_PIN_COUNT] = {
    GA_HEADER_H + 0U * GA_ROW_H,  //  9
    GA_HEADER_H + 1U * GA_ROW_H,  // 16
    GA_HEADER_H + 2U * GA_ROW_H,  // 23
    GA_HEADER_H + 3U * GA_ROW_H,  // 30
    GA_HEADER_H + 4U * GA_ROW_H,  // 37
    GA_HEADER_H + 5U * GA_ROW_H,  // 44
    GA_HEADER_H + 6U * GA_ROW_H,  // 51
    GA_HEADER_H + 7U * GA_ROW_H,  // 58
};

// ============================================================
// Sample rate table
// ============================================================
static const uint32_t k_sample_rates[GA_RATE_COUNT] = {
    10U,   // 10  Hz -- 100 ms
    50U,   // 50  Hz -- 20  ms
    100U,  // 100 Hz -- 10  ms
    200U,  // 200 Hz -- 5   ms
    500U,  // 500 Hz -- 2   ms
};

static const char* const k_rate_labels[GA_RATE_COUNT] = {
    " 10Hz",
    " 50Hz",
    "100Hz",
    "200Hz",
    "500Hz",
};

// ============================================================
// Internal helpers
// ============================================================

static uint32_t ga_period_ms(const GpioAnalyzer* app) {
    uint32_t period = 1000U / k_sample_rates[app->rate_index];
    return (period < 1U) ? 1U : period;
}

static void ga_gpio_init(void) {
    for(uint8_t i = 0U; i < GA_PIN_COUNT; i++) {
        furi_hal_gpio_init(k_pins[i], GpioModeInput, GpioPullNo, GpioSpeedLow);
    }
}

static void ga_gpio_deinit(void) {
    // Return pins to high-impedance analog to avoid back-driving
    // devices connected to the header after the app exits.
    for(uint8_t i = 0U; i < GA_PIN_COUNT; i++) {
        furi_hal_gpio_init(k_pins[i], GpioModeAnalog, GpioPullNo, GpioSpeedLow);
    }
}

// Clears the sample history for all channels. Caller must NOT hold
// the mutex. This function acquires and releases it internally.
static void ga_clear_history(GpioAnalyzer* app) {
    furi_check(app != NULL);
    furi_mutex_acquire(app->mutex, FuriWaitForever);
    for(uint8_t i = 0U; i < GA_PIN_COUNT; i++) {
        memset(app->channels[i].history, 0, sizeof(app->channels[i].history));
        app->channels[i].history_head  = 0U;
        app->channels[i].sample_count  = 0U;
        app->channels[i].current_state = false;
    }
    furi_mutex_release(app->mutex);
}

// ============================================================
// CSV helpers
// ============================================================

static void ga_csv_open(GpioAnalyzer* app) {
    furi_check(app != NULL);
    if(app->csv_open) return;

    // Build timestamped filename using the RTC.
    DateTime dt;
    furi_hal_rtc_get_datetime(&dt);
    snprintf(
        app->csv_path,
        sizeof(app->csv_path),
        GA_CSV_DIR "/gla_%04d%02d%02d_%02d%02d%02d.csv",
        (int)dt.year,
        (int)dt.month,
        (int)dt.day,
        (int)dt.hour,
        (int)dt.minute,
        (int)dt.second);

    if(!storage_file_open(
           app->csv_file, app->csv_path, FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
        // No SD card or permission error = continue without CSV.
        notification_message(app->notifications, &sequence_error);
        return;
    }

    // Write header row: "sample,A7,A6,..." followed by CRLF for
    // maximum compatibility with spreadsheet applications.
    char header[48];
    int hlen = snprintf(
        header,
        sizeof(header),
        "sample,%s,%s,%s,%s,%s,%s,%s,%s\r\n",
        k_csv_headers[0], k_csv_headers[1], k_csv_headers[2],
        k_csv_headers[3], k_csv_headers[4], k_csv_headers[5],
        k_csv_headers[6], k_csv_headers[7]);
    if(hlen > 0 && hlen < (int)sizeof(header)) {
        storage_file_write(app->csv_file, header, (uint16_t)hlen);
    }

    app->csv_open     = true;
    app->sample_number = 0U;
    notification_message(app->notifications, &sequence_success);
}

static void ga_csv_close(GpioAnalyzer* app) {
    furi_check(app != NULL);
    if(!app->csv_open) return;
    storage_file_close(app->csv_file);
    app->csv_open = false;
}

// Writes one CSV data row.  Called from the timer callback while
// the mutex is already held by the caller.
static void ga_csv_write_row(GpioAnalyzer* app) {
    if(!app->csv_open || app->csv_file == NULL) return;

    char line[48];
    int len = snprintf(
        line,
        sizeof(line),
        "%lu,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
        (unsigned long)app->sample_number,
        (int)app->channels[0].current_state,
        (int)app->channels[1].current_state,
        (int)app->channels[2].current_state,
        (int)app->channels[3].current_state,
        (int)app->channels[4].current_state,
        (int)app->channels[5].current_state,
        (int)app->channels[6].current_state,
        (int)app->channels[7].current_state);
    if(len > 0 && len < (int)sizeof(line)) {
        storage_file_write(app->csv_file, line, (uint16_t)len);
    }
    app->sample_number++;
}

// ============================================================
// Capture start / stop
// ============================================================

static void ga_start_capture(GpioAnalyzer* app) {
    furi_check(app != NULL);
    if(app->running) return;
    ga_csv_open(app);
    app->running = true;
    furi_timer_start(app->sample_timer, ga_period_ms(app));
}

static void ga_stop_capture(GpioAnalyzer* app) {
    furi_check(app != NULL);
    if(!app->running) return;
    furi_timer_stop(app->sample_timer);
    app->running = false;
    ga_csv_close(app);
}

static void ga_apply_rate_change(GpioAnalyzer* app) {
    if(app->running) {
        furi_timer_stop(app->sample_timer);
        furi_timer_start(app->sample_timer, ga_period_ms(app));
    }
}

// ============================================================
// Timer callback (FreeRTOS timer service task)
// ============================================================

static void ga_sample_timer_cb(void* context) {
    GpioAnalyzer* app = (GpioAnalyzer*)context;
    furi_check(app != NULL);

    // Skip this tick if the draw callback is holding the mutex,
    // rather than stalling the timer service task.
    if(furi_mutex_acquire(app->mutex, 0U) != FuriStatusOk) return;

    for(uint8_t i = 0U; i < GA_PIN_COUNT; i++) {
        bool state = furi_hal_gpio_read(k_pins[i]);
        app->channels[i].current_state = state;

        uint32_t pos = app->channels[i].history_head;
        app->channels[i].history[pos] = state ? 1U : 0U;
        app->channels[i].history_head = (pos + 1U) % GA_HISTORY_SIZE;
        if(app->channels[i].sample_count < GA_HISTORY_SIZE) {
            app->channels[i].sample_count++;
        }
    }

    ga_csv_write_row(app);

    furi_mutex_release(app->mutex);

    // Signal the ViewDispatcher to redraw the analyzer view.
    // view_commit_model with update=true posts a paint message to
    // the GUI thread without acquiring the view model lock.
    view_commit_model(app->view_analyzer, true);
}

// ============================================================
// Draw callback (GUI thread)
// ============================================================

static void ga_analyzer_draw_cb(Canvas* canvas, void* model) {
    GaAnalyzerModel* m = (GaAnalyzerModel*)model;
    if(m == NULL || m->app == NULL) return;
    GpioAnalyzer* app = m->app;

    // Use a short timeout so a slow timer never stalls the GUI.
    if(furi_mutex_acquire(app->mutex, 25U) != FuriStatusOk) return;

    canvas_clear(canvas);
    canvas_set_font(canvas, FontSecondary);

    // ----------------------------------------------------------
    // Header bar
    // ----------------------------------------------------------
    canvas_draw_str(canvas, (int32_t)GA_LABEL_X, 7, "GPIO Analyzer");

    // Status: RUN/STP + rate + CSV indicator (* when logging).
    char status[20];
    snprintf(
        status,
        sizeof(status),
        "%s %s%s",
        app->running ? "RUN" : "STP",
        k_rate_labels[app->rate_index],
        app->csv_open ? "*" : " ");

    // Rough right-align: FontSecondary glyphs are ~6px wide.
    uint8_t sw = (uint8_t)(strlen(status) * 6U);
    int32_t sx = (int32_t)GA_SCREEN_W - (int32_t)sw - 1;
    if(sx < 0) sx = 0;
    canvas_draw_str(canvas, sx, 7, status);

    // Separator line below header.
    canvas_draw_line(
        canvas, 0, (int32_t)(GA_HEADER_H - 1U),
        (int32_t)(GA_SCREEN_W - 1U), (int32_t)(GA_HEADER_H - 1U));

    // ----------------------------------------------------------
    // Channel rows
    // ----------------------------------------------------------
    canvas_set_font(canvas, FontKeyboard);

    for(uint8_t i = 0U; i < GA_PIN_COUNT; i++) {
        const GaChannel* ch  = &app->channels[i];
        uint8_t          ry  = k_row_top[i];
        uint8_t          tb  = ry + GA_ROW_H - 1U;
        uint8_t          yhi = ry + GA_WAVE_Y_HIGH;
        uint8_t          ylo = ry + GA_WAVE_Y_LOW;

        // Pin label (left column)
        canvas_draw_str(canvas, (int32_t)GA_LABEL_X, (int32_t)tb, k_pin_labels[i]);

        // State indicator (right column).
        // FontKeyboard glyphs are ~8px tall but rows are only 7px,
        // causing text to bleed into the row above. Instead, draw
        // a pixel-precise 5x5 shape that fits cleanly within the row:
        // filled box = HIGH, outline box = LOW.
        // Box occupies y = ry+1 to ry+5, well within ry..ry+6.
        if(ch->current_state) {
            canvas_draw_box(canvas, (int32_t)GA_STATE_X + 1, (int32_t)ry + 1, 5, 5);
        } else {
            canvas_draw_frame(canvas, (int32_t)GA_STATE_X + 1, (int32_t)ry + 1, 5, 5);
        }

        // ----------------------------------------------------------
        // Waveform renderer
        //
        // The circular buffer holds up to GA_HISTORY_SIZE samples.
        // When full:  oldest = history[history_head]
        //             newest = history[(history_head - 1 + SIZE) % SIZE]
        // When not full: oldest = history[0], newest = history[sample_count-1]
        //
        // We display min(sample_count, GA_WAVE_W) samples L-to-R,
        // with the newest sample always at the rightmost pixel column.
        // ----------------------------------------------------------
        uint32_t count      = ch->sample_count;
        uint32_t head       = ch->history_head;
        uint32_t show       = (count < (uint32_t)GA_WAVE_W) ? count : (uint32_t)GA_WAVE_W;
        uint32_t skip       = (uint32_t)GA_WAVE_W - show;
        uint32_t oldest_idx = (count < GA_HISTORY_SIZE) ? 0U : head;
        uint8_t  prev_val   = 0xFFU;  // sentinel: no prior sample

        for(uint32_t s = skip; s < (uint32_t)GA_WAVE_W; s++) {
            uint32_t age        = s - skip;
            uint32_t sample_idx = (oldest_idx + age) % GA_HISTORY_SIZE;
            uint8_t  val        = ch->history[sample_idx];
            uint8_t  px         = (uint8_t)((uint32_t)GA_WAVE_X + s);
            uint8_t  py         = val ? yhi : ylo;

            // Draw vertical transition edge at rising and falling edges.
            if(prev_val != 0xFFU && prev_val != val) {
                canvas_draw_line(
                    canvas, (int32_t)px, (int32_t)yhi,
                    (int32_t)px, (int32_t)ylo);
            }
            canvas_draw_dot(canvas, (int32_t)px, (int32_t)py);
            prev_val = val;
        }
    }

    furi_mutex_release(app->mutex);
}

// ============================================================
// Input callback (GUI thread)
// ============================================================

static bool ga_analyzer_input_cb(InputEvent* event, void* context) {
    GpioAnalyzer* app = (GpioAnalyzer*)context;
    furi_check(app != NULL);
    bool consumed = false;

    if(event->type == InputTypeShort) {
        switch(event->key) {
        case InputKeyOk:
            view_dispatcher_send_custom_event(
                app->view_dispatcher, GaEventAnalyzerToggle);
            consumed = true;
            break;
        case InputKeyUp:
            view_dispatcher_send_custom_event(
                app->view_dispatcher, GaEventAnalyzerRateUp);
            consumed = true;
            break;
        case InputKeyDown:
            view_dispatcher_send_custom_event(
                app->view_dispatcher, GaEventAnalyzerRateDown);
            consumed = true;
            break;
        default:
            break;
        }
    } else if(event->type == InputTypeLong && event->key == InputKeyOk) {
        view_dispatcher_send_custom_event(
            app->view_dispatcher, GaEventAnalyzerClearHistory);
        consumed = true;
    }

    return consumed;
}

// ============================================================
// View setup (called once from ga_alloc)
// ============================================================

void scene_analyzer_setup_view(GpioAnalyzer* app) {
    furi_check(app != NULL);

    app->view_analyzer = view_alloc();
    furi_check(app->view_analyzer != NULL);

    view_set_draw_callback(app->view_analyzer, ga_analyzer_draw_cb);
    view_set_input_callback(app->view_analyzer, ga_analyzer_input_cb);
    view_set_context(app->view_analyzer, app);

    // LockFree model: just a pointer to app. We protect the actual
    // channel data with app->mutex rather than the view model lock.
    view_allocate_model(
        app->view_analyzer, ViewModelTypeLockFree, sizeof(GaAnalyzerModel));

    GaAnalyzerModel* m = view_get_model(app->view_analyzer);
    if(m != NULL) m->app = app;
    view_commit_model(app->view_analyzer, false);

    // Allocate the periodic sample timer here so the callback symbol
    // stays file-local. The timer is started/stopped by the scene.
    app->sample_timer = furi_timer_alloc(ga_sample_timer_cb, FuriTimerTypePeriodic, app);
    furi_check(app->sample_timer != NULL);
}

// ============================================================
// Scene handlers
// ============================================================

void scene_analyzer_on_enter(void* context) {
    GpioAnalyzer* app = (GpioAnalyzer*)context;
    furi_check(app != NULL);

    ga_gpio_init();
    ga_clear_history(app);
    ga_start_capture(app);

    view_dispatcher_switch_to_view(app->view_dispatcher, GaViewAnalyzer);
}

bool scene_analyzer_on_event(void* context, SceneManagerEvent event) {
    GpioAnalyzer* app = (GpioAnalyzer*)context;
    furi_check(app != NULL);
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
        case GaEventAnalyzerToggle:
            if(app->running) {
                ga_stop_capture(app);
            } else {
                ga_start_capture(app);
            }
            consumed = true;
            break;

        case GaEventAnalyzerClearHistory:
            // Stop the timer while clearing to prevent a partial-clear
            // race. Resume immediately after if capture was running.
            if(app->running) furi_timer_stop(app->sample_timer);
            ga_clear_history(app);
            if(app->running) furi_timer_start(app->sample_timer, ga_period_ms(app));
            consumed = true;
            break;

        case GaEventAnalyzerRateUp:
            if(app->rate_index < GA_RATE_COUNT - 1U) {
                app->rate_index++;
                ga_apply_rate_change(app);
            }
            consumed = true;
            break;

        case GaEventAnalyzerRateDown:
            if(app->rate_index > 0U) {
                app->rate_index--;
                ga_apply_rate_change(app);
            }
            consumed = true;
            break;

        default:
            break;
        }
    }

    return consumed;
}

void scene_analyzer_on_exit(void* context) {
    GpioAnalyzer* app = (GpioAnalyzer*)context;
    furi_check(app != NULL);

    ga_stop_capture(app);
    ga_gpio_deinit();
    // History is intentionally NOT cleared here. If the user
    // navigates back and re-enters, they see a blank screen anyway
    // because ga_clear_history() is called in on_enter.
}