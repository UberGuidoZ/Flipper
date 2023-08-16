#pragma once

#include <furi.h>
#include <gui/gui.h>
#include <gui/view.h>
#include <gui/view_dispatcher.h>
#include <notification/notification.h>

#include <gui/modules/submenu.h>
#include <gui/modules/dialog_ex.h>
#include "views/rgb_led_default.h"

typedef struct {
    Gui* gui;
    NotificationApp* notifications;
    ViewDispatcher* view_dispatcher;
    Submenu* submenu;
    DialogEx* dialog;
    RgbLedDefault* rgb_led_default;
    uint32_t view_id;
} RgbLed;

typedef enum {
    RgbLedViewSubmenu,
    RgbLedViewDefault,
    RgbLedViewExitConfirm,
} RgbLedViewView;
