#pragma once

#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <notification/notification_messages.h>
#include "views/flipp_pomodoro_timer_view.h"

#include "modules/flipp_pomodoro.h"

typedef enum
{
    // Reserve first 100 events for button types and indexes, starting from 0
    FlippPomodoroAppCustomEventStageSkip = 100,
    FlippPomodoroAppCustomEventStageComplete, // By Expiration
    FlippPomodoroAppCustomEventTimerTick,
} FlippPomodoroAppCustomEvent;

typedef struct
{
    SceneManager *scene_manager;
    ViewDispatcher *view_dispatcher;
    Gui *gui;
    NotificationApp *notification_app;
    FlippPomodoroTimerView *timer_view;
    FlippPomodoroState *state;
} FlippPomodoroApp;

typedef enum
{
    FlippPomodoroAppViewTimer,
} FlippPomodoroAppView;