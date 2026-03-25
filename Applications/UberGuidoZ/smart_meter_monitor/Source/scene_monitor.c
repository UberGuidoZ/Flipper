// ############################################################
// #  scene_monitor.c - CC1101 capture, waveform, burst stats #
// #  By: UberGuidoZ | https://github.com/UberGuidoZ/         #
// #  v1.11                                                   #
// ############################################################
//
//  The CC1101 is polled by a FuriTimer at the selected sample
//  rate. RSSI values fill a 128-sample circular buffer that maps
//  1:1 to the 128px-wide waveform (newest = rightmost pixel).
//  A dashed threshold line across the waveform marks the burst
//  detection level, adjustable in 5 dBm steps with UP/DOWN.
//
//  CONTROLS:
//    OK (short)   - reset stats (count, intervals)
//    OK (long)    - pause / resume capture
//    UP           - raise detection threshold (less sensitive)
//    DOWN         - lower detection threshold (more sensitive)
//    LEFT         - cycle to previous frequency
//    RIGHT        - cycle to next frequency
//    BACK         - stop radio, return to menu
//
// ##############################################################

#include "smart_meter.h"
#include <string.h>
#include <stdio.h>

// ============================================================
// Frequency and sample rate tables
// ============================================================
static const uint32_t k_frequencies[SM_FREQ_COUNT] = {
    433920000U,   // 433.92 MHz - some water meters
    868300000U,   // 868.30 MHz - EU electric / gas
    915000000U,   // 915.00 MHz - US electric / gas (ERT/SCM)
};

static const char* const k_freq_labels[SM_FREQ_COUNT] = {
    "433MHz",
    "868MHz",
    "915MHz",
};

static const uint32_t k_sample_rates[SM_RATE_COUNT] = {
    100U,    //  100 ms - 13s  history
    250U,    //  250 ms - 32s  history
    500U,    //  500 ms - 64s  history (default)
    1000U,   // 1000 ms - 128s history
    2000U,   // 2000 ms - 256s history
};

static const char* const k_rate_labels[SM_RATE_COUNT] = {
    "100ms",
    "250ms",
    "500ms",
    "1s",
    "2s",
};

// ============================================================
// Radio helpers
// ============================================================

static void sm_radio_start(SmartMeterApp* app) {
    furi_check(app != NULL);
    if(app->radio_running) return;

    furi_hal_subghz_reset();
    furi_hal_subghz_set_frequency_and_path(k_frequencies[app->freq_index]);
    furi_hal_subghz_rx();
    app->radio_running = true;
}

static void sm_radio_stop(SmartMeterApp* app) {
    furi_check(app != NULL);
    if(!app->radio_running) return;

    furi_hal_subghz_idle();
    furi_hal_subghz_sleep();
    app->radio_running = false;
}

// Restart radio on frequency change without clearing stats.
static void sm_radio_restart(SmartMeterApp* app) {
    sm_radio_stop(app);
    sm_radio_start(app);
}

// ============================================================
// Stats reset (does not touch the RSSI history or radio)
// ============================================================
static void sm_reset_stats(SmartMeterApp* app) {
    furi_check(app != NULL);
    furi_mutex_acquire(app->mutex, FuriWaitForever);
    app->burst_count          = 0U;
    app->last_burst_tick      = 0U;
    app->last_interval_ms     = 0U;
    app->avg_interval_ms      = 0U;
    app->burst_ts_head        = 0U;
    app->burst_ts_count       = 0U;
    app->tx_per_min           = 0U;
    app->burst_enter_tick     = 0U;
    app->burst_sample_count   = 0U;
    app->last_burst_samples   = 0U;
    app->in_burst             = false;
    app->burst_above          = 0U;
    app->burst_below          = 0U;
    // Clear the RSSI history so the waveform starts fresh.
    memset(app->rssi_history, (uint8_t)SM_RSSI_MIN_DBM, sizeof(app->rssi_history));
    app->rssi_head  = 0U;
    app->rssi_count = 0U;
    furi_mutex_release(app->mutex);
}

