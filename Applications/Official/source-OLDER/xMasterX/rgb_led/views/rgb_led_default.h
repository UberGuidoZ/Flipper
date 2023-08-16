#pragma once

#include <gui/view.h>
#include "../led_ll.h"


void rgb_led_init(void);

typedef struct RgbLedDefault RgbLedDefault;

RgbLedDefault* rgb_led_default_alloc();

void rgb_led_default_free(RgbLedDefault* rgb_led_default);

View* rgb_led_default_get_view(RgbLedDefault* rgb_led_default);

void rgb_led_default_set_connected_status(RgbLedDefault* rgb_led_default, bool connected);
