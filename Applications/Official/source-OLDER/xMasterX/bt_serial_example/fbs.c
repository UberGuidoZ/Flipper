#include "fbs.h"

const uint16_t BT_SERIAL_BUFFER_SIZE = 128;

void draw_callback(Canvas* canvas, void* ctx) {
    FBS* app = ctx;
    furi_check(furi_mutex_acquire(app->app_mutex, FuriWaitForever) == FuriStatusOk);

    canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignCenter, (char*)app->display_text);

    furi_mutex_release(app->app_mutex);
}

void input_callback(InputEvent* input, void* ctx) {
    FBS* app = ctx;
    furi_message_queue_put(app->event_queue, input, FuriWaitForever);
}

FBS* fbs_alloc() {
    FBS* app = malloc(sizeof(FBS));
    app->app_mutex = furi_mutex_alloc(FuriMutexTypeNormal);
    app->event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    app->gui = furi_record_open(RECORD_GUI);
    app->view_port = view_port_alloc();
    view_port_draw_callback_set(app->view_port, draw_callback, app);
    view_port_input_callback_set(app->view_port, input_callback, app);
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);

    app->bt_connected = false;
    app->bt = furi_record_open(RECORD_BT);
    return app;
}

void fbs_free(FBS* app) {
    view_port_enabled_set(app->view_port, false);
    gui_remove_view_port(app->gui, app->view_port);
    furi_record_close(RECORD_GUI);
    app->gui = NULL;
    view_port_free(app->view_port);
    free(app->display_text);

    furi_mutex_free(app->app_mutex);
    furi_message_queue_free(app->event_queue);

    furi_record_close(RECORD_BT);
    app->bt = NULL;

    free(app);
}

static uint16_t bt_serial_event_callback(SerialServiceEvent event, void* context) {
    furi_assert(context);
    Bt* bt = context;
    UNUSED(bt);
    uint16_t ret = 0;

    if(event.event == SerialServiceEventTypeDataReceived) {
        FURI_LOG_D(TAG, "SerialServiceEventTypeDataReceived");
        FURI_LOG_D(TAG, "Size: %u", event.data.size);
        FURI_LOG_D(TAG, "Data: ");
        for (size_t i = 0; i < event.data.size; i++)
        {
            printf("%X ", event.data.buffer[i]);
        }
        printf("\r\n");
    } else if(event.event == SerialServiceEventTypeDataSent) {
        FURI_LOG_D(TAG, "SerialServiceEventTypeDataSent");
        FURI_LOG_D(TAG, "Size: %u", event.data.size);
        FURI_LOG_D(TAG, "Data: ");
        for (size_t i = 0; i < event.data.size; i++)
        {
            printf("%X ", event.data.buffer[i]);
        }
        printf("\r\n");
    }
    return ret;
}

int32_t fbs_app(void* p) {
    UNUSED(p);
    FBS* app = fbs_alloc();

    if (furi_hal_bt_is_active()) {
        FURI_LOG_D(TAG, "BT is working, hijacking the serial connection...");
        furi_hal_bt_serial_set_event_callback(BT_SERIAL_BUFFER_SIZE, bt_serial_event_callback, app);
        furi_hal_bt_start_advertising();
    } else {
        FURI_LOG_D(TAG, "Please, enable the Bluetooth and restart the app");
    }
    
    InputEvent event;
    for(bool processing = true; processing;) {
        int status = furi_message_queue_get(app->event_queue, &event, 100);
        furi_check(furi_mutex_acquire(app->app_mutex, FuriWaitForever) == FuriStatusOk);
        if(status == FuriStatusOk && event.type == InputTypePress && event.key == InputKeyBack) {
            processing = false;
        }
        furi_mutex_release(app->app_mutex);
        view_port_update(app->view_port);
    }

    furi_hal_bt_serial_set_event_callback(0, NULL, NULL);

    fbs_free(app);
    FURI_LOG_D(TAG, "Released everything");
    return 0;
}