// ######################################################
// #  scene_instructions.c - 4-page on-screen reference #
// #  By: UberGuidoZ | https://github.com/UberGuidoZ/   #
// #  v1.11                                             #
// ######################################################
//
//  Page 1: About Smart Meters
//  Page 2: Frequencies
//  Page 3: Display Guide
//  Page 4: Controls
//
//  LEFT  - previous page
//  RIGHT - next page
//  BACK  - return to menu
//
// ##############################################################

#include "smart_meter.h"
#include <stdio.h>
#include <string.h>

static const char* const k_page_titles[SM_INSTR_PAGES] = {
    "About Smart Meters",
    "Frequencies",
    "Display Guide",
    "Controls",
};

// Max 21 chars per line (FontKeyboard at 6px/char = 126px)
static const char* const k_page_body[SM_INSTR_PAGES][SM_INSTR_BODY_LINES] = {
    // Page 0: About Smart Meters
    {
        "Meters broadcast ERT",
        "packets every 30-60s",
        "CC1101 detects RSSI",
        "spikes from each TX",
        "No decoding needed",
    },
    // Page 1: Frequencies
    {
        "433MHz some water",
        "868.30MHz EU electric",
        "915.00MHz US electric",
        "LEFT/RIGHT to change",
        "Changing freq resets",
    },
    // Page 2: Display Guide
    {
        "Bars = RSSI strength",
        "Dashed line=threshold",
        "Spike = transmission",
        "C=count /m=per min",
        "~SCM=brief INT?=noise",
    },
    // Page 3: Controls
    {
        "OK       pause/resume",
        "OK hold  reset stats",
        "UP/DOWN  threshold",
        "Hold U/D rate change",
        "L/R      frequency",
    },
};

// ============================================================
// Draw callback
// ============================================================
static void sm_instructions_draw_cb(Canvas* canvas, void* model) {
    SmInstrModel* m = (SmInstrModel*)model;
    if(m == NULL) return;

    uint8_t page = m->page;
    if(page >= SM_INSTR_PAGES) page = 0U;

    canvas_clear(canvas);

    // ----------------------------------------------------------
    // Title bar with page indicator right-aligned (same baseline).
    // ----------------------------------------------------------
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 1, 7, k_page_titles[page]);

    char indicator[8];
    snprintf(indicator, sizeof(indicator), "%d/%d", (int)(page + 1U), (int)SM_INSTR_PAGES);
    size_t iw = canvas_string_width(canvas, indicator);
    canvas_draw_str(canvas, (int32_t)(127U - iw), 7, indicator);

    canvas_draw_line(canvas, 0, 9, 127, 9);

    // ----------------------------------------------------------
    // Body text: 5 lines, 10px spacing (8px glyph + 2px gap).
    // Lines at baselines y=19,29,39,49,59.
    // ----------------------------------------------------------
    canvas_set_font(canvas, FontKeyboard);
    int32_t y = 19;
    for(uint8_t i = 0U; i < SM_INSTR_BODY_LINES; i++) {
        if(k_page_body[page][i] != NULL && k_page_body[page][i][0] != '\0') {
            canvas_draw_str(canvas, 1, y, k_page_body[page][i]);
        }
        y += 10;
    }
}

// ============================================================
// Input callback
// ============================================================
static bool sm_instructions_input_cb(InputEvent* event, void* context) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);

    if(event->type == InputTypeShort) {
        switch(event->key) {
        case InputKeyLeft:
            view_dispatcher_send_custom_event(
                app->view_dispatcher, SmEventInstrPageLeft);
            return true;
        case InputKeyRight:
            view_dispatcher_send_custom_event(
                app->view_dispatcher, SmEventInstrPageRight);
            return true;
        default:
            break;
        }
    }
    return false;
}

// ============================================================
// View setup
// ============================================================
void scene_instructions_setup_view(SmartMeterApp* app) {
    furi_check(app != NULL);

    app->view_instructions = view_alloc();
    furi_check(app->view_instructions != NULL);

    view_set_draw_callback(app->view_instructions, sm_instructions_draw_cb);
    view_set_input_callback(app->view_instructions, sm_instructions_input_cb);
    view_set_context(app->view_instructions, app);

    view_allocate_model(
        app->view_instructions, ViewModelTypeLockFree, sizeof(SmInstrModel));

    SmInstrModel* m = view_get_model(app->view_instructions);
    if(m != NULL) m->page = 0U;
    view_commit_model(app->view_instructions, false);
}

// ============================================================
// Scene handlers
// ============================================================
void scene_instructions_on_enter(void* context) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);

    SmInstrModel* m = view_get_model(app->view_instructions);
    if(m != NULL) m->page = 0U;
    view_commit_model(app->view_instructions, false);

    view_dispatcher_switch_to_view(app->view_dispatcher, SmViewInstructions);
}

bool scene_instructions_on_event(void* context, SceneManagerEvent event) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);

    if(event.type != SceneManagerEventTypeCustom) return false;

    SmInstrModel* m = view_get_model(app->view_instructions);
    if(m == NULL) return false;

    switch(event.event) {
    case SmEventInstrPageLeft:
        if(m->page > 0U) m->page--;
        view_commit_model(app->view_instructions, true);
        return true;
    case SmEventInstrPageRight:
        if(m->page < SM_INSTR_PAGES - 1U) m->page++;
        view_commit_model(app->view_instructions, true);
        return true;
    default:
        break;
    }
    return false;
}

void scene_instructions_on_exit(void* context) {
    UNUSED(context);
}