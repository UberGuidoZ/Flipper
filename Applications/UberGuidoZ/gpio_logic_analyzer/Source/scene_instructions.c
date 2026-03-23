// #######################################################
// #  scene_instructions.c - 4-page on-screen reference  #
// #  By: UberGuidoZ | https://github.com/UberGuidoZ/    #
// #  v2.01                                              #
// #######################################################
//
//  Page 1: Voltage & Safety
//  Page 2: Display Guide
//  Page 3: Controls
//  Page 4: CSV Logging
//
//  LEFT  - previous page
//  RIGHT - next page
//  BACK  - return to menu
//
// ##############################################################

#include "gpio_analyzer.h"
#include <stdio.h>
#include <string.h>

// ============================================================
// Page content
// Each page: title + up to GA_INSTR_BODY_LINES body lines.
// Lines are rendered with FontKeyboard (~4px per char, 6px line pitch).
// Max safe line length: ~30 chars.
// ============================================================

#define GA_INSTR_BODY_LINES  5U

static const char* const k_page_titles[GA_INSTR_PAGES] = {
    "Voltage & Safety",
    "Display Guide",
    "Controls",
    "CSV Logging",
};

static const char* const k_page_body[GA_INSTR_PAGES][GA_INSTR_BODY_LINES] = {
    // Page 0: Voltage & Safety (max 21 chars)
    {
        "3.3V MAX on all pins",
        "PA4: 5V tolerant",
        "Others: 3.3V ONLY",
        "No pull resistors",
        "Never short to VCC!",
    },
    // Page 1: Display Guide (max 21 chars)
    {
        "Each row = one pin",
        "Labels: A7,A6...C0",
        "Waveform scrolls left",
        "Filled box = HIGH",
        "Outline box = LOW",
    },
    // Page 2: Controls (max 21 chars)
    {
        "OK       start/stop",
        "OK hold  clr history",
        "UP/DOWN  rate change",
        "BACK     exit to menu",
        "* header = CSV active",
    },
    // Page 3: CSV Logging (max 21 chars)
    {
        "Auto-logs to SD card",
        "gla_YYYYMMDD_HHMMSS",
        "/ext/gpio_analyzer/",
        "Cols: sample,A7..C0",
        "0 = LOW   1 = HIGH",
    },
};

// ============================================================
// Draw callback
// ============================================================

static void ga_instructions_draw_cb(Canvas* canvas, void* model) {
    GaInstrModel* m = (GaInstrModel*)model;
    if(m == NULL) return;

    uint8_t page = m->page;
    if(page >= GA_INSTR_PAGES) page = 0U;

    canvas_clear(canvas);

    // ----------------------------------------------------------
    // Title bar (FontSecondary, baseline y=7).
    // Page indicator right-aligned on the same line: "1/4"
    // ----------------------------------------------------------
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 1, 7, k_page_titles[page]);

    char indicator[8];
    snprintf(indicator, sizeof(indicator), "%d/%d", (int)(page + 1U), (int)GA_INSTR_PAGES);
    size_t iw = canvas_string_width(canvas, indicator);
    canvas_draw_str(canvas, (int32_t)(127U - iw), 7, indicator);

    // Separator
    canvas_draw_line(canvas, 0, 9, 127, 9);

    // ----------------------------------------------------------
    // Body text (FontKeyboard ~8px tall, 10px line spacing = 2px gap).
    // 5 lines at y=19,29,39,49,59. Last line on page 3 is centered.
    // ----------------------------------------------------------
    canvas_set_font(canvas, FontKeyboard);
    int32_t y = 19;
    for(uint8_t i = 0U; i < GA_INSTR_BODY_LINES; i++) {
        if(k_page_body[page][i] != NULL && k_page_body[page][i][0] != '\0') {
            int32_t lx = 1;
            if(page == 3U && i == GA_INSTR_BODY_LINES - 1U) {
                size_t lw = canvas_string_width(canvas, k_page_body[page][i]);
                lx = (int32_t)((128U - lw) / 2U);
                if(lx < 1) lx = 1;
            }
            canvas_draw_str(canvas, lx, y, k_page_body[page][i]);
        }
        y += 10;
    }
}

// ============================================================
// Input callback
// ============================================================

static bool ga_instructions_input_cb(InputEvent* event, void* context) {
    GpioAnalyzer* app = (GpioAnalyzer*)context;
    furi_check(app != NULL);
    bool consumed = false;

    if(event->type == InputTypeShort) {
        switch(event->key) {
        case InputKeyLeft:
            view_dispatcher_send_custom_event(
                app->view_dispatcher, GaEventInstrPageLeft);
            consumed = true;
            break;
        case InputKeyRight:
            view_dispatcher_send_custom_event(
                app->view_dispatcher, GaEventInstrPageRight);
            consumed = true;
            break;
        default:
            break;
        }
    }

    return consumed;
}

// ============================================================
// View setup (called once from ga_alloc)
// ============================================================

void scene_instructions_setup_view(GpioAnalyzer* app) {
    furi_check(app != NULL);

    app->view_instructions = view_alloc();
    furi_check(app->view_instructions != NULL);

    view_set_draw_callback(app->view_instructions, ga_instructions_draw_cb);
    view_set_input_callback(app->view_instructions, ga_instructions_input_cb);
    view_set_context(app->view_instructions, app);

    // LockFree model: just holds the current page number.
    // Only ever written from the scene event handler (GUI thread).
    view_allocate_model(
        app->view_instructions, ViewModelTypeLockFree, sizeof(GaInstrModel));

    GaInstrModel* m = view_get_model(app->view_instructions);
    if(m != NULL) m->page = 0U;
    view_commit_model(app->view_instructions, false);
}

// ============================================================
// Scene handlers
// ============================================================

void scene_instructions_on_enter(void* context) {
    GpioAnalyzer* app = (GpioAnalyzer*)context;
    furi_check(app != NULL);

    // Always start on page 0 when the user enters instructions.
    GaInstrModel* m = view_get_model(app->view_instructions);
    if(m != NULL) m->page = 0U;
    view_commit_model(app->view_instructions, false);

    view_dispatcher_switch_to_view(app->view_dispatcher, GaViewInstructions);
}

bool scene_instructions_on_event(void* context, SceneManagerEvent event) {
    GpioAnalyzer* app = (GpioAnalyzer*)context;
    furi_check(app != NULL);
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        GaInstrModel* m = view_get_model(app->view_instructions);
        if(m == NULL) return false;

        switch(event.event) {
        case GaEventInstrPageLeft:
            if(m->page > 0U) m->page--;
            view_commit_model(app->view_instructions, true);
            consumed = true;
            break;

        case GaEventInstrPageRight:
            if(m->page < GA_INSTR_PAGES - 1U) m->page++;
            view_commit_model(app->view_instructions, true);
            consumed = true;
            break;

        default:
            break;
        }
    }

    return consumed;
}

void scene_instructions_on_exit(void* context) {
    UNUSED(context);
    // Nothing to tear down.
}