// ============================================================
// Timer callback (FreeRTOS timer service task)
// ============================================================
static void sm_sample_timer_cb(void* context) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);

    if(furi_mutex_acquire(app->mutex, 0U) != FuriStatusOk) return;

    if(app->radio_running && app->monitoring) {
        // Read RSSI from CC1101.
        float rssi_f = furi_hal_subghz_get_rssi();
        int8_t rssi;
        if(rssi_f      <= (float)SM_RSSI_MIN_DBM) rssi = (int8_t)SM_RSSI_MIN_DBM;
        else if(rssi_f >= (float)SM_RSSI_MAX_DBM) rssi = (int8_t)SM_RSSI_MAX_DBM;
        else                                       rssi = (int8_t)rssi_f;

        app->rssi_current = rssi;

        // Write into circular buffer.
        app->rssi_history[app->rssi_head] = rssi;
        app->rssi_head = (app->rssi_head + 1U) % SM_HISTORY_SIZE;
        if(app->rssi_count < SM_HISTORY_SIZE) app->rssi_count++;

        // Burst detection with hysteresis.
        bool above = (rssi > app->threshold_dbm);

        if(!app->in_burst) {
            // Not currently in a burst - look for entry condition.
            if(above) {
                app->burst_above++;
                if(app->burst_above >= SM_BURST_ENTER_SAMPLES) {
                    app->in_burst           = true;
                    app->burst_below        = 0U;
                    app->burst_sample_count = app->burst_above;
                    app->burst_enter_tick   = furi_get_tick();
                }
            } else {
                app->burst_above = 0U;
            }
        } else {
            // In a burst - count samples and look for exit condition.
            if(above) {
                app->burst_sample_count++;
                app->burst_below = 0U;
            } else {
                app->burst_below++;
                if(app->burst_below >= SM_BURST_EXIT_SAMPLES) {
                    // Burst ended - record all stats.
                    app->in_burst             = false;
                    app->burst_above          = 0U;
                    app->last_burst_samples   = app->burst_sample_count;
                    app->burst_sample_count   = 0U;

                    uint32_t now = furi_get_tick();

                    // Interval tracking.
                    if(app->burst_count > 0U && app->last_burst_tick > 0U) {
                        app->last_interval_ms = now - app->last_burst_tick;
                        if(app->avg_interval_ms == 0U) {
                            app->avg_interval_ms = app->last_interval_ms;
                        } else {
                            app->avg_interval_ms =
                                (app->avg_interval_ms * 3U + app->last_interval_ms) / 4U;
                        }
                    }
                    app->last_burst_tick = now;
                    app->burst_count++;

                    // Tx/min: store timestamp in circular buffer.
                    app->burst_timestamps[app->burst_ts_head] = now;
                    app->burst_ts_head =
                        (uint8_t)((app->burst_ts_head + 1U) % 30U);
                    if(app->burst_ts_count < 30U) app->burst_ts_count++;

                    // Count entries within the last 60 seconds.
                    uint8_t tpm = 0U;
                    for(uint8_t t = 0U; t < app->burst_ts_count; t++) {
                        if((now - app->burst_timestamps[t]) <= 60000U) tpm++;
                    }
                    app->tx_per_min = tpm;

                    notification_message(app->notifications, &sequence_blink_green_10);
                }
            }
        }
    }

    furi_mutex_release(app->mutex);
    view_commit_model(app->view_monitor, true);
}

