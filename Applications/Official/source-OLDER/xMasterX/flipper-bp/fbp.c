#include "fbp.h"

enum FBPSubmenuIndex {
    FBPSubmenuIndexInternal,
    FBPSubmenuIndexGPIOSimpleMotor,
};

uint32_t fbp_start_view(void* context) {
    UNUSED(context);
    return FBPAppViewSubmenu;
}

uint32_t fbp_exit(void* context) {
    UNUSED(context);
    return VIEW_NONE;
}

void fbp_submenu_callback(void* context, uint32_t index) {
    furi_assert(context);
    FBP* app = context;
    if(index == FBPSubmenuIndexInternal) {
        view_dispatcher_switch_to_view(app->view_dispatcher, FBPAppViewInternal);
    } else if (index == FBPSubmenuIndexGPIOSimpleMotor) {
        view_dispatcher_switch_to_view(app->view_dispatcher, FBPAppViewGPIOSimpleMotor);
    }
}

FBP* fbp_alloc() {
    FBP* app = malloc(sizeof(FBP));
    app->app_mutex = furi_mutex_alloc(FuriMutexTypeNormal);
    app->event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    app->bt_connected = false;
    app->bt = furi_record_open(RECORD_BT);

    app->gui = furi_record_open(RECORD_GUI);
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    // set submenu
    app->submenu = submenu_alloc();
    view_set_previous_callback(submenu_get_view(app->submenu), fbp_exit);
    view_dispatcher_add_view(app->view_dispatcher, FBPAppViewSubmenu, submenu_get_view(app->submenu));

    // add Flipper Internal View
    app->flipper_vibrator = flipper_vibrator_alloc(app);
    submenu_add_item(app->submenu, "Flipper Internal Vibrator", FBPSubmenuIndexInternal, fbp_submenu_callback, app);
    view_set_previous_callback(flipper_vibrator_get_view(app->flipper_vibrator), fbp_start_view);
    view_dispatcher_add_view(app->view_dispatcher, FBPAppViewInternal, flipper_vibrator_get_view(app->flipper_vibrator));

    // add GPIO Simple Motor View
    app->gpio_simple_motor = gpio_simple_motor_alloc(app);
    submenu_add_item(app->submenu, "Flipper GPIO Simple Motor", FBPSubmenuIndexGPIOSimpleMotor, fbp_submenu_callback, app);
    view_set_previous_callback(gpio_simple_motor_get_view(app->gpio_simple_motor), fbp_start_view);
    view_dispatcher_add_view(app->view_dispatcher, FBPAppViewGPIOSimpleMotor, gpio_simple_motor_get_view(app->gpio_simple_motor));

    view_dispatcher_switch_to_view(app->view_dispatcher, FBPAppViewSubmenu);
    return app;
}

void fbs_free(FBP* app) {
    furi_assert(app);

    // Free views
    view_dispatcher_remove_view(app->view_dispatcher, FBPAppViewSubmenu);
    submenu_free(app->submenu);

    // free Flipper Internal Vibrator
    view_dispatcher_remove_view(app->view_dispatcher, FBPAppViewInternal);
    flipper_vibrator_free(app->flipper_vibrator);

    // free GPIO Simple Motor
    view_dispatcher_remove_view(app->view_dispatcher, FBPAppViewGPIOSimpleMotor);
    gpio_simple_motor_free(app->gpio_simple_motor);

    // Other deallocations
    view_dispatcher_free(app->view_dispatcher);


    furi_record_close(RECORD_GUI);
    app->gui = NULL;

    furi_mutex_free(app->app_mutex);
    furi_message_queue_free(app->event_queue);

    furi_record_close(RECORD_BT);
    app->bt = NULL;

    free(app);
}

int32_t fbp_app(void* p) {
    UNUSED(p);
    FBP* app = fbp_alloc();

    view_dispatcher_run(app->view_dispatcher);

    furi_hal_bt_serial_set_event_callback(0, NULL, NULL);

    fbs_free(app);
    return 0;
}