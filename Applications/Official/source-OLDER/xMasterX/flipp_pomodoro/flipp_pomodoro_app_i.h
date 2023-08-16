#pragma once

#define FURI_DEBUG 1

/**
 * Index of dependencies for the main app
 */

// Platform Imports

#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view_stack.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/elements.h>
#include <dolphin/dolphin.h>
#include <input/input.h>

// App resource imports

#include "helpers/time.h"
#include "helpers/notifications.h"
#include "modules/flipp_pomodoro.h"
#include "flipp_pomodoro_app.h"
#include "scenes/flipp_pomodoro_scene.h"
#include "views/flipp_pomodoro_timer_view.h"

// Auto-compiled icons
#include "flipp_pomodoro_icons.h"
