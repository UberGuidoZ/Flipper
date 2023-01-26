/* Copyright (C) 2022-2023 Salvatore Sanfilippo -- All Rights Reserved
 * See the LICENSE file for information about the license. */

#include "app.h"

/* Renders a single view with frequency and modulation setting. However
 * this are logically two different views, and only one of the settings
 * will be highlighted. */
void render_view_settings(Canvas *const canvas, ProtoViewApp *app) {
    canvas_set_font(canvas, FontPrimary);
    if (app->current_view == ViewFrequencySettings)
        canvas_draw_str_with_border(canvas,1,10,"Frequency",ColorWhite,ColorBlack);
    else
        canvas_draw_str(canvas,1,10,"Frequency");

    if (app->current_view == ViewModulationSettings)
        canvas_draw_str_with_border(canvas,70,10,"Modulation",ColorWhite,ColorBlack);
    else
        canvas_draw_str(canvas,70,10,"Modulation");
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas,10,61,"Use up and down to modify");

    /* Show frequency. We can use big numbers font since it's just a number. */
    if (app->current_view == ViewFrequencySettings) {
        char buf[16];
        snprintf(buf,sizeof(buf),"%.2f",(double)app->frequency/1000000);
        canvas_set_font(canvas, FontBigNumbers);
        canvas_draw_str(canvas, 30, 40, buf);
    } else if (app->current_view == ViewModulationSettings) {
        int current = app->modulation;
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str(canvas, 33, 39, ProtoViewModulations[current].name);
    }
}

/* Handle input for the settings view. */
void process_input_settings(ProtoViewApp *app, InputEvent input) {
    if (input.type == InputTypeLong && input.key == InputKeyOk) {
        /* Long pressing to OK sets the default frequency and
         * modulation. */
        app->frequency = subghz_setting_get_default_frequency(app->setting);
        app->modulation = 0;
    } else if (input.type == InputTypePress &&
              (input.key != InputKeyDown || input.key != InputKeyUp))
    {
        /* Handle up and down to change frequency or modulation. */
        if (app->current_view == ViewFrequencySettings) {
            size_t curidx = 0, i;
            size_t count = subghz_setting_get_frequency_count(app->setting);

            /* Scan the list of frequencies to check for the index of the
             * currently set frequency. */
            for(i = 0; i < count; i++) {
                uint32_t freq = subghz_setting_get_frequency(app->setting,i);
                if (freq == app->frequency) {
                    curidx = i;
                    break;
                }
            }
            if (i == count) return; /* Should never happen. */

            if (input.key == InputKeyUp) {
                curidx = (curidx+1) % count;
            } else if (input.key == InputKeyDown) {
                curidx = curidx == 0 ? count-1 : curidx-1;
            } else {
                return;
            }
            app->frequency = subghz_setting_get_frequency(app->setting,curidx);
        } else if (app->current_view == ViewModulationSettings) {
            uint32_t count = 0;
            uint32_t modid = app->modulation;

            while(ProtoViewModulations[count].name != NULL) count++;
            if (input.key == InputKeyUp) {
                modid = (modid+1) % count;
            } else if (input.key == InputKeyDown) {
                modid = modid == 0 ? count-1 : modid-1;
            } else {
                return;
            }
            app->modulation = modid;
        }
    } else {
        return;
    }

    /* Apply changes. */
    FURI_LOG_E(TAG, "Setting view, setting frequency/modulation to %lu %s", app->frequency, ProtoViewModulations[app->modulation].name);
    radio_rx_end(app);
    radio_begin(app);
    radio_rx(app);
}
