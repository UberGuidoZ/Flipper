#pragma once

// #########################################################
// #  smart_meter.h - Smart Meter Monitor for Flipper Zero #
// #  By: UberGuidoZ | https://github.com/UberGuidoZ/      #
// #  v1.11                                                #
// #########################################################

#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_subghz.h>
#include <datetime/datetime.h>
#include <gui/gui.h>
#include <gui/view.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/submenu.h>
#include <input/input.h>
#include <notification/notification_messages.h>

// ------------------------------------------------------------
// Screen geometry (actual OLED pixels)
// ------------------------------------------------------------
#define SM_SCREEN_W       128U
#define SM_SCREEN_H        64U
#define SM_HEADER_H         9U   // 8px text row + 1px separator

// Waveform area
#define SM_WAVE_W         128U   // full screen width = sample history
#define SM_WAVE_H          28U   // 28px tall
#define SM_WAVE_Y_TOP      10U   // top pixel of waveform
#define SM_WAVE_Y_BOT      37U   // bottom pixel (SM_WAVE_Y_TOP + SM_WAVE_H - 1)

// Stats rows (FontKeyboard, 8px tall, 2px gap above each)
#define SM_STATS1_Y        48U   // baseline; glyph occupies y=40..48
#define SM_STATS2_Y        59U   // baseline; glyph occupies y=51..59

// RSSI scaling
#define SM_RSSI_MIN_DBM  (-120)  // mapped to 0px waveform height
#define SM_RSSI_MAX_DBM  (-40)   // mapped to SM_WAVE_H px height
#define SM_RSSI_RANGE      80    // SM_RSSI_MAX_DBM - SM_RSSI_MIN_DBM

// History buffer = waveform width (1 sample per pixel column)
#define SM_HISTORY_SIZE   SM_WAVE_W

// ------------------------------------------------------------
// Threshold
// ------------------------------------------------------------
#define SM_THRESHOLD_DEFAULT  (-90)
#define SM_THRESHOLD_MIN      (-110)
#define SM_THRESHOLD_MAX      (-60)
#define SM_THRESHOLD_STEP       5

// ------------------------------------------------------------
// Burst detection hysteresis
// ------------------------------------------------------------
#define SM_BURST_ENTER_SAMPLES  2U  // consecutive above-threshold to enter burst
#define SM_BURST_EXIT_SAMPLES   3U  // consecutive below-threshold to exit burst

// ------------------------------------------------------------
// Sample rates
// ------------------------------------------------------------
#define SM_RATE_COUNT     5U
#define SM_DEFAULT_RATE   2U   // 500 ms

// ------------------------------------------------------------
// Frequencies
// ------------------------------------------------------------
#define SM_FREQ_COUNT     3U
#define SM_DEFAULT_FREQ   2U   // 915 MHz (most common in US)

// ------------------------------------------------------------
// Splash
// ------------------------------------------------------------
#define SM_SPLASH_DELAY_MS   3000U
#define SM_SPLASH_TIMER_DONE    0U

// ------------------------------------------------------------
// Instructions
// ------------------------------------------------------------
#define SM_INSTR_PAGES        4U
#define SM_INSTR_BODY_LINES   5U

// ------------------------------------------------------------
// Scene IDs
// Order must match sm_on_enter/event/exit arrays in smart_meter_app.c.
// ------------------------------------------------------------
typedef enum {
    SmSceneSplash = 0,
    SmSceneMenu,
    SmSceneFreq,
    SmSceneMonitor,
    SmSceneInstructions,
    SmSceneAbout,
    SmSceneCount,
} SmSceneId;

// ------------------------------------------------------------
// View IDs
// ------------------------------------------------------------
typedef enum {
    SmViewSplash = 0,
    SmViewMenu,
    SmViewFreq,
    SmViewMonitor,
    SmViewInstructions,
    SmViewAbout,
    SmViewCount,
} SmViewId;

// ------------------------------------------------------------
// Custom events
// ------------------------------------------------------------
typedef enum {
    SmEventMenuStart = 0,
    SmEventMenuFreq,
    SmEventMenuInstr,
    SmEventMenuAbout,
} SmMenuEvent;

typedef enum {
    SmEventMonitorToggle = 0,
    SmEventMonitorReset,
    SmEventMonitorRateUp,
    SmEventMonitorRateDown,
    SmEventMonitorFreqNext,
    SmEventMonitorFreqPrev,
    SmEventMonitorThreshUp,
    SmEventMonitorThreshDown,
} SmMonitorEvent;

