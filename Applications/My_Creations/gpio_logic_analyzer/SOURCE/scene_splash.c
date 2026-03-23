// #####################################################
// #  scene_splash.c - title / splash screen           #
// #  By: UberGuidoZ | https://github.com/UberGuidoZ/  #
// #  v2.01                                            #
// #####################################################
//
//  Displays a bordered title card for 1.5 seconds, then
//  automatically advances to the main menu. Any key press
//  also skips the splash immediately.
//
//  Layout (128x64 OLED):
//
//    +---------------------+  <- 2px border with corner marks
//    |                     |  <- Box around title text, centered
//    |     GPIO Logic      |  <- FontPrimary, line 1
//    |      Analyzer       |  <- FontPrimary, line 2
//    |---------------------|  <- separator
//    | v2.01 by UberGuidoZ |  <- FontKeyboard byline
//    |github.com/UberGuidoZ|  <- FontKeyboard URL
//    +---------------------+
//
// ##############################################################

#include "gpio_analyzer.h"

// Auto-advance delay in milliseconds
#define GA_SPLASH_DELAY_MS  3000U

// Custom event fired when the timer expires
#define GA_SPLASH_TIMER_DONE  0U

// ============================================================
// Splash timer callback (FreeRTOS timer service task)
// Fires once after GA_SPLASH_DELAY_MS.
// ============================================================

static void ga_splash_timer_cb(void* context) {
    GpioAnalyzer* app = (GpioAnalyzer*)context;
    furi_check(app != NULL);
    view_dispatcher_send_custom_event(app->view_dispatcher, GA_SPLASH_TIMER_DONE);
}

// ============================================================
// Draw callback - pure static content, no model needed
// ============================================================

static void ga_splash_draw_cb(Canvas* canvas, void* model) {
    UNUSED(model);
    canvas_clear(canvas);

    // ----------------------------------------------------------
    // Title - FontPrimary. Measure actual pixel widths at runtime
    // so centering is exact regardless of font metrics.
    // Box is sized from the wider string + 8px padding each side.
    // ----------------------------------------------------------
    canvas_set_font(canvas, FontPrimary);
    size_t w1 = canvas_string_width(canvas, "GPIO Logic");
    size_t w2 = canvas_string_width(canvas, "Analyzer");
    size_t box_w = (w1 > w2 ? w1 : w2) + 16U;
    int32_t box_x = (int32_t)((128U - box_w) / 2U);
    if(box_x < 1) box_x = 1;
    canvas_draw_frame(canvas, box_x, 8, (uint8_t)box_w, 32);
    canvas_draw_str(canvas, (int32_t)((128U - w1) / 2U), 22, "GPIO Logic");
    canvas_draw_str(canvas, (int32_t)((128U - w2) / 2U), 34, "Analyzer");

    // ----------------------------------------------------------
    // Separator between title box and byline.
    // ----------------------------------------------------------
    canvas_draw_line(canvas, 10, 44, 118, 44);

    // ----------------------------------------------------------
    // Byline - FontKeyboard (~6px/char, ~8px tall).
    // "v2.01 by UberGuidoZ"  = 19 chars * 6 = 114px, x=(128-114)/2 = 7
    // "github.com/UberGuidoZ" = 21 chars * 6 = 126px, x=(128-126)/2 = 1
    // ----------------------------------------------------------
    canvas_set_font(canvas, FontKeyboard);
    canvas_draw_str(canvas,  7, 54, "v2.01 by UberGuidoZ");
    canvas_draw_str(canvas,  1, 62, "github.com/UberGuidoZ");
}

// ============================================================
// Input callback - any key skips the splash immediately
// ============================================================

static bool ga_splash_input_cb(InputEvent* event, void* context) {
    GpioAnalyzer* app = (GpioAnalyzer*)context;
    furi_check(app != NULL);

    // Only act on key-down edges to avoid double-fires.
    if(event->type == InputTypeShort || event->type == InputTypeLong) {
        view_dispatcher_send_custom_event(app->view_dispatcher, GA_SPLASH_TIMER_DONE);
        return true;
    }
    return false;
}

// ============================================================
// View setup (called once from ga_alloc)
// ============================================================

void scene_splash_setup_view(GpioAnalyzer* app) {
    furi_check(app != NULL);

    app->view_splash = view_alloc();
    furi_check(app->view_splash != NULL);

    view_set_draw_callback(app->view_splash, ga_splash_draw_cb);
    view_set_input_callback(app->view_splash, ga_splash_input_cb);
    view_set_context(app->view_splash, app);
    // No model - all content is static.
}

// ============================================================
// Scene handlers
// ============================================================

void scene_splash_on_enter(void* context) {
    GpioAnalyzer* app = (GpioAnalyzer*)context;
    furi_check(app != NULL);

    // Allocate a one-shot timer. It is freed in on_exit so the
    // callback cannot fire after the scene has been torn down.
    app->splash_timer = furi_timer_alloc(
        ga_splash_timer_cb, FuriTimerTypeOnce, app);
    furi_check(app->splash_timer != NULL);
    furi_timer_start(app->splash_timer, GA_SPLASH_DELAY_MS);

    view_dispatcher_switch_to_view(app->view_dispatcher, GaViewSplash);
}

bool scene_splash_on_event(void* context, SceneManagerEvent event) {
    GpioAnalyzer* app = (GpioAnalyzer*)context;
    furi_check(app != NULL);

    if(event.type == SceneManagerEventTypeCustom &&
       event.event == GA_SPLASH_TIMER_DONE) {
        // Stop and free the timer before transitioning so it cannot
        // double-fire if the user tapped a key and the timer also
        // expired in the same tick.
        if(app->splash_timer != NULL) {
            furi_timer_stop(app->splash_timer);
            furi_timer_free(app->splash_timer);
            app->splash_timer = NULL;
        }
        scene_manager_next_scene(app->scene_manager, GaSceneMenu);
        return true;
    }
    return false;
}

void scene_splash_on_exit(void* context) {
    GpioAnalyzer* app = (GpioAnalyzer*)context;
    furi_check(app != NULL);

    // Guard in case on_event already freed it (key-skip path).
    if(app->splash_timer != NULL) {
        furi_timer_stop(app->splash_timer);
        furi_timer_free(app->splash_timer);
        app->splash_timer = NULL;
    }
}