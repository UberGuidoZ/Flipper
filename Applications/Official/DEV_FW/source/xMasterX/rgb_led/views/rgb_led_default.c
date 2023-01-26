#include "rgb_led_default.h"
#include <furi.h>
#include <furi_hal_usb_hid.h>
#include <gui/elements.h>
#include <RGB_LED_icons.h>

struct RgbLedDefault{
    View* view;
};

typedef struct {
    bool left_pressed;
    bool up_pressed;
    bool right_pressed;
    bool down_pressed;
    bool ok_pressed;
    bool back_pressed;
    bool connected;
} RgbLedDefaultModel;

uint8_t led_status = 0;

void rgb_led_init() {
    /// let's see it crashes
    led_init();
}

#define COLORS_LEN 7

uint32_t colors[COLORS_LEN][3] = {
    {255, 255, 255}, 
    {255, 0, 0}, 
    {0, 255, 0}, 
    {0, 0, 255}, 
    {255, 255, 0}, 
    {255, 0, 255}, 
    {0, 255, 255}
};

void rgb_led_fire() {
    for (uint8_t i = 0; i < 15; i++)
    {
        uint8_t r = colors[3][0];
        uint8_t g = colors[3][1];
        uint8_t b = colors[3][2];

        led_set_rgb(i, r, g, b);
    }

    led_status = led_update();

    
}

static void rgb_led_default_draw_callback(Canvas* canvas, void* context) {
    furi_assert(context);
    RgbLedDefaultModel* model = context;
    canvas_set_font(canvas, FontPrimary);
    elements_multiline_text_aligned(canvas, 0, 20, AlignLeft, AlignTop, model->up_pressed ? "HIT" : "WAIT");
    elements_multiline_text_aligned(canvas, 0, 40, AlignLeft, AlignTop, led_status == 0 ? "LED OK" : "BUSY");
    return;
}

static void rgb_led_default_process(RgbLedDefault* rgb_led_default, InputEvent* event) {
    with_view_model(
        rgb_led_default->view,
        RgbLedDefaultModel * model,
        {
            if(event->type == InputTypePress) {
                if(event->key == InputKeyUp) {
                  rgb_led_fire();
                  model->up_pressed = true;  
                }
            } else if(event->type == InputTypeRelease) {
                if(event->key == InputKeyUp) {
                    model->up_pressed = false;
                } 
            }
        },
        true);
}

static bool rgb_led_default_input_callback(InputEvent* event, void* context) {
    furi_assert(context);
    RgbLedDefault* rgb_led_default = context;
    bool consumed = false;

    if(event->type == InputTypeLong && event->key == InputKeyBack) {
//        furi_hal_hid_kb_release_all();
    } else {
        rgb_led_default_process(rgb_led_default, event);
        consumed = true;
    }

    return consumed;
}

RgbLedDefault* rgb_led_default_alloc() {
    RgbLedDefault* rgb_led_default = malloc(sizeof(RgbLedDefault));
    rgb_led_default->view = view_alloc();
    view_set_context(rgb_led_default->view, rgb_led_default);
    view_allocate_model(rgb_led_default->view, ViewModelTypeLocking, sizeof(RgbLedDefaultModel));
    view_set_draw_callback(rgb_led_default->view, rgb_led_default_draw_callback);
    view_set_input_callback(rgb_led_default->view, rgb_led_default_input_callback);

    rgb_led_init();

    return rgb_led_default;
}

void rgb_led_default_free(RgbLedDefault* rgb_led_default) {
    furi_assert(rgb_led_default);
    view_free(rgb_led_default->view);
    free(rgb_led_default);
}

View* rgb_led_default_get_view(RgbLedDefault* rgb_led_default) {
    furi_assert(rgb_led_default);

    return rgb_led_default->view;
}

void rgb_led_default_set_connected_status(RgbLedDefault* rgb_led_default, bool connected) {
    furi_assert(rgb_led_default);
    with_view_model(
        rgb_led_default->view, RgbLedDefaultModel * model, { model->connected = connected; }, true);
}
