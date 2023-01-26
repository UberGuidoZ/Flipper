#include "../brainfuck_i.h"

void brainfuck_scene_dev_env_on_enter(void* context) {
    BFApp* app = context;
    view_dispatcher_switch_to_view(app->view_dispatcher, brainfuckViewDev);
}

bool brainfuck_scene_dev_env_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void brainfuck_scene_dev_env_on_exit(void* context) {
    UNUSED(context);
}
