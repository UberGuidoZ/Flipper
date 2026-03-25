// ####################################################
// #  scene_splash.c - title / splash screen          #
// #  By: UberGuidoZ | https://github.com/UberGuidoZ/ #
// #  v1.11                                           #
// ####################################################

#include "smart_meter.h"

// ============================================================
// Splash timer callback
// ============================================================
static void sm_splash_timer_cb(void* context) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);
    view_dispatcher_send_custom_event(app->view_dispatcher, SM_SPLASH_TIMER_DONE);
}

// ============================================================
// Draw callback
// ============================================================
static void sm_splash_draw_cb(Canvas* canvas, void* model) {
    UNUSED(model);
    canvas_clear(canvas);

    // ----------------------------------------------------------
    // Box around title block only. Measured at runtime for exact
    // centering regardless of font metrics.
    // ----------------------------------------------------------
    canvas_set_font(canvas, FontPrimary);
    size_t w1 = canvas_string_width(canvas, "Smart Meter");
    size_t w2 = canvas_string_width(canvas, "Monitor");
    size_t box_w = (w1 > w2 ? w1 : w2) + 16U;
    int32_t box_x = (int32_t)((128U - box_w) / 2U);
    if(box_x < 1) box_x = 1;
    canvas_draw_frame(canvas, box_x, 8, (uint8_t)box_w, 32);
    canvas_draw_str(canvas, (int32_t)((128U - w1) / 2U), 22, "Smart Meter");
    canvas_draw_str(canvas, (int32_t)((128U - w2) / 2U), 34, "Monitor");

    // ----------------------------------------------------------
    // Separator and byline (FontKeyboard ~6px/char).
    // "v1.11 by UberGuidoZ" = 19 chars * 6 = 114px, x=(128-114)/2 = 7
    // "github.com/UberGuidoZ" = 21 chars * 6 = 126px, x=1
    // ----------------------------------------------------------
    canvas_draw_line(canvas, 10, 44, 118, 44);
    canvas_set_font(canvas, FontKeyboard);
    canvas_draw_str(canvas,  7, 54, "v1.11 by UberGuidoZ");
    canvas_draw_str(canvas,  1, 62, "github.com/UberGuidoZ");
}

// ============================================================
// Input callback - any key skips the splash
// ============================================================
static bool sm_splash_input_cb(InputEvent* event, void* context) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);
    if(event->type == InputTypeShort || event->type == InputTypeLong) {
        view_dispatcher_send_custom_event(app->view_dispatcher, SM_SPLASH_TIMER_DONE);
        return true;
    }
    return false;
}

// ============================================================
// View setup
// ============================================================
void scene_splash_setup_view(SmartMeterApp* app) {
    furi_check(app != NULL);
    app->view_splash = view_alloc();
    furi_check(app->view_splash != NULL);
    view_set_draw_callback(app->view_splash, sm_splash_draw_cb);
    view_set_input_callback(app->view_splash, sm_splash_input_cb);
    view_set_context(app->view_splash, app);
}

// ============================================================
// Scene handlers
// ============================================================
void scene_splash_on_enter(void* context) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);

    app->splash_timer = furi_timer_alloc(sm_splash_timer_cb, FuriTimerTypeOnce, app);
    furi_check(app->splash_timer != NULL);
    furi_timer_start(app->splash_timer, SM_SPLASH_DELAY_MS);

    view_dispatcher_switch_to_view(app->view_dispatcher, SmViewSplash);
}

bool scene_splash_on_event(void* context, SceneManagerEvent event) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);

    if(event.type == SceneManagerEventTypeCustom &&
       event.event == SM_SPLASH_TIMER_DONE) {
        if(app->splash_timer != NULL) {
            furi_timer_stop(app->splash_timer);
            furi_timer_free(app->splash_timer);
            app->splash_timer = NULL;
        }
        scene_manager_next_scene(app->scene_manager, SmSceneMenu);
        return true;
    }
    return false;
}

void scene_splash_on_exit(void* context) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);
    if(app->splash_timer != NULL) {
        furi_timer_stop(app->splash_timer);
        furi_timer_free(app->splash_timer);
        app->splash_timer = NULL;
    }
}