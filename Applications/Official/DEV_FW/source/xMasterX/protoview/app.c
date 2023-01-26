/* Copyright (C) 2022-2023 Salvatore Sanfilippo -- All Rights Reserved
 * See the LICENSE file for information about the license. */

#include "app.h"

RawSamplesBuffer *RawSamples, *DetectedSamples;
extern const SubGhzProtocolRegistry protoview_protocol_registry;

/* Draw some text with a border. If the outside color is black and the inside
 * color is white, it just writes the border of the text, but the function can
 * also be used to write a bold variation of the font setting both the
 * colors to black, or alternatively to write a black text with a white
 * border so that it is visible if there are black stuff on the background. */
/* The callback actually just passes the control to the actual active
 * view callback, after setting up basic stuff like cleaning the screen
 * and setting color to black. */
static void render_callback(Canvas *const canvas, void *ctx) {
    ProtoViewApp *app = ctx;

    /* Clear screen. */
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_box(canvas, 0, 0, 127, 63);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontPrimary);

    /* Call who is in charge right now. */
    switch(app->current_view) {
    case ViewRawPulses: render_view_raw_pulses(canvas,app); break;
    case ViewInfo: render_view_info(canvas,app); break;
    case ViewFrequencySettings:
    case ViewModulationSettings:
        render_view_settings(canvas,app); break;
    case ViewLast: furi_crash(TAG " ViewLast selected"); break;
    }
}

/* Here all we do is putting the events into the queue that will be handled
 * in the while() loop of the app entry point function. */
static void input_callback(InputEvent* input_event, void* ctx)
{
    ProtoViewApp *app = ctx;
    furi_message_queue_put(app->event_queue,input_event,FuriWaitForever);
}

/* Allocate the application state and initialize a number of stuff.
 * This is called in the entry point to create the application state. */
ProtoViewApp* protoview_app_alloc() {
    ProtoViewApp *app = malloc(sizeof(ProtoViewApp));

    // Init shared data structures
    RawSamples = raw_samples_alloc();
    DetectedSamples = raw_samples_alloc();

    //init setting
    app->setting = subghz_setting_alloc();
    subghz_setting_load(app->setting, EXT_PATH("subghz/assets/setting_user"));

    // GUI
    app->gui = furi_record_open(RECORD_GUI);
    app->view_port = view_port_alloc();
    view_port_draw_callback_set(app->view_port, render_callback, app);
    view_port_input_callback_set(app->view_port, input_callback, app);
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);
    app->event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    app->current_view = ViewRawPulses;

    // Signal found and visualization defaults
    app->signal_bestlen = 0;
    app->signal_decoded = false;
    app->us_scale = PROTOVIEW_RAW_VIEW_DEFAULT_SCALE;
    app->signal_offset = 0;

    //init Worker & Protocol
    app->txrx = malloc(sizeof(ProtoViewTxRx));

    /* Setup rx worker and environment. */
    app->txrx->worker = subghz_worker_alloc();
    app->txrx->environment = subghz_environment_alloc();
    subghz_environment_set_protocol_registry(
        app->txrx->environment, (void*)&protoview_protocol_registry);
    app->txrx->receiver = subghz_receiver_alloc_init(app->txrx->environment);

    subghz_receiver_set_filter(app->txrx->receiver, SubGhzProtocolFlag_Decodable);
    subghz_worker_set_overrun_callback(
        app->txrx->worker, (SubGhzWorkerOverrunCallback)subghz_receiver_reset);
    subghz_worker_set_pair_callback(
        app->txrx->worker, (SubGhzWorkerPairCallback)subghz_receiver_decode);
    subghz_worker_set_context(app->txrx->worker, app->txrx->receiver);
    
    app->frequency = subghz_setting_get_default_frequency(app->setting);
    app->modulation = 0; /* Defaults to ProtoViewModulations[0]. */

    furi_hal_power_suppress_charge_enter();
    app->running = 1;

    return app;
}

/* Free what the application allocated. It is not clear to me if the
 * Flipper OS, once the application exits, will be able to reclaim space
 * even if we forget to free something here. */
