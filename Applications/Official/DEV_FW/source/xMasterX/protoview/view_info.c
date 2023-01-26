/* Copyright (C) 2022-2023 Salvatore Sanfilippo -- All Rights Reserved
 * See the LICENSE file for information about the license. */

#include "app.h"

/* Renders the view with the detected message information. */
void render_view_info(Canvas *const canvas, ProtoViewApp *app) {
    if (app->signal_decoded == false) {
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 30,36,"No signal decoded");
        return;
    }

    /* Protocol name as title. */
    canvas_set_font(canvas, FontPrimary);
    uint8_t y = 8, lineheight = 10;
    canvas_draw_str(canvas, 0, y, app->signal_info.name);
    y += lineheight;

    /* Info fields. */
    char buf[128];
    canvas_set_font(canvas, FontSecondary);
    if (app->signal_info.raw[0]) {
        snprintf(buf,sizeof(buf),"Raw: %s", app->signal_info.raw);
        canvas_draw_str(canvas, 0, y, buf);
        y += lineheight;
    }
    canvas_draw_str(canvas, 0, y, app->signal_info.info1);
    y += lineheight;
    canvas_draw_str(canvas, 0, y, app->signal_info.info2);
    y += lineheight;
    canvas_draw_str(canvas, 0, y, app->signal_info.info3);
    y += lineheight;
}

/* Handle input for the settings view. */
void process_input_info(ProtoViewApp *app, InputEvent input) {
    UNUSED(app);
    UNUSED(input);
    return;
}
