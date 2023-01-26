#include "rgb_led.h"
#include <furi.h>
#include <furi_hal.h>
#include <notification/notification_messages.h>
#define TAG "RgbLedApp"

enum RgbLedSubmenuIndex {
    RgbLedSubmenuIndexDefault,
};

void rgb_led_submenu_callback(void* context, uint32_t index) {
    furi_assert(context);
    RgbLed* app = context;
    if(index == RgbLedSubmenuIndexDefault) {
        app->view_id = RgbLedViewDefault;
        view_dispatcher_switch_to_view(app->view_dispatcher, RgbLedViewDefault);
    } 
}

void rgb_led_dialog_callback(DialogExResult result, void* context) {
    furi_assert(context);
    RgbLed* app = context;
    if(result == DialogExResultLeft) {
        view_dispatcher_stop(app->view_dispatcher);
    } else if(result == DialogExResultRight) {
        view_dispatcher_switch_to_view(app->view_dispatcher, app->view_id); // Show last view
    } else if(result == DialogExResultCenter) {
        view_dispatcher_switch_to_view(app->view_dispatcher, RgbLedViewSubmenu);
    }
}

uint32_t rgb_led_exit_confirm_view(void* context) {
    UNUSED(context);
    return RgbLedViewExitConfirm;
}

uint32_t rgb_led_exit(void* context) {
    UNUSED(context);
    return VIEW_NONE;
}

RgbLed* rgb_led_app_alloc() {
    RgbLed* app = malloc(sizeof(RgbLed));

    // Gui
    app->gui = furi_record_open(RECORD_GUI);

    // Notifications
    app->notifications = furi_record_open(RECORD_NOTIFICATION);

    // View dispatcher
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    // Submenu view
    app->submenu = submenu_alloc();
    submenu_add_item(
        app->submenu, "Default", RgbLedSubmenuIndexDefault, rgb_led_submenu_callback, app);
    view_set_previous_callback(submenu_get_view(app->submenu), rgb_led_exit);
    view_dispatcher_add_view(app->view_dispatcher, RgbLedViewSubmenu, submenu_get_view(app->submenu));

    // Dialog view
    app->dialog = dialog_ex_alloc();
    dialog_ex_set_result_callback(app->dialog, rgb_led_dialog_callback);
    dialog_ex_set_context(app->dialog, app);
    dialog_ex_set_left_button_text(app->dialog, "Exit");
    dialog_ex_set_right_button_text(app->dialog, "Stay");
    dialog_ex_set_center_button_text(app->dialog, "Menu");
    dialog_ex_set_header(app->dialog, "Close Current App?", 16, 12, AlignLeft, AlignTop);
    view_dispatcher_add_view(
        app->view_dispatcher, RgbLedViewExitConfirm, dialog_ex_get_view(app->dialog));

    // Default view
    app->rgb_led_default = rgb_led_default_alloc();
    view_set_previous_callback(
        rgb_led_default_get_view(app->rgb_led_default), rgb_led_exit_confirm_view);
    view_dispatcher_add_view(
        app->view_dispatcher, RgbLedViewDefault, rgb_led_default_get_view(app->rgb_led_default));

    // TODO switch to menu after Media is done
    app->view_id = RgbLedViewSubmenu;
    view_dispatcher_switch_to_view(app->view_dispatcher, app->view_id);

    return app;
}

void rgb_led_app_free(RgbLed* app) {
    furi_assert(app);

    // Reset notification
    notification_internal_message(app->notifications, &sequence_reset_blue);

    // Free views
    view_dispatcher_remove_view(app->view_dispatcher, RgbLedViewSubmenu);
    submenu_free(app->submenu);
    view_dispatcher_remove_view(app->view_dispatcher, RgbLedViewExitConfirm);
    dialog_ex_free(app->dialog);
    view_dispatcher_remove_view(app->view_dispatcher, RgbLedViewDefault);
    rgb_led_default_free(app->rgb_led_default);
    view_dispatcher_free(app->view_dispatcher);
    // Close records
    furi_record_close(RECORD_GUI);
    app->gui = NULL;
    furi_record_close(RECORD_NOTIFICATION);
    app->notifications = NULL;

    // Free rest
    free(app);
}

int32_t rgb_led_app(void* p) {
    UNUSED(p);
    RgbLed* app = rgb_led_app_alloc();
    view_dispatcher_run(app->view_dispatcher);
    rgb_led_app_free(app);
    return 0;
}
