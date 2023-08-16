#include "internal.h"
#include "../fbp.h"

static const uint16_t BT_SERIAL_BUFFER_SIZE = 128;

struct FlipperVibrator {
    View* view;
    FBP* fbp;
};

typedef struct {
    char* display_text;
} FlipperVibratorModel;


static void process_general_command(TCodeCommand command) {
    if (command.command_type == Magnitude && command.data.magnitude_command.motion_type == Vibrate && command.data.magnitude_command.channel_id == 0) {
        furi_hal_vibro_on(command.data.magnitude_command.magnitude > 0.1f);
        return;
    }

    if (command.command_type == MagnitudeSpeed && command.data.magnitude_speed_command.motion_type == Vibrate && command.data.magnitude_speed_command.channel_id == 0) {
        furi_hal_vibro_on(command.data.magnitude_speed_command.magnitude > 0.1f);
        return;
    }

    if (command.command_type == MagnitudeTimeInterval && command.data.magnitude_time_interval_command.motion_type == Vibrate && command.data.magnitude_time_interval_command.channel_id == 0) {
        furi_hal_vibro_on(command.data.magnitude_time_interval_command.magnitude > 0.1f);
        return;
    }
}


static uint16_t bt_serial_event_callback(SerialServiceEvent event, void* context) {
    furi_assert(context);
    FlipperVibrator* flipper_vibrator = context;
    UNUSED(flipper_vibrator);

    if(event.event == SerialServiceEventTypeDataReceived) {
        FURI_LOG_D(TAG, "SerialServiceEventTypeDataReceived");
        FURI_LOG_D(TAG, "Size: %u", event.data.size);
        FURI_LOG_D(TAG, "Data: ");
        for (size_t i = 0; i < event.data.size; i++)
        {
            printf("%X ", event.data.buffer[i]);
        }
        printf("\r\n");

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
                process_general_command(current_command);
            }
        }
    }
    return 0;
}

static bool input_callback(InputEvent* event, void* ctx) {
    furi_assert(ctx);
    FlipperVibrator* flipper_vibrator = ctx;
    if(event->key == InputKeyBack) {
        furi_hal_bt_serial_set_event_callback(0, NULL, NULL);
        return false;
    } 
    
    if (event->key == InputKeyOk) {
        if (furi_hal_bt_is_active()) {
            FURI_LOG_D(TAG, "BT is working, hijacking the serial connection...");
            furi_hal_bt_start_advertising();
            furi_hal_bt_serial_set_event_callback(BT_SERIAL_BUFFER_SIZE, bt_serial_event_callback, flipper_vibrator);

            with_view_model(
                flipper_vibrator->view,
                FlipperVibratorModel * model,
                { model->display_text = "Ready ^_^"; },
                true);

        } else {
            FURI_LOG_E(TAG, "Please, enable the Bluetooth and restart the app");

            with_view_model(
                flipper_vibrator->view,
                FlipperVibratorModel * model,
                { model->display_text = "Error: Bluetooth not enabled"; },
                true);
        }
    }
    return true;
}

static void draw_callback(Canvas* canvas, void* ctx) {
    furi_assert(ctx);
    FlipperVibratorModel* app = ctx;
    canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignCenter, (char*)app->display_text);
}

FlipperVibrator* flipper_vibrator_alloc(FBP* fbp) {
    furi_assert(fbp);
    FlipperVibrator* flipper_vibrator = malloc(sizeof(FlipperVibrator));
    flipper_vibrator->view = view_alloc();
    flipper_vibrator->fbp = fbp;
    view_set_context(flipper_vibrator->view, flipper_vibrator);
    view_allocate_model(flipper_vibrator->view, ViewModelTypeLocking, sizeof(FlipperVibratorModel));
    view_set_draw_callback(flipper_vibrator->view, draw_callback);
    view_set_input_callback(flipper_vibrator->view, input_callback);

    with_view_model(
        flipper_vibrator->view,
        FlipperVibratorModel * model,
        { model->display_text = "Press OK to start"; },
        true);

    return flipper_vibrator;
}

void flipper_vibrator_free(FlipperVibrator* flipper_vibrator) {
    furi_assert(flipper_vibrator);
    view_free(flipper_vibrator->view);
    free(flipper_vibrator);
}

View* flipper_vibrator_get_view(FlipperVibrator* flipper_vibrator) {
    furi_assert(flipper_vibrator);
    return flipper_vibrator->view;
}