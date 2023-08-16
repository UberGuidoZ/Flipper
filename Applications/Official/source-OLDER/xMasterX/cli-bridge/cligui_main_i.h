#pragma once

#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_version.h>
#include <furi_hal_usb_cdc.h>
#include <furi_hal_usb.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/text_box.h>
#include <gui/modules/text_input.h>
#include <m-dict.h>
#include <loader/loader.h>
#include "internal_defs.h"

#define TEXT_BOX_STORE_SIZE (4096)
#define TEXT_INPUT_STORE_SIZE (512)

typedef enum {
    ViewTextInput,
    ViewConsoleOutput,
} CliguiState;

typedef struct {
    CliguiState state;
    struct {
        FuriStreamBuffer* app_tx;
        FuriStreamBuffer* app_rx;
    } streams;
} CliguiData;

typedef struct {
    CliguiData* data;
    Gui* gui;
    TextBox* text_box;
    FuriString* text_box_store;
    char text_input_store[TEXT_INPUT_STORE_SIZE + 1];
    TextInput* text_input;
    ViewDispatcher* view_dispatcher;
    ViewDispatcher_internal* view_dispatcher_i;
} CliguiApp;