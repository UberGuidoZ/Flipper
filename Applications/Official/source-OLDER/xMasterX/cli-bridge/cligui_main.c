#include "cligui_main_i.h"
#include "cli_control.h"
#include "text_input.h"
#include "console_output.h"

static bool cligui_custom_event_cb(void* context, uint32_t event) {
    UNUSED(event);
    CliguiApp* app = context;
    UNUSED(app);
    return true;
}
static bool cligui_back_event_cb(void* context) {
    CliguiApp* app = context;
    UNUSED(app);
    return true;
}
static void cligui_tick_event_cb(void* context) {
    CliguiApp* app = context;
    size_t available = furi_stream_buffer_bytes_available(app->data->streams.app_rx);
    for(size_t i = 0; i < available; i++) {
        char c = 0;
        size_t len = furi_stream_buffer_receive(app->data->streams.app_rx, &c, 1, 100);
        if(len > 0) {
            furi_string_push_back(app->text_box_store, c);
        }
    }
    if(available > 0) {
        text_box_set_text(app->text_box, furi_string_get_cstr(app->text_box_store));
    }
    // Set input header stuff
    size_t len = furi_string_size(app->text_box_store);
    size_t idx = len - 2;
    while(idx > 0) {
        if(furi_string_get_char(app->text_box_store, idx) == '\n') {
            idx++;
            break;
        }
        idx--;
    }
    text_input_set_header_text(app->text_input, furi_string_get_cstr(app->text_box_store) + idx);
    UNUSED(app);
}

ViewPortInputCallback prev_input_callback;
volatile bool persistent_exit = false;
static void input_callback_wrapper(InputEvent* event, void* context) {
    CliguiApp* app = context;
    if(event->type == InputTypeLong && event->key == InputKeyBack) {
        persistent_exit = false;
        view_dispatcher_stop(app->view_dispatcher);
    }
    if(event->type == InputTypeLong && event->key == InputKeyOk) {
        if(app->data->state == ViewConsoleOutput) {
            persistent_exit = true;
            view_dispatcher_stop(app->view_dispatcher);
        }
    }
    if(app->data->state == ViewTextInput) {
        text_input_input_handler(app, event);
    } else {
        console_output_input_handler(app, event);
    }
    prev_input_callback(event, app->view_dispatcher);
}

int32_t cligui_main(void* p) {
    UNUSED(p);
    CliguiApp* cligui = malloc(sizeof(CliguiApp));
    cligui->data = malloc(sizeof(CliguiData));

    latch_tx_handler();
    cligui->data->streams.app_tx = rx_stream;
    cligui->data->streams.app_rx = tx_stream;

    cligui->gui = furi_record_open(RECORD_GUI);
    cligui->view_dispatcher = view_dispatcher_alloc();
    cligui->view_dispatcher_i = (ViewDispatcher_internal*)(cligui->view_dispatcher);
    prev_input_callback =
        ((ViewPort_internal*)cligui->view_dispatcher_i->view_port)->input_callback;
    view_port_input_callback_set(
        cligui->view_dispatcher_i->view_port, input_callback_wrapper, cligui);
    view_dispatcher_enable_queue(cligui->view_dispatcher);
    view_dispatcher_set_event_callback_context(cligui->view_dispatcher, cligui);
    view_dispatcher_set_custom_event_callback(cligui->view_dispatcher, cligui_custom_event_cb);
    view_dispatcher_set_navigation_event_callback(cligui->view_dispatcher, cligui_back_event_cb);
    view_dispatcher_set_tick_event_callback(cligui->view_dispatcher, cligui_tick_event_cb, 100);

    view_dispatcher_attach_to_gui(
        cligui->view_dispatcher, cligui->gui, ViewDispatcherTypeFullscreen);

    view_dispatcher_send_to_front(cligui->view_dispatcher);

    cligui->text_box = text_box_alloc();
    view_dispatcher_add_view(
        cligui->view_dispatcher, ViewConsoleOutput, text_box_get_view(cligui->text_box));
    cligui->text_box_store = furi_string_alloc();
    furi_string_reserve(cligui->text_box_store, TEXT_BOX_STORE_SIZE);
    furi_string_set_char(cligui->text_box_store, 0, 0);
    text_box_set_text(cligui->text_box, furi_string_get_cstr(cligui->text_box_store));
    text_box_set_focus(cligui->text_box, TextBoxFocusEnd);

    cligui->text_input = text_input_alloc();
    text_input_set_result_callback(
        cligui->text_input,
        text_input_result_callback,
        cligui,
        cligui->text_input_store,
        TEXT_INPUT_STORE_SIZE,
        true);
    view_dispatcher_add_view(
        cligui->view_dispatcher, ViewTextInput, text_input_get_view(cligui->text_input));

    view_dispatcher_switch_to_view(cligui->view_dispatcher, ViewTextInput);
    cligui->data->state = ViewTextInput;

    view_dispatcher_run(cligui->view_dispatcher);

    view_dispatcher_remove_view(cligui->view_dispatcher, ViewConsoleOutput);
    view_dispatcher_remove_view(cligui->view_dispatcher, ViewTextInput);
    text_box_free(cligui->text_box);
    furi_string_free(cligui->text_box_store);
    text_input_free(cligui->text_input);
    view_dispatcher_free(cligui->view_dispatcher);

    unlatch_tx_handler(persistent_exit);

    furi_record_close(RECORD_GUI);

    free(cligui->data);
    free(cligui);

    return 0;
}