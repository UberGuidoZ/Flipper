#pragma once

// ###########################################################
// #  gpio_analyzer.h -GPIO Logic Analyzer for Flipper Zero  #
// #  By: UberGuidoZ | https://github.com/UberGuidoZ/        #
// #  v2.01                                                  #
// ###########################################################

#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_gpio.h>
#include <datetime/datetime.h>
#include <gui/gui.h>
#include <gui/view.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/submenu.h>
#include <input/input.h>
#include <notification/notification_messages.h>
#include <storage/storage.h>

// ------------------------------------------------------------
// Screen geometry (actual OLED pixels)
// ------------------------------------------------------------
#define GA_SCREEN_W         128U
#define GA_SCREEN_H          64U
#define GA_HEADER_H           9U   // 8px text row + 1px separator
#define GA_ROW_H              7U   // per-channel row height
#define GA_LABEL_X            1U   // left edge of pin label column
#define GA_WAVE_X            15U   // left edge of waveform column
#define GA_WAVE_W           104U   // waveform pixel width (= sample history depth)
#define GA_STATE_X          120U   // left edge of H/L state column
#define GA_WAVE_Y_HIGH        1U   // row-relative y of HIGH sample dot
#define GA_WAVE_Y_LOW         5U   // row-relative y of LOW  sample dot

// Number of GPIO channels displayed
#define GA_PIN_COUNT          8U

// History buffer depth must equal waveform width for 1:1 pixel mapping.
// A _Static_assert in scene_analyzer.c enforces this at compile time.
#define GA_HISTORY_SIZE     GA_WAVE_W

// ------------------------------------------------------------
// Sample rate table sizing and default index
// ------------------------------------------------------------
#define GA_RATE_COUNT         5U
#define GA_DEFAULT_RATE_IDX   2U   // 100 Hz

// ------------------------------------------------------------
// CSV output
// ------------------------------------------------------------
#define GA_CSV_DIR          "/ext/gpio_analyzer"
#define GA_CSV_PATH_LEN      64U

// ------------------------------------------------------------
// Instructions
// ------------------------------------------------------------
#define GA_INSTR_PAGES        4U   // total pages in the instructions view

// ------------------------------------------------------------
// Scene IDs (order must match ga_scene_handlers[] in
// gpio_analyzer_app.c)
// ------------------------------------------------------------
typedef enum {
    GaSceneSplash = 0,
    GaSceneMenu,
    GaSceneAnalyzer,
    GaSceneInstructions,
    GaSceneAbout,
    GaSceneCount,
} GaSceneId;

// ------------------------------------------------------------
// View IDs (registered with ViewDispatcher)
// ------------------------------------------------------------
typedef enum {
    GaViewSplash = 0,
    GaViewMenu,
    GaViewAnalyzer,
    GaViewInstructions,
    GaViewAbout,
    GaViewCount,
} GaViewId;

// ------------------------------------------------------------
// Custom events sent via view_dispatcher_send_custom_event()
// ------------------------------------------------------------
typedef enum {
    GaEventMenuStart        = 0,
    GaEventMenuInstructions = 1,
    GaEventMenuAbout        = 2,
} GaMenuEvent;

typedef enum {
    GaEventAnalyzerToggle       = 0,
    GaEventAnalyzerClearHistory = 1,
    GaEventAnalyzerRateUp       = 2,
    GaEventAnalyzerRateDown     = 3,
} GaAnalyzerEvent;

typedef enum {
    GaEventInstrPageLeft  = 0,
    GaEventInstrPageRight = 1,
} GaInstrEvent;

// ------------------------------------------------------------
// Per-channel capture state
// ------------------------------------------------------------
typedef struct {
    bool     current_state;
    uint8_t  history[GA_HISTORY_SIZE];
    uint32_t history_head;
    uint32_t sample_count;
} GaChannel;

// ------------------------------------------------------------
// Forward declaration - allows view model structs below to
// hold a GpioAnalyzer* before the full struct is defined.
// ------------------------------------------------------------
typedef struct GpioAnalyzer_s GpioAnalyzer;

// ------------------------------------------------------------
// View models
// ------------------------------------------------------------

// Analyzer view: the draw callback reads channel data through app.
typedef struct {
    GpioAnalyzer* app;
} GaAnalyzerModel;

// Instructions view: tracks which page is displayed.
typedef struct {
    uint8_t page;
} GaInstrModel;

// ------------------------------------------------------------
// Full application state struct
// ------------------------------------------------------------
struct GpioAnalyzer_s {
    // Scene / view management
    SceneManager*    scene_manager;
    ViewDispatcher*  view_dispatcher;
    Submenu*         submenu;
    View*            view_splash;
    View*            view_analyzer;
    View*            view_instructions;
    View*            view_about;

    // GPIO capture
    FuriMutex*       mutex;
    FuriTimer*       sample_timer;
    FuriTimer*       splash_timer;     // one-shot auto-advance timer
    GaChannel        channels[GA_PIN_COUNT];
    bool             running;
    uint8_t          rate_index;
    uint32_t         sample_number;

    // CSV logging
    Storage*         storage;
    File*            csv_file;
    bool             csv_open;
    char             csv_path[GA_CSV_PATH_LEN];

    // Notifications
    NotificationApp* notifications;
};

// ------------------------------------------------------------
// Scene handler declarations (implemented in each scene_*.c)
// ------------------------------------------------------------
void scene_splash_on_enter(void* context);
bool scene_splash_on_event(void* context, SceneManagerEvent event);
void scene_splash_on_exit(void* context);

void scene_menu_on_enter(void* context);
bool scene_menu_on_event(void* context, SceneManagerEvent event);
void scene_menu_on_exit(void* context);

void scene_analyzer_on_enter(void* context);
bool scene_analyzer_on_event(void* context, SceneManagerEvent event);
void scene_analyzer_on_exit(void* context);

void scene_instructions_on_enter(void* context);
bool scene_instructions_on_event(void* context, SceneManagerEvent event);
void scene_instructions_on_exit(void* context);

void scene_about_on_enter(void* context);
bool scene_about_on_event(void* context, SceneManagerEvent event);
void scene_about_on_exit(void* context);

// ------------------------------------------------------------
// View setup functions (called once from ga_alloc)
// ------------------------------------------------------------
void scene_splash_setup_view(GpioAnalyzer* app);
void scene_analyzer_setup_view(GpioAnalyzer* app);
void scene_instructions_setup_view(GpioAnalyzer* app);
void scene_about_setup_view(GpioAnalyzer* app);