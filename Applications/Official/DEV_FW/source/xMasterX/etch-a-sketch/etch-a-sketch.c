#include <assets_icons.h>
#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/elements.h>
#include <gui/icon.h>
#include <input/input.h>
#include <notification/notification.h>
#include <notification/notification_messages.h>
#include <stdbool.h> // Header-file for boolean data-type.
#include <stdio.h>
#include <string.h>

#define WIDTH 64
#define HEIGHT 32

const int brush_size = 2;

typedef struct selected_position {
    int x;
    int y;
} selected_position;

typedef struct {
    selected_position selected;
    bool board[64][32];
    bool isDrawing;
    bool showWelcome;
} EtchData;

// Sequence to indicate that drawing is enabled.
const NotificationSequence sequence_begin_draw = {
    &message_display_backlight_on,

    // Vibrate to indicate that drawing is enabled.
    &message_vibro_on,
    &message_note_g5,
    &message_delay_50,
    &message_note_c6,
    &message_delay_50,
    &message_note_e5,
    &message_vibro_off,
    &message_sound_off,
    NULL,
};

// sequence to indicate that drawing is disabled
const NotificationSequence sequence_end_draw = {
    &message_red_0,
    // Indicate that drawing is disabled.
    &message_vibro_on,
    &message_note_g5,
    &message_delay_50,
    &message_note_e5,
    &message_delay_50,
    &message_vibro_off,
    &message_sound_off,
    &message_do_not_reset,
    NULL,
};

// Indicate that drawing is enabled.
const NotificationSequence sequence_draw_enabled = {
    &message_red_255,
    &message_do_not_reset,
    NULL,
};

// Indicate that drawing is disabled.
const NotificationSequence sequence_draw_disabled = {
    &message_red_0,
    &message_do_not_reset,
    NULL,
};

const NotificationSequence sequence_cleanup = {
    &message_red_0,
    &message_green_0,
    &message_blue_0,
    &message_sound_off,
    &message_vibro_off,
    NULL,
};

void etch_draw_callback(Canvas* canvas, void* ctx) {
    const EtchData* etch_state = acquire_mutex((ValueMutex*)ctx, 25);
    UNUSED(ctx);

    canvas_clear(canvas);

    // Show Welcome Message
    if(etch_state->showWelcome) {
        // Draw Etch A Sketch frame
        canvas_draw_frame(canvas, 5, 3, 119, 55); // Border
        canvas_draw_icon(canvas, 8, 50, &I_Ok_btn_pressed_13x13); // Left Knob
        canvas_draw_icon(canvas, 107, 50, &I_Ok_btn_pressed_13x13); // Right Knob

        // Draw Etch A Sketch text banner
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 36, 15, "Etch A Sketch");

        // Draw Etch A Sketch instructions "Hold Back to clear"
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 31, 26, "* Hold ");
        canvas_draw_icon(canvas, 59, 18, &I_Pin_back_arrow_10x8);
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 72, 26, "to clear");

        // Draw Etch A Sketch instructions "Hold OK button to draw"
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 31, 37, "* Hold");
        canvas_draw_icon(canvas, 61, 30, &I_ButtonCenter_7x7);
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 72, 37, "to draw");
    }

    canvas_set_color(canvas, ColorBlack);
    //draw the canvas(64x32) on screen(144x64) using brush_size*brush_size tiles
    for(int y = 0; y < 32; y++) {
        for(int x = 0; x < 64; x++) {
            if(etch_state->board[x][y]) {
                canvas_draw_box(canvas, x * brush_size, y * brush_size, 2, 2);
            }
        }
    }

    //draw cursor as a brush_size by brush_size black box
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_box(
        canvas,
        etch_state->selected.x * brush_size,
        etch_state->selected.y * brush_size,
        brush_size,
        brush_size);

    //release the mutex
    release_mutex((ValueMutex*)ctx, etch_state);
}

