#include "../brainfuck_i.h"

void brainfuck_scene_file_select_on_enter(void* context) {
    BFApp* app = context;

    DialogsApp* dialogs = furi_record_open("dialogs");
    FuriString* path;
    path = furi_string_alloc();
    furi_string_set(path, "/ext/brainfuck");

    DialogsFileBrowserOptions browser_options;
    dialog_file_browser_set_basic_options(&browser_options, ".b", &I_bfico);
    browser_options.base_path = "/ext/brainfuck";
    browser_options.hide_ext = false;

    bool selected = dialog_file_browser_show(dialogs, path, path, &browser_options);

    if(selected){
        furi_string_set(app->BF_file_path, path);
        scene_manager_next_scene(app->scene_manager, brainfuckSceneDevEnv);
    }
    else{
        scene_manager_search_and_switch_to_previous_scene(app->scene_manager, brainfuckSceneStart);
    }
}

bool brainfuck_scene_file_select_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void brainfuck_scene_file_select_on_exit(void* context) {
    UNUSED(context);
}
