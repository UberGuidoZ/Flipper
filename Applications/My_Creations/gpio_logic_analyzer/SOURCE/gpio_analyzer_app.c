// #########################################################
// #  gpio_analyzer_app.c - entry point and app lifecycle  #
// #  By: UberGuidoZ | https://github.com/UberGuidoZ/      #
// #  v2.01                                                #
// #########################################################

#include "gpio_analyzer.h"
#include <string.h>

// ============================================================
// Scene handler tables
// The current Flipper SDK SceneManagerHandlers struct uses three
// separate arrays (on_enter, on_event, on_exit) rather than a
// single array of per-scene structs. Order must match GaSceneId.
// ============================================================
static void (*const ga_on_enter_handlers[])(void*) = {
    scene_splash_on_enter,
    scene_menu_on_enter,
    scene_analyzer_on_enter,
    scene_instructions_on_enter,
    scene_about_on_enter,
};

static bool (*const ga_on_event_handlers[])(void*, SceneManagerEvent) = {
    scene_splash_on_event,
    scene_menu_on_event,
    scene_analyzer_on_event,
    scene_instructions_on_event,
    scene_about_on_event,
};

static void (*const ga_on_exit_handlers[])(void*) = {
    scene_splash_on_exit,
    scene_menu_on_exit,
    scene_analyzer_on_exit,
    scene_instructions_on_exit,
    scene_about_on_exit,
};

static const SceneManagerHandlers ga_scene_manager_handlers = {
    .on_enter_handlers = ga_on_enter_handlers,
    .on_event_handlers = ga_on_event_handlers,
    .on_exit_handlers  = ga_on_exit_handlers,
    .scene_num         = GaSceneCount,
};

// ============================================================
// ViewDispatcher callbacks
// ============================================================

// Routes custom events (button/UI actions) to the active scene.
static bool ga_custom_event_cb(void* context, uint32_t event) {
    GpioAnalyzer* app = (GpioAnalyzer*)context;
    furi_check(app != NULL);
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

// Routes BACK presses to the active scene; scene manager pops
// the stack on unhandled backs. Returning false from the last
// scene causes ViewDispatcher to stop its run loop.
static bool ga_back_event_cb(void* context) {
    GpioAnalyzer* app = (GpioAnalyzer*)context;
    furi_check(app != NULL);
    return scene_manager_handle_back_event(app->scene_manager);
}

// ============================================================
// Allocate and initialise all app resources
// ============================================================
static GpioAnalyzer* ga_alloc(void) {
    GpioAnalyzer* app = malloc(sizeof(GpioAnalyzer));
    furi_check(app != NULL);
    memset(app, 0, sizeof(GpioAnalyzer));

    app->rate_index = GA_DEFAULT_RATE_IDX;

    // ----------------------------------------------------------
    // Scene manager
    // ----------------------------------------------------------
    app->scene_manager = scene_manager_alloc(&ga_scene_manager_handlers, app);
    furi_check(app->scene_manager != NULL);

    // ----------------------------------------------------------
    // View dispatcher
    // ----------------------------------------------------------
    app->view_dispatcher = view_dispatcher_alloc();
    furi_check(app->view_dispatcher != NULL);

    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, ga_custom_event_cb);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, ga_back_event_cb);

    // Attach to the GUI fullscreen layer.
    Gui* gui = furi_record_open(RECORD_GUI);
    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);
    furi_record_close(RECORD_GUI);

    // ----------------------------------------------------------
    // Submenu (main menu view)
    // ----------------------------------------------------------
    app->submenu = submenu_alloc();
    furi_check(app->submenu != NULL);
    view_dispatcher_add_view(
        app->view_dispatcher, GaViewMenu, submenu_get_view(app->submenu));

    // ----------------------------------------------------------
    // Custom views (each scene_*.c sets up draw/input callbacks)
    // ----------------------------------------------------------
    scene_splash_setup_view(app);
    view_dispatcher_add_view(app->view_dispatcher, GaViewSplash, app->view_splash);

    scene_analyzer_setup_view(app);
    view_dispatcher_add_view(app->view_dispatcher, GaViewAnalyzer, app->view_analyzer);

    scene_instructions_setup_view(app);
    view_dispatcher_add_view(app->view_dispatcher, GaViewInstructions, app->view_instructions);

    scene_about_setup_view(app);
    view_dispatcher_add_view(app->view_dispatcher, GaViewAbout, app->view_about);

    // ----------------------------------------------------------
    // Capture primitives (timer and mutex allocated once)
    // ----------------------------------------------------------
    app->mutex = furi_mutex_alloc(FuriMutexTypeNormal);
    furi_check(app->mutex != NULL);

    // Timer is started/stopped by the analyzer scene.
    // The timer callback is registered in scene_analyzer.c.
    // We store only the handle here; setup is in scene_analyzer.c.
    // (The timer is allocated there so the callback symbol stays local.)

    // ----------------------------------------------------------
    // Storage and CSV file object
    // ----------------------------------------------------------
    app->storage = furi_record_open(RECORD_STORAGE);
    furi_check(app->storage != NULL);

    app->csv_file = storage_file_alloc(app->storage);
    furi_check(app->csv_file != NULL);

    // Ensure the output directory exists on the SD card.
    // FSE_EXIST is a success condition (directory already present).
    FS_Error dir_err = storage_common_mkdir(app->storage, GA_CSV_DIR);
    if(dir_err != FSE_OK && dir_err != FSE_EXIST) {
        // Non-fatal: CSV open will simply fail later if the dir is absent.
    }

    // ----------------------------------------------------------
    // Notifications
    // ----------------------------------------------------------
    app->notifications = furi_record_open(RECORD_NOTIFICATION);
    furi_check(app->notifications != NULL);

    return app;
}

// ============================================================
// Free all resources in reverse-allocation order
// ============================================================
static void ga_free(GpioAnalyzer* app) {
    furi_check(app != NULL);

    // Timer was allocated in scene_analyzer.c and must be freed
    // before the mutex it may reference.
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

    if(app->csv_file != NULL) {
        if(app->csv_open) {
            storage_file_close(app->csv_file);
            app->csv_open = false;
        }
        storage_file_free(app->csv_file);
        app->csv_file = NULL;
    }
    furi_record_close(RECORD_STORAGE);

    furi_mutex_free(app->mutex);

    // Remove views before freeing view dispatcher.
    view_dispatcher_remove_view(app->view_dispatcher, GaViewAbout);
    view_dispatcher_remove_view(app->view_dispatcher, GaViewInstructions);
    view_dispatcher_remove_view(app->view_dispatcher, GaViewAnalyzer);
    view_dispatcher_remove_view(app->view_dispatcher, GaViewMenu);
    view_dispatcher_remove_view(app->view_dispatcher, GaViewSplash);

    view_free(app->view_about);
    view_free(app->view_instructions);
    view_free(app->view_analyzer);
    view_free(app->view_splash);
    submenu_free(app->submenu);

    view_dispatcher_free(app->view_dispatcher);
    scene_manager_free(app->scene_manager);

    free(app);
}

// ============================================================
// Application entry point
// ============================================================
int32_t gpio_analyzer_app(void* p) {
    UNUSED(p);

    GpioAnalyzer* app = ga_alloc();

    // Push the main menu scene; ViewDispatcher run() blocks until
    // view_dispatcher_stop() is called (by the back handler on the
    // root menu scene returning false).
    scene_manager_next_scene(app->scene_manager, GaSceneSplash);
    view_dispatcher_run(app->view_dispatcher);

    ga_free(app);
    return 0;
}