// ============================================================
// Draw callback (GUI thread)
// ============================================================
static void sm_monitor_draw_cb(Canvas* canvas, void* model) {
    SmMonitorModel* m = (SmMonitorModel*)model;
    if(m == NULL || m->app == NULL) return;
    SmartMeterApp* app = m->app;

    // Always clear so stale pixels never persist on mutex timeout.
    canvas_clear(canvas);

    if(furi_mutex_acquire(app->mutex, 25U) != FuriStatusOk) return;

    // ----------------------------------------------------------
    // Header bar
    // ----------------------------------------------------------
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 1, 7, "SMM");

    // Right side: frequency, rate, and paused indicator.
    char hdr[24];
    snprintf(hdr, sizeof(hdr), "%s %s%s",
        k_freq_labels[app->freq_index],
        k_rate_labels[app->rate_index],
        app->monitoring ? "" : " II");
    size_t hw = canvas_string_width(canvas, hdr);
    canvas_draw_str(canvas, (int32_t)(SM_SCREEN_W - 1U - hw), 7, hdr);

    // Separator
    canvas_draw_line(canvas, 0, (int32_t)(SM_HEADER_H - 1U),
                     (int32_t)(SM_SCREEN_W - 1U), (int32_t)(SM_HEADER_H - 1U));

    // ----------------------------------------------------------
    // RSSI waveform (bar graph, newest = rightmost column)
    // ----------------------------------------------------------
    uint32_t count      = app->rssi_count;
    uint32_t head       = app->rssi_head;
    uint32_t show       = count < SM_WAVE_W ? count : SM_WAVE_W;
    uint32_t oldest_idx = count < SM_HISTORY_SIZE ? 0U : head;

    for(uint32_t s = 0U; s < show; s++) {
        uint32_t x          = SM_WAVE_W - show + s;
        uint32_t sample_idx = (oldest_idx + s) % SM_HISTORY_SIZE;
        int8_t   rssi       = app->rssi_history[sample_idx];

        int32_t h = ((int32_t)rssi - SM_RSSI_MIN_DBM) * (int32_t)SM_WAVE_H / SM_RSSI_RANGE;
        if(h < 0)              h = 0;
        if(h > (int32_t)SM_WAVE_H) h = (int32_t)SM_WAVE_H;

        if(h > 0) {
            canvas_draw_line(
                canvas,
                (int32_t)x, (int32_t)SM_WAVE_Y_BOT,
                (int32_t)x, (int32_t)SM_WAVE_Y_BOT - h + 1);
        }
    }

    // ----------------------------------------------------------
    // Threshold line (2-on, 2-off dashed across full width)
    // ----------------------------------------------------------
    int32_t thr_h = ((int32_t)app->threshold_dbm - SM_RSSI_MIN_DBM) *
                    (int32_t)SM_WAVE_H / SM_RSSI_RANGE;
    if(thr_h < 0)               thr_h = 0;
    if(thr_h > (int32_t)SM_WAVE_H) thr_h = (int32_t)SM_WAVE_H;
    int32_t thr_y = (int32_t)SM_WAVE_Y_BOT - thr_h;
    for(int32_t x = 0; x < (int32_t)SM_SCREEN_W; x += 4) {
        canvas_draw_dot(canvas, x,     thr_y);
        canvas_draw_dot(canvas, x + 1, thr_y);
    }

    // ----------------------------------------------------------
    // Stats line 1: RSSI, burst count, tx/min
    // ----------------------------------------------------------
    canvas_set_font(canvas, FontKeyboard);
    char s1[20];
    snprintf(s1, sizeof(s1), "%4ddBm C:%03lu %2u/m",
        (int)app->rssi_current,
        (unsigned long)app->burst_count,
        (unsigned)app->tx_per_min);
    canvas_draw_str(canvas, 0, (int32_t)SM_STATS1_Y, s1);

    // ----------------------------------------------------------
    // Stats line 2: intervals + SCM framing hint
    //
    // SCM heuristic: brief above-threshold events (1-2 samples)
    // are consistent with short meter packets. Sustained events
    // (4+ samples at any poll rate) suggest interference.
    // This is energy-based estimation only, not actual decoding.
    // ----------------------------------------------------------
    char s2[28];
    if(app->burst_count < 2U) {
        snprintf(s2, sizeof(s2), "Thr:%3ddBm", (int)app->threshold_dbm);
    } else {
        uint32_t lst_s = app->last_interval_ms / 1000U;
        uint32_t avg_s = app->avg_interval_ms  / 1000U;
        if(lst_s > 999U) lst_s = 999U;
        if(avg_s > 999U) avg_s = 999U;

        const char* hint = "";
        if(app->last_burst_samples <= 2U) {
            hint = " ~SCM";   // brief: consistent with meter packet
        } else if(app->last_burst_samples >= 4U) {
            hint = " INT?";   // sustained: likely interference
        }

        snprintf(s2, sizeof(s2), "I:%3lus A:%3lus%s",
            (unsigned long)lst_s,
            (unsigned long)avg_s,
            hint);
    }
    canvas_draw_str(canvas, 0, (int32_t)SM_STATS2_Y, s2);

    furi_mutex_release(app->mutex);
}

