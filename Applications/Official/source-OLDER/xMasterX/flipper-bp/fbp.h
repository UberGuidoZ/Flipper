#pragma once

#include <furi.h>
#include <furi_hal_bt.h>
#include <furi_hal_bt_serial.h>
#include <bt/bt_service/bt.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/submenu.h>

#include "tcode.h"
#include "views/internal.h"
#include "views/gpio_simple_motor.h"

#define TAG "Flipper BP"

typedef struct FBP FBP;

struct FBP {
    Submenu* submenu;
    ViewDispatcher* view_dispatcher;
    Gui* gui;

    Bt* bt;
    bool bt_connected;

    FuriMutex* app_mutex;
    FuriMessageQueue* event_queue;

    FlipperVibrator* flipper_vibrator;
    GPIOSimpleMotor* gpio_simple_motor;
};

typedef enum {
    FBPAppViewSubmenu,
    FBPAppViewInternal,
    FBPAppViewGPIOSimpleMotor,
} FBPAppView;