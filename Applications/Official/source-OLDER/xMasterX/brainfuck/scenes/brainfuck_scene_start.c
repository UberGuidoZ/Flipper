#include "../brainfuck_i.h"
enum SubmenuIndex {
    SubmenuIndexNew,
    SubmenuIndexOpen,
    SubmenuIndexAbout,
};

void brainfuck_scene_start_submenu_callback(void* context, uint32_t index) {
    BFApp* brainfuck = context;
    view_dispatcher_send_custom_event(brainfuck->view_dispatcher, index);
}
void brainfuck_scene_start_on_enter(void* context) {
    BFApp* brainfuck = context;

    Submenu* submenu = brainfuck->submenu;
    submenu_add_item(submenu, "New", SubmenuIndexNew, brainfuck_scene_start_submenu_callback, brainfuck);
    submenu_add_item(submenu, "Open", SubmenuIndexOpen, brainfuck_scene_start_submenu_callback, brainfuck);
    submenu_add_item(submenu, "About", SubmenuIndexAbout, brainfuck_scene_start_submenu_callback, brainfuck);

    submenu_set_selected_item(submenu, scene_manager_get_scene_state(brainfuck->scene_manager, brainfuckSceneStart));
    view_dispatcher_switch_to_view(brainfuck->view_dispatcher, brainfuckViewMenu);
}

bool brainfuck_scene_start_on_event(void* context, SceneManagerEvent event) {
    BFApp* brainfuck = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SubmenuIndexNew) {
            scene_manager_next_scene(brainfuck->scene_manager, brainfuckSceneFileCreate);
            consumed = true;
        } else if(event.event == SubmenuIndexOpen) {
            scene_manager_next_scene(brainfuck->scene_manager, brainfuckSceneFileSelect);
            consumed = true;
        } else if(event.event == SubmenuIndexAbout) {
            text_box_set_text(brainfuck->text_box, "FlipperBrainfuck\n\nAn F0 brainfuck intepretor\nBy github.com/Nymda");
            scene_manager_next_scene(brainfuck->scene_manager, brainfuckSceneExecEnv);
            consumed = true;
        }
        scene_manager_set_scene_state(brainfuck->scene_manager, brainfuckSceneStart, event.event);
    }

    return consumed;
}

void brainfuck_scene_start_on_exit(void* context) {
    BFApp* brainfuck = context;
    submenu_reset(brainfuck->submenu);
}
