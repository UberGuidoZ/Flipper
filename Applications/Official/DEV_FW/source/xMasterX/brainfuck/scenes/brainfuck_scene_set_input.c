#include "../brainfuck_i.h"

void set_input_text_input_callback(void* context) {
    BFApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, brainfuckCustomEventTextInputDone);
}

void brainfuck_scene_set_input_on_enter(void* context) {
    BFApp* app = context;
    TextInput* text_input = app->text_input;

    text_input_set_header_text(text_input, "Edit input buffer");
    text_input_set_result_callback(
        text_input,
        set_input_text_input_callback,
        app,
        app->inputBuffer,
        64,
        true);

    view_dispatcher_switch_to_view(app->view_dispatcher, brainfuckViewTextInput);
}

bool brainfuck_scene_set_input_on_event(void* context, SceneManagerEvent event) {
    BFApp* app = context;
    
    bool consumed = false;
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == brainfuckCustomEventTextInputDone) {
            scene_manager_search_and_switch_to_previous_scene(app->scene_manager, brainfuckSceneDevEnv);
        }
    }
    return consumed;
}

void brainfuck_scene_set_input_on_exit(void* context) {
    BFApp* app = context;
    scene_manager_search_and_switch_to_previous_scene(app->scene_manager, brainfuckSceneDevEnv);
}
