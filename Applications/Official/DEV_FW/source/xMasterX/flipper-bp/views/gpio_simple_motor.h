#pragma once
#include <gui/view.h>
#include <furi_hal_gpio.h>
#include <furi_hal_pwm.h>

typedef struct FBP FBP;
typedef struct GPIOSimpleMotor GPIOSimpleMotor;

GPIOSimpleMotor* gpio_simple_motor_alloc(FBP* fbp);

void gpio_simple_motor_free(GPIOSimpleMotor* motor_app);

View* gpio_simple_motor_get_view(GPIOSimpleMotor* motor_app);
