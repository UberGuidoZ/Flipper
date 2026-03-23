// #####################################################
// #  scene_about.c - about screen                     #
// #  By: UberGuidoZ | https://github.com/UberGuidoZ/  #
// #  v2.01                                            #
// #####################################################

#include "gpio_analyzer.h"

// ============================================================
// Draw callback - static content, no model needed
// ============================================================

static void ga_about_draw_cb(Canvas* canvas, void* model) {
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
// Input callback - no local inputs; BACK is handled by the
// ViewDispatcher navigation callback (pops the scene).
// ============================================================

static bool ga_about_input_cb(InputEvent* event, void* context) {
    UNUSED(event);
    UNUSED(context);
    return false;
}

// ============================================================
// View setup (called once from ga_alloc)
// ============================================================

void scene_about_setup_view(GpioAnalyzer* app) {
    furi_check(app != NULL);

    app->view_about = view_alloc();
    furi_check(app->view_about != NULL);

    view_set_draw_callback(app->view_about, ga_about_draw_cb);
    view_set_input_callback(app->view_about, ga_about_input_cb);
    // No model needed for static content.
}

// ============================================================
// Scene handlers
// ============================================================

void scene_about_on_enter(void* context) {
    GpioAnalyzer* app = (GpioAnalyzer*)context;
    furi_check(app != NULL);
    view_dispatcher_switch_to_view(app->view_dispatcher, GaViewAbout);
}

bool scene_about_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void scene_about_on_exit(void* context) {
    UNUSED(context);
}