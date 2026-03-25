// ####################################################
// #  scene_menu.c - main menu scene                  #
// #  By: UberGuidoZ | https://github.com/UberGuidoZ/ #
// #  v1.11                                           #
// ####################################################

#include "smart_meter.h"

static void sm_menu_cb(void* context, uint32_t index) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void scene_menu_on_enter(void* context) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);

    submenu_reset(app->submenu_menu);
    submenu_set_header(app->submenu_menu, "Smart Meter Monitor");

    submenu_add_item(app->submenu_menu, "Start Monitor",  SmEventMenuStart, sm_menu_cb, app);
    submenu_add_item(app->submenu_menu, "Instructions",   SmEventMenuInstr, sm_menu_cb, app);
    submenu_add_item(app->submenu_menu, "About",          SmEventMenuAbout, sm_menu_cb, app);

    view_dispatcher_switch_to_view(app->view_dispatcher, SmViewMenu);
}

bool scene_menu_on_event(void* context, SceneManagerEvent event) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);

    // BACK on the root menu exits the app entirely.
    if(event.type == SceneManagerEventTypeBack) {
        view_dispatcher_stop(app->view_dispatcher);
        return true;
    }

    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
        case SmEventMenuStart:
            scene_manager_next_scene(app->scene_manager, SmSceneMonitor);
            return true;
        case SmEventMenuInstr:
            scene_manager_next_scene(app->scene_manager, SmSceneInstructions);
            return true;
        case SmEventMenuAbout:
            scene_manager_next_scene(app->scene_manager, SmSceneAbout);
            return true;
        default:
            break;
        }
    }
    return false;
}

void scene_menu_on_exit(void* context) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);
    submenu_reset(app->submenu_menu);
}