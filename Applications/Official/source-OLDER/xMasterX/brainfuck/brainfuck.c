#include "brainfuck_i.h"

/*
    Due to the lack of documentation on the flipper i copied the picopass app,
    ripped its insides out and used its hollow corpse to build this app inside of.

    i dont know how this stuff works and after 6 hours of trying to learn it, i dont care
*/

bool brainfuck_custom_event_callback(void* context, uint32_t event) {
    furi_assert(context);
    BFApp* brainfuck = context;
    return scene_manager_handle_custom_event(brainfuck->scene_manager, event);
}

bool brainfuck_back_event_callback(void* context) {
    furi_assert(context);
    BFApp* brainfuck = context;
    return scene_manager_handle_back_event(brainfuck->scene_manager);
}

BFApp* brainfuck_alloc() {
    BFApp* brainfuck = malloc(sizeof(BFApp));

    brainfuck->dataSize = 0;
    brainfuck->view_dispatcher = view_dispatcher_alloc();
    brainfuck->scene_manager = scene_manager_alloc(&brainfuck_scene_handlers, brainfuck);
    view_dispatcher_enable_queue(brainfuck->view_dispatcher);
    view_dispatcher_set_event_callback_context(brainfuck->view_dispatcher, brainfuck);
    view_dispatcher_set_custom_event_callback(brainfuck->view_dispatcher, brainfuck_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(brainfuck->view_dispatcher, brainfuck_back_event_callback);

    // Open GUI record
    brainfuck->gui = furi_record_open(RECORD_GUI);
    view_dispatcher_attach_to_gui(brainfuck->view_dispatcher, brainfuck->gui, ViewDispatcherTypeFullscreen);

    // Open Notification record
    brainfuck->notifications = furi_record_open(RECORD_NOTIFICATION);

    // Submenu
    brainfuck->submenu = submenu_alloc();
    view_dispatcher_add_view(brainfuck->view_dispatcher, brainfuckViewMenu, submenu_get_view(brainfuck->submenu));

    // Popup
    brainfuck->popup = popup_alloc();
    view_dispatcher_add_view(brainfuck->view_dispatcher, brainfuckViewPopup, popup_get_view(brainfuck->popup));

    // Text Input
    brainfuck->text_input = text_input_alloc();
    view_dispatcher_add_view(brainfuck->view_dispatcher, brainfuckViewTextInput, text_input_get_view(brainfuck->text_input));

    // Textbox
    brainfuck->text_box = text_box_alloc();
    view_dispatcher_add_view(brainfuck->view_dispatcher, brainfuckViewTextBox, text_box_get_view(brainfuck->text_box)); 
    brainfuck->text_box_store = furi_string_alloc();

    // Dev environment
    brainfuck->BF_dev_env = bf_dev_env_alloc(brainfuck);
    view_dispatcher_add_view(brainfuck->view_dispatcher, brainfuckViewDev, bf_dev_env_get_view(brainfuck->BF_dev_env));

    // File path
    brainfuck->BF_file_path = furi_string_alloc();

    return brainfuck;
}

void brainfuck_free(BFApp* brainfuck) {
    furi_assert(brainfuck);

    // Submenu
    view_dispatcher_remove_view(brainfuck->view_dispatcher, brainfuckViewMenu);
    submenu_free(brainfuck->submenu);

    // Popup
    view_dispatcher_remove_view(brainfuck->view_dispatcher, brainfuckViewPopup);
    popup_free(brainfuck->popup);

    // TextInput
    view_dispatcher_remove_view(brainfuck->view_dispatcher, brainfuckViewTextInput);
    text_input_free(brainfuck->text_input);

    // TextBox
    view_dispatcher_remove_view(brainfuck->view_dispatcher, brainfuckViewTextBox);
    text_box_free(brainfuck->text_box);
    furi_string_free(brainfuck->text_box_store);

    //dev env
    view_dispatcher_remove_view(brainfuck->view_dispatcher, brainfuckViewDev);
    bf_dev_env_free(brainfuck->BF_dev_env);

    // View Dispatcher
    view_dispatcher_free(brainfuck->view_dispatcher);

    // Scene Manager
    scene_manager_free(brainfuck->scene_manager);

    // GUI
    furi_record_close(RECORD_GUI);
    brainfuck->gui = NULL;

    // Notifications
    furi_record_close(RECORD_NOTIFICATION);
    brainfuck->notifications = NULL;

    free(brainfuck);
}

void brainfuck_show_loading_popup(void* context, bool show) {
    BFApp* brainfuck = context;
    TaskHandle_t timer_task = xTaskGetHandle(configTIMER_SERVICE_TASK_NAME);

    if(show) {
        // Raise timer priority so that animations can play
        vTaskPrioritySet(timer_task, configMAX_PRIORITIES - 1);
        view_dispatcher_switch_to_view(brainfuck->view_dispatcher, brainfuckViewLoading);
    } else {
        // Restore default timer priority
        vTaskPrioritySet(timer_task, configTIMER_TASK_PRIORITY);
    }
}

int32_t brainfuck_app(void* p) {
    UNUSED(p);
    BFApp* brainfuck = brainfuck_alloc();
    if(!brainfuck){ return 0; }

    Storage* storage = furi_record_open(RECORD_STORAGE);
    storage_simply_mkdir(storage, "/ext/brainfuck");

    scene_manager_next_scene(brainfuck->scene_manager, brainfuckSceneStart);

    view_dispatcher_run(brainfuck->view_dispatcher);

    brainfuck_free(brainfuck);

    return 0;
}