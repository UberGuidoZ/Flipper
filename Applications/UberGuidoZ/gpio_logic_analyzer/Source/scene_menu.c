// #####################################################
// #  scene_menu.c - main menu scene                   #
// #  By: UberGuidoZ | https://github.com/UberGuidoZ/  #
// #  v2.01                                            #
// #####################################################

#include "gpio_analyzer.h"

// ============================================================
// Submenu callback - sends a custom event to the scene manager
// ============================================================
static void ga_menu_submenu_cb(void* context, uint32_t index) {
    GpioAnalyzer* app = (GpioAnalyzer*)context;
    furi_check(app != NULL);
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

// ============================================================
// Scene handlers
// ============================================================

void scene_menu_on_enter(void* context) {
    GpioAnalyzer* app = (GpioAnalyzer*)context;
    furi_check(app != NULL);

    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "GPIO Logic Analyzer");

    submenu_add_item(
        app->submenu,
        "Start Analyzer",
        GaEventMenuStart,
        ga_menu_submenu_cb,
        app);
    submenu_add_item(
        app->submenu,
        "Instructions",
        GaEventMenuInstructions,
        ga_menu_submenu_cb,
        app);
    submenu_add_item(
        app->submenu,
        "About",
        GaEventMenuAbout,
        ga_menu_submenu_cb,
        app);

    view_dispatcher_switch_to_view(app->view_dispatcher, GaViewMenu);
}

bool scene_menu_on_event(void* context, SceneManagerEvent event) {
    GpioAnalyzer* app = (GpioAnalyzer*)context;
    furi_check(app != NULL);
    bool consumed = false;

    // BACK on the root menu exits the app entirely.
    // Without this, scene manager would pop the menu and reveal
    // the splash scene underneath, trapping the user.
    if(event.type == SceneManagerEventTypeBack) {
        view_dispatcher_stop(app->view_dispatcher);
        return true;
    }

    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
        case GaEventMenuStart:
            scene_manager_next_scene(app->scene_manager, GaSceneAnalyzer);
            consumed = true;
            break;
        case GaEventMenuInstructions:
            scene_manager_next_scene(app->scene_manager, GaSceneInstructions);
            consumed = true;
            break;
        case GaEventMenuAbout:
            scene_manager_next_scene(app->scene_manager, GaSceneAbout);
            consumed = true;
            break;
        default:
            break;
        }
    }

    return consumed;
}

void scene_menu_on_exit(void* context) {
    GpioAnalyzer* app = (GpioAnalyzer*)context;
    furi_check(app != NULL);
    submenu_reset(app->submenu);
}