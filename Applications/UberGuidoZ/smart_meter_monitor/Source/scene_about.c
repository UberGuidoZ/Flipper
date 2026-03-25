// ####################################################
// #  scene_about.c - about screen                    #
// #  By: UberGuidoZ | https://github.com/UberGuidoZ/ #
// #  v1.11                                           #
// ####################################################

#include "smart_meter.h"

static void sm_about_draw_cb(Canvas* canvas, void* model) {
    UNUSED(model);
    canvas_clear(canvas);

    // ----------------------------------------------------------
    // Box around title block measured at runtime for exact
    // centering. Identical geometry to the splash screen.
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
    // Separator and byline.
    // ----------------------------------------------------------
    canvas_draw_line(canvas, 10, 44, 118, 44);
    canvas_set_font(canvas, FontKeyboard);
    canvas_draw_str(canvas,  7, 54, "v1.11 by UberGuidoZ");
    canvas_draw_str(canvas,  1, 62, "github.com/UberGuidoZ");
}

static bool sm_about_input_cb(InputEvent* event, void* context) {
    UNUSED(event);
    UNUSED(context);
    return false;
}

void scene_about_setup_view(SmartMeterApp* app) {
    furi_check(app != NULL);

    app->view_about = view_alloc();
    furi_check(app->view_about != NULL);

    view_set_draw_callback(app->view_about, sm_about_draw_cb);
    view_set_input_callback(app->view_about, sm_about_input_cb);
}

void scene_about_on_enter(void* context) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);
    view_dispatcher_switch_to_view(app->view_dispatcher, SmViewAbout);
}

bool scene_about_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void scene_about_on_exit(void* context) {
    UNUSED(context);
}