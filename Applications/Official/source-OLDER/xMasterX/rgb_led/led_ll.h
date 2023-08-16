#ifndef _LED_H
#define _LED_H

#include <stdint.h>

#define LED_BRIGHTNESS_MAX 3
#define LED_COUNT 100

#define RGB_UINT(R, G, B) (((G) << 16) | ((R) << 8) | (B))
#define RGB_R(V) ((V >> 8) & 0xFF)
#define RGB_G(V) ((V >> 16) & 0xFF)
#define RGB_B(V) (V & 0xFF)


void led_init();
void led_deinit();
int8_t led_update();
uint8_t led_get_brightness();
uint32_t led_get(uint16_t i);

void led_set(uint16_t i, uint32_t v);
void led_set_rgb(uint16_t i, uint32_t r, uint32_t g, uint32_t b);
void led_set_rgbf(uint16_t i, float r, float g, float b);
void led_clear();

void led_set_brightness(uint8_t brightness);

#endif