#include "console_output.h"

void console_output_input_handler(CliguiApp* app, InputEvent* event) {
    if(event->type == InputTypeShort && (event->key == InputKeyOk || event->key == InputKeyLeft)) {
        view_dispatcher_switch_to_view(app->view_dispatcher, ViewTextInput);
        app->data->state = ViewTextInput;
    }
    if(event->type == InputTypeShort && event->key == InputKeyBack) {
        char eot = 0x03;
        furi_stream_buffer_send(app->data->streams.app_tx, &eot, 1, FuriWaitForever);
    }
    
}