void etch_input_callback(InputEvent* input_event, void* ctx) {
    furi_assert(ctx);
    FuriMessageQueue* event_queue = ctx;
    furi_message_queue_put(event_queue, input_event, FuriWaitForever);
}

int32_t etch_a_sketch_app(void* p) {
    UNUSED(p);
    FuriMessageQueue* event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    EtchData* etch_state = malloc(sizeof(EtchData));
    ValueMutex etch_state_mutex;
    if(!init_mutex(&etch_state_mutex, etch_state, sizeof(EtchData))) {
        FURI_LOG_E("etch", "cannot create mutex\r\n");
        free(etch_state);
        return -1;
    }

    // Configure view port
    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, etch_draw_callback, &etch_state_mutex);
    view_port_input_callback_set(view_port, etch_input_callback, event_queue);

    // Register view port in GUI
    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    NotificationApp* notification = furi_record_open(RECORD_NOTIFICATION);

    InputEvent event;

    // Show Welcome Banner
    etch_state->showWelcome = true;

    while(furi_message_queue_get(event_queue, &event, FuriWaitForever) == FuriStatusOk) {
        //break out of the loop if the back key is pressed
        if(event.key == InputKeyBack && event.type == InputTypeShort) {
            break;
        }

        // Clear
        // TODO: Do animation of shaking board
        if(event.key == InputKeyBack && event.type == InputTypeLong) {
            etch_state->showWelcome = false;
            etch_state->board[1][1] = true;
            for(int y = 0; y < 32; y++) {
                for(int x = 0; x < 64; x++) {
                    etch_state->board[x][y] = false;
                }
            }
            view_port_update(view_port);
        }

        // Keep LED on while drawing
        if(etch_state->isDrawing) {
            notification_message(notification, &sequence_draw_enabled);
        } else {
            notification_message(notification, &sequence_draw_disabled);
        }

        // Single Dot Select
        if(event.key == InputKeyOk && event.type == InputTypeShort) {
            etch_state->board[etch_state->selected.x][etch_state->selected.y] =
                !etch_state->board[etch_state->selected.x][etch_state->selected.y];
        }

        // Start Drawing
        if(event.key == InputKeyOk && event.type == InputTypeLong) {
            // notification_message(furi_record_open(RECORD_NOTIFICATION), &sequence_begin_draw);
            notification_message(notification, &sequence_begin_draw);

            if(etch_state->isDrawing) {
                // We're ending the drawing
                notification_message(notification, &sequence_end_draw);
            }

            etch_state->isDrawing = !etch_state->isDrawing;
            etch_state->board[etch_state->selected.x][etch_state->selected.y] = true;

            view_port_update(view_port);
        }

        //check the key pressed and change x and y accordingly
        if(event.type == InputTypeShort || event.type == InputTypeRepeat ||
           event.type == InputTypeLong) {
            switch(event.key) {
            case InputKeyUp:
                etch_state->selected.y -= 1;
                break;
            case InputKeyDown:
                etch_state->selected.y += 1;
                break;
            case InputKeyLeft:
                etch_state->selected.x -= 1;
                break;
            case InputKeyRight:
                etch_state->selected.x += 1;
                break;
            default:
                break;
            }

            //check if cursor position is out of bounds and reset it to the closest position
            if(etch_state->selected.x < 0) {
                etch_state->selected.x = 0;
            }
            if(etch_state->selected.x > 61) {
                etch_state->selected.x = 61;
            }
            if(etch_state->selected.y < 0) {
                etch_state->selected.y = 0;
            }
            if(etch_state->selected.y > 31) {
                etch_state->selected.y = 31;
            }
            if(etch_state->isDrawing == true) {
                etch_state->board[etch_state->selected.x][etch_state->selected.y] = true;
            }
            view_port_update(view_port);
        }
    }

    notification_message(notification, &sequence_cleanup);
    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    furi_message_queue_free(event_queue);
    free(etch_state);
    furi_record_close(RECORD_NOTIFICATION);
    furi_record_close(RECORD_GUI);

    return 0;
}