void protoview_app_free(ProtoViewApp *app) {
    furi_assert(app);

    // Put CC1101 on sleep.
    radio_sleep(app);

    // View related.
    view_port_enabled_set(app->view_port, false);
    gui_remove_view_port(app->gui, app->view_port);
    view_port_free(app->view_port);
    furi_record_close(RECORD_GUI);
    furi_message_queue_free(app->event_queue);
    app->gui = NULL;

    // Frequency setting.
    subghz_setting_free(app->setting);

    // Worker stuff.
    subghz_receiver_free(app->txrx->receiver);
    subghz_environment_free(app->txrx->environment);
    subghz_worker_free(app->txrx->worker);
    free(app->txrx);

    // Raw samples buffers.
    raw_samples_free(RawSamples);
    raw_samples_free(DetectedSamples);
    furi_hal_power_suppress_charge_exit();

    free(app);
}

/* Called periodically. Do signal processing here. Data we process here
 * will be later displayed by the render callback. The side effect of this
 * function is to scan for signals and set DetectedSamples. */
static void timer_callback(void *ctx) {
    ProtoViewApp *app = ctx;
    scan_for_signal(app);
}

int32_t protoview_app_entry(void* p) {
    UNUSED(p);
    ProtoViewApp *app = protoview_app_alloc();

    /* Create a timer. We do data analysis in the callback. */
    FuriTimer *timer = furi_timer_alloc(timer_callback, FuriTimerTypePeriodic, app);
    furi_timer_start(timer, furi_kernel_get_tick_frequency() / 4);

    /* Start listening to signals immediately. */
    radio_begin(app);
    radio_rx(app);

    /* This is the main event loop: here we get the events that are pushed
     * in the queue by input_callback(), and process them one after the
     * other. The timeout is 100 milliseconds, so if not input is received
     * before such time, we exit the queue_get() function and call
     * view_port_update() in order to refresh our screen content. */
    InputEvent input;
    while(app->running) {
        FuriStatus qstat = furi_message_queue_get(app->event_queue, &input, 100);
        if (qstat == FuriStatusOk) {
            if (DEBUG_MSG) FURI_LOG_E(TAG, "Main Loop - Input: type %d key %u",
                    input.type, input.key);

            /* Handle navigation here. Then handle view-specific inputs
             * in the view specific handling function. */
            if (input.type == InputTypeShort &&
                input.key == InputKeyBack)
            {
                /* Exit the app. */
                app->running = 0;
            } else if (input.type == InputTypeShort &&
                       input.key == InputKeyRight)
            {
                /* Go to the next view. */
                app->current_view++;
                if (app->current_view == ViewLast) app->current_view = 0;
            } else if (input.type == InputTypeShort &&
                       input.key == InputKeyLeft)
            {
                /* Go to the previous view. */
                if (app->current_view == 0)
                    app->current_view = ViewLast-1;
                else
                    app->current_view--;
            } else {
                /* This is where we pass the control to the currently
                 * active view input processing. */
                switch(app->current_view) {
                case ViewRawPulses:
                    process_input_raw_pulses(app,input);
                    break;
                case ViewInfo:
                    process_input_info(app,input);
                    break;
                case ViewFrequencySettings:
                case ViewModulationSettings:
                    process_input_settings(app,input);
                    break;
                case ViewLast: furi_crash(TAG " ViewLast selected"); break;
                }
            }
        } else {
            /* Useful to understand if the app is still alive when it
             * does not respond because of bugs. */
            if (DEBUG_MSG) {
                static int c = 0; c++;
                if (!(c % 20)) FURI_LOG_E(TAG, "Loop timeout");
            }
        }
        view_port_update(app->view_port);
    }

    /* App no longer running. Shut down and free. */
    if (app->txrx->txrx_state == TxRxStateRx) {
        FURI_LOG_E(TAG, "Putting CC1101 to sleep before exiting.");
        radio_rx_end(app);
        radio_sleep(app);
    }

    furi_timer_free(timer);
    protoview_app_free(app);
    return 0;
}

