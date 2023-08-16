#include "text_input.h"
#include "cligui_main_i.h"

void text_input_result_callback(void* ctx) {
    CliguiApp* app = ctx;
    char* data = app->text_input_store;
    size_t len = strlen(data);
    for(size_t i = 0; i < len; i++) {
        if(data[i] >= 0x41 && data[i] <= 0x5A) {
            // Char is uppercase
            data[i] += 0x20;
        }
    }
    furi_stream_buffer_send(app->data->streams.app_tx, data, len, FuriWaitForever);
    furi_stream_buffer_send(app->data->streams.app_tx, "\r\n", 2, FuriWaitForever);
    data[0] = 0;
    view_dispatcher_switch_to_view(app->view_dispatcher, ViewConsoleOutput);
    app->data->state = ViewConsoleOutput;
}

void text_input_input_handler(CliguiApp* app, InputEvent* event) {
    if(event->type == InputTypeShort && event->key == InputKeyBack) {
        // view_dispatcher_switch_to_view(app->view_dispatcher, ViewConsoleOutput);
        // app->data->state = ViewConsoleOutput;
        size_t len = strlen(app->text_input_store);
        app->text_input_store[len] = ' ';
        app->text_input_store[len + 1] = 0;
    }
    if(event->type == InputTypeLong && (event->key == InputKeyLeft || event->key == InputKeyRight)) {
        view_dispatcher_switch_to_view(app->view_dispatcher, ViewConsoleOutput);
        app->data->state = ViewConsoleOutput;
    }
}