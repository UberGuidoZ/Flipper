// ######################################################
// #  smart_meter_app.c - entry point and app lifecycle #
// #  By: UberGuidoZ | https://github.com/UberGuidoZ/   #
// #  v1.11                                             #
// ######################################################

#include "smart_meter.h"
#include <string.h>

// ============================================================
// Scene handler tables
// Order must match SmSceneId enum in smart_meter.h.
// ============================================================
static void (*const sm_on_enter_handlers[])(void*) = {
    scene_splash_on_enter,
    scene_menu_on_enter,
    scene_freq_on_enter,
    scene_monitor_on_enter,
    scene_instructions_on_enter,
    scene_about_on_enter,
};

static bool (*const sm_on_event_handlers[])(void*, SceneManagerEvent) = {
    scene_splash_on_event,
    scene_menu_on_event,
    scene_freq_on_event,
    scene_monitor_on_event,
    scene_instructions_on_event,
    scene_about_on_event,
};

static void (*const sm_on_exit_handlers[])(void*) = {
    scene_splash_on_exit,
    scene_menu_on_exit,
    scene_freq_on_exit,
    scene_monitor_on_exit,
    scene_instructions_on_exit,
    scene_about_on_exit,
};

static const SceneManagerHandlers sm_scene_manager_handlers = {
    .on_enter_handlers = sm_on_enter_handlers,
    .on_event_handlers = sm_on_event_handlers,
    .on_exit_handlers  = sm_on_exit_handlers,
    .scene_num         = SmSceneCount,
};

// ============================================================
// ViewDispatcher callbacks
// ============================================================

static bool sm_custom_event_cb(void* context, uint32_t event) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

static bool sm_back_event_cb(void* context) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);
    return scene_manager_handle_back_event(app->scene_manager);
}

// ============================================================
// Allocate all app resources
// ============================================================
static SmartMeterApp* sm_alloc(void) {
    SmartMeterApp* app = malloc(sizeof(SmartMeterApp));
    furi_check(app != NULL);
    memset(app, 0, sizeof(SmartMeterApp));

    app->freq_index    = SM_DEFAULT_FREQ;
    app->rate_index    = SM_DEFAULT_RATE;
    app->threshold_dbm = (int8_t)SM_THRESHOLD_DEFAULT;
    app->monitoring    = true;

    // ----------------------------------------------------------
    // Scene manager
    // ----------------------------------------------------------
    app->scene_manager = scene_manager_alloc(&sm_scene_manager_handlers, app);
    furi_check(app->scene_manager != NULL);

    // ----------------------------------------------------------
    // View dispatcher
    // ----------------------------------------------------------
    app->view_dispatcher = view_dispatcher_alloc();
    furi_check(app->view_dispatcher != NULL);

    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, sm_custom_event_cb);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, sm_back_event_cb);

    Gui* gui = furi_record_open(RECORD_GUI);
    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);
    furi_record_close(RECORD_GUI);

    // ----------------------------------------------------------
    // Submenus
    // ----------------------------------------------------------
    app->submenu_menu = submenu_alloc();
    furi_check(app->submenu_menu != NULL);
    view_dispatcher_add_view(
        app->view_dispatcher, SmViewMenu, submenu_get_view(app->submenu_menu));

    app->submenu_freq = submenu_alloc();
    furi_check(app->submenu_freq != NULL);
    view_dispatcher_add_view(
        app->view_dispatcher, SmViewFreq, submenu_get_view(app->submenu_freq));

    // ----------------------------------------------------------
    // Custom views
    // ----------------------------------------------------------
    scene_splash_setup_view(app);
    view_dispatcher_add_view(app->view_dispatcher, SmViewSplash, app->view_splash);

    scene_monitor_setup_view(app);
    view_dispatcher_add_view(app->view_dispatcher, SmViewMonitor, app->view_monitor);

    scene_instructions_setup_view(app);
    view_dispatcher_add_view(app->view_dispatcher, SmViewInstructions, app->view_instructions);

    scene_about_setup_view(app);
    view_dispatcher_add_view(app->view_dispatcher, SmViewAbout, app->view_about);

    // ----------------------------------------------------------
    // Mutex and notifications
    // ----------------------------------------------------------
    app->mutex = furi_mutex_alloc(FuriMutexTypeNormal);
    furi_check(app->mutex != NULL);

    app->notifications = furi_record_open(RECORD_NOTIFICATION);
    furi_check(app->notifications != NULL);

    return app;
}

// ============================================================
// Free all resources in reverse-allocation order
// ============================================================
static void sm_free(SmartMeterApp* app) {
    furi_check(app != NULL);

    if(app->sample_timer != NULL) {
        furi_timer_stop(app->sample_timer);
        furi_timer_free(app->sample_timer);
        app->sample_timer = NULL;
    }

    if(app->splash_timer != NULL) {
        furi_timer_stop(app->splash_timer);
        furi_timer_free(app->splash_timer);
        app->splash_timer = NULL;
    }

    furi_record_close(RECORD_NOTIFICATION);
    furi_mutex_free(app->mutex);

    view_dispatcher_remove_view(app->view_dispatcher, SmViewAbout);
    view_dispatcher_remove_view(app->view_dispatcher, SmViewInstructions);
    view_dispatcher_remove_view(app->view_dispatcher, SmViewMonitor);
    view_dispatcher_remove_view(app->view_dispatcher, SmViewSplash);
    view_dispatcher_remove_view(app->view_dispatcher, SmViewFreq);
    view_dispatcher_remove_view(app->view_dispatcher, SmViewMenu);

    view_free(app->view_about);
    view_free(app->view_instructions);
    view_free(app->view_monitor);
    view_free(app->view_splash);
    submenu_free(app->submenu_freq);
    submenu_free(app->submenu_menu);

    view_dispatcher_free(app->view_dispatcher);
    scene_manager_free(app->scene_manager);

    free(app);
}

// ============================================================
// Application entry point
// ============================================================
int32_t smart_meter_app(void* p) {
    UNUSED(p);

    SmartMeterApp* app = sm_alloc();
    scene_manager_next_scene(app->scene_manager, SmSceneSplash);
    view_dispatcher_run(app->view_dispatcher);
    sm_free(app);
    return 0;
}