typedef enum {
    SmEventInstrPageLeft = 0,
    SmEventInstrPageRight,
} SmInstrEvent;

// ------------------------------------------------------------
// Forward declaration - allows model structs below to hold
// a SmartMeterApp* before the full struct is defined.
// ------------------------------------------------------------
typedef struct SmartMeterApp_s SmartMeterApp;

// ------------------------------------------------------------
// View models
// ------------------------------------------------------------

// Monitor view: draw callback reads app state through this pointer.
typedef struct {
    SmartMeterApp* app;
} SmMonitorModel;

// Instructions view: tracks which page is displayed.
typedef struct {
    uint8_t page;
} SmInstrModel;

// ------------------------------------------------------------
// Full application state struct
// ------------------------------------------------------------
struct SmartMeterApp_s {
    // Scene / view management
    SceneManager*    scene_manager;
    ViewDispatcher*  view_dispatcher;
    Submenu*         submenu_menu;
    Submenu*         submenu_freq;
    View*            view_splash;
    View*            view_monitor;
    View*            view_instructions;
    View*            view_about;

    // Timers and sync
    FuriTimer*       sample_timer;
    FuriTimer*       splash_timer;
    FuriMutex*       mutex;

    // Radio state
    bool             radio_running;
    bool             monitoring;   // capture on / off
    bool             ok_long_consumed; // prevents long-press release from also triggering short
    uint8_t          freq_index;   // index into k_frequencies[]
    uint8_t          rate_index;   // index into k_sample_rates[]

    // RSSI circular buffer
    int8_t           rssi_history[SM_HISTORY_SIZE];
    uint32_t         rssi_head;    // next write position
    uint32_t         rssi_count;   // samples stored (capped at SM_HISTORY_SIZE)
    int8_t           rssi_current; // most recent RSSI value

    // Threshold (dBm, adjusted by UP/DOWN)
    int8_t           threshold_dbm;

    // Burst detection
    bool             in_burst;
    uint8_t          burst_above;  // consecutive above-threshold samples
    uint8_t          burst_below;  // consecutive below-threshold samples

    // Transmission stats
    uint32_t         burst_count;              // total detected transmissions
    uint32_t         last_burst_tick;          // furi_get_tick() of last burst exit
    uint32_t         last_interval_ms;         // interval between last two bursts
    uint32_t         avg_interval_ms;          // running average interval

    // Tx/minute - rolling 60s window of burst exit timestamps
    uint32_t         burst_timestamps[30U];    // circular buffer of burst ticks
    uint8_t          burst_ts_head;            // next write index
    uint8_t          burst_ts_count;           // entries stored (capped at 30)
    uint8_t          tx_per_min;              // transmissions in last 60s

    // Burst duration / SCM framing hint
    uint32_t         burst_enter_tick;         // tick when burst was entered
    uint8_t          burst_sample_count;       // above-threshold samples in current burst
    uint8_t          last_burst_samples;       // sample count of last completed burst

    // Notifications
    NotificationApp* notifications;
};

// ------------------------------------------------------------
// Scene handler declarations
// ------------------------------------------------------------
void scene_splash_on_enter(void* context);
bool scene_splash_on_event(void* context, SceneManagerEvent event);
void scene_splash_on_exit(void* context);

void scene_menu_on_enter(void* context);
bool scene_menu_on_event(void* context, SceneManagerEvent event);
void scene_menu_on_exit(void* context);

void scene_freq_on_enter(void* context);
bool scene_freq_on_event(void* context, SceneManagerEvent event);
void scene_freq_on_exit(void* context);

void scene_monitor_on_enter(void* context);
bool scene_monitor_on_event(void* context, SceneManagerEvent event);
void scene_monitor_on_exit(void* context);

void scene_instructions_on_enter(void* context);
bool scene_instructions_on_event(void* context, SceneManagerEvent event);
void scene_instructions_on_exit(void* context);

void scene_about_on_enter(void* context);
bool scene_about_on_event(void* context, SceneManagerEvent event);
void scene_about_on_exit(void* context);

// ------------------------------------------------------------
// View setup functions (called once from sm_alloc)
// ------------------------------------------------------------
void scene_splash_setup_view(SmartMeterApp* app);
void scene_monitor_setup_view(SmartMeterApp* app);
void scene_instructions_setup_view(SmartMeterApp* app);
void scene_about_setup_view(SmartMeterApp* app);