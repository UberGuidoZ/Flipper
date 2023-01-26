#pragma once

typedef struct BFDevEnv BFDevEnv;
typedef struct BFExecEnv BFExecEnv;
typedef unsigned char byte;

#include "brainfuck.h"
#include "worker.h"

#include <furi.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <notification/notification_messages.h>

#include <gui/modules/submenu.h>
#include <gui/modules/popup.h>
#include <gui/modules/loading.h>
#include <gui/modules/text_input.h>
#include <gui/modules/widget.h>
#include <gui/modules/text_box.h>

#include <dialogs/dialogs.h>
#include <input/input.h>

#include "scenes/brainfuck_scene.h"

#include "views/bf_dev_env.h"

#include <storage/storage.h>
#include <lib/toolbox/path.h>
#include <Brainfuck_icons.h>

#include <storage/storage.h>
#include <stream/stream.h>
#include <stream/buffered_file_stream.h>
#include <toolbox/stream/file_stream.h>

#define BF_INST_BUFFER_SIZE 2048
#define BF_OUTPUT_SIZE 512
#define BF_STACK_INITIAL_SIZE 128
#define BF_INPUT_BUFFER_SIZE 64
#define BF_STACK_STEP_SIZE 32

enum brainfuckCustomEvent {
    // Reserve first 100 events for button types and indexes, starting from 0
    brainfuckCustomEventReserved = 100,

    brainfuckCustomEventViewExit,
    brainfuckCustomEventWorkerExit,
    brainfuckCustomEventByteInputDone,
    brainfuckCustomEventTextInputDone,
};

typedef enum {
    EventTypeTick,
    EventTypeKey,
} EventType;

struct BFApp {
    ViewDispatcher* view_dispatcher;
    Gui* gui;
    NotificationApp* notifications;
    SceneManager* scene_manager;
    Submenu* submenu;
    Popup* popup;
    TextInput* text_input;
    TextBox* text_box;
    FuriString* text_box_store;
    FuriString* BF_file_path;
    BFDevEnv* BF_dev_env;
    int dataSize;
    char dataBuffer[BF_INST_BUFFER_SIZE];
    char inputBuffer[BF_INPUT_BUFFER_SIZE];
};

typedef enum {
    brainfuckViewMenu,
    brainfuckViewPopup,
    brainfuckViewLoading,
    brainfuckViewTextInput,
    brainfuckViewTextBox,
    brainfuckViewWidget,
    brainfuckViewDev,
    brainfuckViewExec,
} brainfuckView;
