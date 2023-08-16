#include "gpio_simple_motor.h"
#include "../fbp.h"

static const uint16_t BT_SERIAL_BUFFER_SIZE = 128;
static const uint32_t DEFAULT_FREQ = 1000;
static const FuriHalPwmOutputId DEFAULT_PWM_OUTPUT_ID = FuriHalPwmOutputIdTim1PA7;

struct GPIOSimpleMotor {
    View* view;
    FBP* fbp;

    uint8_t current_pwm_duty;
};

typedef struct {
    char* display_text_1;
    char* display_text_2;
    char* display_text_3;
} GPIOSimpleMotorModel;


static void process_general_command(TCodeCommand command, GPIOSimpleMotor* motor) {
    if (command.command_type == Magnitude && command.data.magnitude_command.motion_type == Vibrate && command.data.magnitude_command.channel_id == 0) {
        // just enable vibration on X
        uint8_t new_duty = (uint8_t) (command.data.magnitude_command.magnitude * 100);
        if (new_duty > 100) {
            new_duty = 100;
        }
        FURI_LOG_D(TAG, "Setting vibration power on %u", new_duty);

        // using Pulse-Widht Modulation to control a motor via a transistor
        // just google for a typical arduino + PWM + motor scheme
        if (new_duty == 0) {
            furi_hal_pwm_stop(DEFAULT_PWM_OUTPUT_ID);
        } else if (motor->current_pwm_duty == 0) {
            furi_hal_pwm_start(DEFAULT_PWM_OUTPUT_ID, DEFAULT_FREQ, new_duty);
        } else {
            furi_hal_pwm_set_params(DEFAULT_PWM_OUTPUT_ID, DEFAULT_FREQ, new_duty);
        }
        motor->current_pwm_duty = new_duty;
        return;
    }
}

static uint16_t bt_serial_event_callback(SerialServiceEvent event, void* context) {
    furi_assert(context);
    GPIOSimpleMotor* motor = context;

    if(event.event == SerialServiceEventTypeDataReceived) {
        TCodeCommandArray commands = tcode_decode(event.data.buffer, event.data.size);
        FURI_LOG_D(TAG, "Decoded commands array size: %u", commands.size);
        for (uint16_t i = 0; i < commands.size; i++) {
            FURI_LOG_D(TAG, "Command #%u, type: %u\n", i, commands.commands[i].command_type);
        }
        for (uint16_t i = 0; i < commands.size; i++) {
            // looking for first vibro command to execute
            TCodeCommand current_command = commands.commands[i];
            TCodeCommandType type = current_command.command_type;
            if ((type == Magnitude || type == MagnitudeSpeed || type == MagnitudeTimeInterval)) {
                process_general_command(current_command, motor);
            }
        }
    }
    return 0;
}

static bool input_callback(InputEvent* event, void* ctx) {
    furi_assert(ctx);
    GPIOSimpleMotor* motor = ctx;
    if(event->key == InputKeyBack) {
        furi_hal_bt_serial_set_event_callback(0, NULL, NULL);
        return false;
    } 
    
    if (event->key == InputKeyOk) {
        if (furi_hal_bt_is_active()) {
            FURI_LOG_D(TAG, "BT is working, hijacking the serial connection...");
            furi_hal_bt_start_advertising();
            furi_hal_bt_serial_set_event_callback(BT_SERIAL_BUFFER_SIZE, bt_serial_event_callback, motor);

            with_view_model(
                motor->view,
                GPIOSimpleMotorModel * model,
                { 
                    model->display_text_1 = "";
                    model->display_text_2 = "Ready ^_^";
                    model->display_text_3 = "";
                },
                true);

        } else {
            FURI_LOG_E(TAG, "Please, enable the Bluetooth and restart the app");

            with_view_model(
                motor->view,
                GPIOSimpleMotorModel * model,
                { 
                    model->display_text_1 = "Error:";
                    model->display_text_2 = "Bluetooth is not enabled";
                    model->display_text_3 = "";
                },
                true);
        }
    }
    return true;
}

static void draw_callback(Canvas* canvas, void* ctx) {
    furi_assert(ctx);
    GPIOSimpleMotorModel* app = ctx;
    canvas_draw_str_aligned(canvas, 64, 24, AlignCenter, AlignCenter, (char*)app->display_text_1);
    canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignCenter, (char*)app->display_text_2);
    canvas_draw_str_aligned(canvas, 64, 40, AlignCenter, AlignCenter, (char*)app->display_text_3);
}

GPIOSimpleMotor* gpio_simple_motor_alloc(FBP* fbp) {
    furi_assert(fbp);
    GPIOSimpleMotor* motor = malloc(sizeof(GPIOSimpleMotor));
    motor->view = view_alloc();
    motor->fbp = fbp;
    view_set_context(motor->view, motor);
    view_allocate_model(motor->view, ViewModelTypeLocking, sizeof(GPIOSimpleMotorModel));
    view_set_draw_callback(motor->view, draw_callback);
    view_set_input_callback(motor->view, input_callback);

    with_view_model(
        motor->view,
        GPIOSimpleMotorModel * model,
        { 
            model->display_text_1 = "Please, connect the";
            model->display_text_2 = "transistor base to pin A7!"; 
            model->display_text_3 = "Press OK to start"; 
        },
        true);

    return motor;
}

void gpio_simple_motor_free(GPIOSimpleMotor* motor) {
    furi_assert(motor);
    furi_hal_pwm_stop(DEFAULT_PWM_OUTPUT_ID);
    view_free(motor->view);
    free(motor);
}

View* gpio_simple_motor_get_view(GPIOSimpleMotor* motor) {
    furi_assert(motor);
    return motor->view;
}