// ============================================================
// Input callback (GUI thread)
// ============================================================
static bool sm_monitor_input_cb(InputEvent* event, void* context) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);

    if(event->type == InputTypeShort) {
        switch(event->key) {
        case InputKeyOk:
            // Skip toggle if a long press was just consumed for reset.
            if(app->ok_long_consumed) {
                app->ok_long_consumed = false;
                return true;
            }
            view_dispatcher_send_custom_event(
                app->view_dispatcher, SmEventMonitorToggle);
            return true;
        case InputKeyUp:
            view_dispatcher_send_custom_event(
                app->view_dispatcher, SmEventMonitorThreshUp);
            return true;
        case InputKeyDown:
            view_dispatcher_send_custom_event(
                app->view_dispatcher, SmEventMonitorThreshDown);
            return true;
        case InputKeyLeft:
            view_dispatcher_send_custom_event(
                app->view_dispatcher, SmEventMonitorFreqPrev);
            return true;
        case InputKeyRight:
            view_dispatcher_send_custom_event(
                app->view_dispatcher, SmEventMonitorFreqNext);
            return true;
        default:
            break;
        }
    } else if(event->type == InputTypeLong) {
        switch(event->key) {
        case InputKeyOk:
            app->ok_long_consumed = true;
            view_dispatcher_send_custom_event(
                app->view_dispatcher, SmEventMonitorReset);
            return true;
        case InputKeyUp:
            view_dispatcher_send_custom_event(
                app->view_dispatcher, SmEventMonitorRateUp);
            return true;
        case InputKeyDown:
            view_dispatcher_send_custom_event(
                app->view_dispatcher, SmEventMonitorRateDown);
            return true;
        default:
            break;
        }
    }

    return false;
}

// ============================================================
// View setup (called once from sm_alloc)
// ============================================================
void scene_monitor_setup_view(SmartMeterApp* app) {
    furi_check(app != NULL);

    app->view_monitor = view_alloc();
    furi_check(app->view_monitor != NULL);

    view_set_draw_callback(app->view_monitor, sm_monitor_draw_cb);
    view_set_input_callback(app->view_monitor, sm_monitor_input_cb);
    view_set_context(app->view_monitor, app);

    view_allocate_model(
        app->view_monitor, ViewModelTypeLockFree, sizeof(SmMonitorModel));

    SmMonitorModel* m = view_get_model(app->view_monitor);
    if(m != NULL) m->app = app;
    view_commit_model(app->view_monitor, false);

    // Allocate the sample timer here so the callback symbol stays local.
    app->sample_timer = furi_timer_alloc(sm_sample_timer_cb, FuriTimerTypePeriodic, app);
    furi_check(app->sample_timer != NULL);
}

// ============================================================
// Scene handlers
// ============================================================
void scene_monitor_on_enter(void* context) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);

    sm_reset_stats(app);
    sm_radio_start(app);
    furi_timer_start(app->sample_timer, k_sample_rates[app->rate_index]);

    view_dispatcher_switch_to_view(app->view_dispatcher, SmViewMonitor);
}

bool scene_monitor_on_event(void* context, SceneManagerEvent event) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);

    if(event.type != SceneManagerEventTypeCustom) return false;

    switch(event.event) {

    case SmEventMonitorToggle:
        app->monitoring = !app->monitoring;
        return true;

    case SmEventMonitorReset:
        sm_reset_stats(app);
        return true;

    case SmEventMonitorThreshUp:
        if(app->threshold_dbm < (int8_t)SM_THRESHOLD_MAX) {
            app->threshold_dbm += (int8_t)SM_THRESHOLD_STEP;
        }
        return true;

    case SmEventMonitorThreshDown:
        if(app->threshold_dbm > (int8_t)SM_THRESHOLD_MIN) {
            app->threshold_dbm -= (int8_t)SM_THRESHOLD_STEP;
        }
        return true;

    case SmEventMonitorFreqNext:
        app->freq_index = (uint8_t)((app->freq_index + 1U) % SM_FREQ_COUNT);
        sm_radio_restart(app);
        sm_reset_stats(app);
        return true;

    case SmEventMonitorFreqPrev:
        app->freq_index = (uint8_t)(
            app->freq_index > 0U ? app->freq_index - 1U : SM_FREQ_COUNT - 1U);
        sm_radio_restart(app);
        sm_reset_stats(app);
        return true;

    case SmEventMonitorRateUp:
        if(app->rate_index < SM_RATE_COUNT - 1U) {
            app->rate_index++;
            furi_timer_stop(app->sample_timer);
            furi_timer_start(app->sample_timer, k_sample_rates[app->rate_index]);
        }
        return true;

    case SmEventMonitorRateDown:
        if(app->rate_index > 0U) {
            app->rate_index--;
            furi_timer_stop(app->sample_timer);
            furi_timer_start(app->sample_timer, k_sample_rates[app->rate_index]);
        }
        return true;

    default:
        break;
    }
    return false;
}

void scene_monitor_on_exit(void* context) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);
    furi_timer_stop(app->sample_timer);
    sm_radio_stop(app);
}