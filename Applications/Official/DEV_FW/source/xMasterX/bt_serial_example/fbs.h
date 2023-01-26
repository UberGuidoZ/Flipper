#pragma once

#include <furi.h>
#include <furi_hal_bt.h>
#include <furi_hal_bt_serial.h>
#include <bt/bt_service/bt.h>
#include <gui/gui.h>
#include <gui/view.h>

#define TAG "FlipperBTSerial"

typedef struct {
    Bt* bt;
    bool bt_connected;

    char* display_text;

    ViewPort* view_port;
    Gui* gui;

    FuriMutex* app_mutex;
    FuriMessageQueue* event_queue;
} FBS;