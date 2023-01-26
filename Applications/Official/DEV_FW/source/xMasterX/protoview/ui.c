/* Copyright (C) 2022-2023 Salvatore Sanfilippo -- All Rights Reserved
 * See the LICENSE file for information about the license. */

#include "app.h"

void canvas_draw_str_with_border(Canvas* canvas, uint8_t x, uint8_t y, const char* str, Color text_color, Color border_color)
{
    struct {
        uint8_t x; uint8_t y;
    } dir[8] = {
        {-1,-1},
        {0,-1},
        {1,-1},
        {1,0},
        {1,1},
        {0,1},
        {-1,1},
        {-1,0}
    };

    /* Rotate in all the directions writing the same string to create a
     * border, then write the actual string in the other color in the
     * middle. */
    canvas_set_color(canvas, border_color);
    for (int j = 0; j < 8; j++)
        canvas_draw_str(canvas,x+dir[j].x,y+dir[j].y,str);
    canvas_set_color(canvas, text_color);
    canvas_draw_str(canvas,x,y,str);
    canvas_set_color(canvas, ColorBlack);
}
