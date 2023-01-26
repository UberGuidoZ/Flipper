#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <stdlib.h>
#include <dolphin/dolphin.h>
#include <notification/notification.h>
#include <notification/notification_messages.h>

#include "dcf77_app.h"

uint8_t get_dcf_message_bit(uint8_t* message, uint8_t bit)
{
    if (bit == 59 || bit == 0)
    {
        return 0;
    }

    uint8_t byte_ = bit / 8;
    uint8_t bit_ = bit % 8;

    uint8_t bit_value = *(message+byte_) & (1 << (7-bit_));

    if (!!bit_value)
    {
        return 1;
    }
    return 0;
}

// should it still be const?
static void update_dcf77_message_from_rtc(AppFSM* app_fsm)
{
    FuriHalRtcDateTime dt;
    furi_hal_rtc_get_datetime(&dt);
    app_fsm->bit_number = dt.second;
    app_fsm->next_message = malloc(8);
    /*
    set_dcf_message(app_fsm->next_message, dt.minute, dt.hour,
                    dt.day, dt.month, (uint8_t)(dt.year % 100), dt.weekday,
                    false, false, false, false, 0x0000);
                    */

    //set_dcf_message(app_fsm->next_message, 41, 17, 27, 12, 22, 2, false, false, false, false, 0x0000);

    app_fsm->tx_hour = 17;
    app_fsm->tx_minute = 41;
    app_fsm->tx_day = 27;
    app_fsm->tx_month = 12;
    app_fsm->tx_year = 22;
    app_fsm->tx_dow = 2;


    app_fsm->tx_hour = dt.hour;
    app_fsm->tx_minute = dt.minute;
    app_fsm->tx_day = dt.day;
    app_fsm->tx_month = dt.month;
    app_fsm->tx_year = dt.year % 100;
    app_fsm->tx_dow = dt.weekday;

    set_dcf_message(app_fsm->next_message, app_fsm->tx_minute, app_fsm->tx_hour, app_fsm->tx_day, app_fsm->tx_month,
                    app_fsm->tx_year, app_fsm->tx_dow, false, false, false, false, 0x0000);
    app_fsm->buffer_swap_pending = true;
}

static void set_outputs(bool status, AppFSM* app_fsm)
{
    UNUSED(app_fsm);
    static bool last;
    if (last != status)
    {
        if (status)
        {
            furi_hal_light_set(LightRed, 0xFF);
            furi_hal_light_set(LightGreen, 0x1F);
            // furi_hal_speaker_start(50, 1);

        }
        else
        {
            furi_hal_light_set(LightRed | LightGreen | LightBlue, 0);
            // furi_hal_speaker_stop();
        }
    }
    last = status;
}

static void comparator_trigger_callback(bool level, void *comp_ctx) {
    UNUSED(comp_ctx);
    furi_hal_gpio_write(&gpio_ext_pa7, !level);
}


void gpio_init()
{
    furi_hal_gpio_write(OUTPUT_PIN, false);
    furi_hal_gpio_init(OUTPUT_PIN, GpioModeOutputPushPull, GpioPullNo, GpioSpeedVeryHigh);
}

void gpio_mark()
{
    furi_hal_gpio_write(OUTPUT_PIN, true);
}


void gpio_space()
{
    furi_hal_gpio_write(OUTPUT_PIN, false);
}

void gpio_deinit()
{
    furi_hal_gpio_init(OUTPUT_PIN, GpioModeAnalog, GpioPullNo, GpioSpeedVeryHigh);
}

void dcf77_lf_init(int freq, AppFSM* app_fsm)
{
    /* // this ends up doing
    // LL_TIM_SetAutoReload(FURI_HAL_RFID_READ_TIMER, period);
    // LL_TIM_OC_SetCompareCH1(FURI_HAL_RFID_READ_TIMER, period*duty_cycle);
    */

    furi_hal_rfid_tim_reset();
    furi_hal_gpio_init_simple(&gpio_ext_pa7, GpioModeOutputPushPull);
    furi_hal_rfid_comp_set_callback(comparator_trigger_callback, app_fsm);
    furi_hal_rfid_pins_read();
    furi_hal_rfid_tim_read(freq, 0.5);
}

void dcf77_mark(int freq)
{
        furi_hal_rfid_tim_reset();
        furi_hal_rfid_tim_read(freq, 0.5);
        furi_hal_rfid_comp_start();
        furi_hal_rfid_tim_read_start();
        /* --- */
        furi_hal_rfid_comp_stop();
        furi_hal_rfid_tim_read_stop();
}

void dcf77_space()
{
        furi_hal_rfid_comp_stop();
        furi_hal_rfid_tim_read_stop();
}

void dcf77_deinit()
{
    dcf77_space();
    furi_hal_rfid_comp_stop();
    furi_hal_rfid_comp_set_callback(NULL, NULL);
    furi_hal_gpio_init_simple(&gpio_ext_pa7, GpioModeAnalog);
    furi_hal_rfid_tim_read_stop();
    furi_hal_rfid_tim_reset();
    furi_hal_rfid_pins_reset();
}

static void render_callback(Canvas* const canvas, void* ctx)
{
    // const AppFSM* app_fsm = acquire_mutex((ValueMutex*)ctx, 25);
    AppFSM* app_fsm = acquire_mutex((ValueMutex*)ctx, 25);
    if(app_fsm == NULL) {
        return;
    }

    char buffer[64];
    uint8_t yoffset = 9;
    uint8_t bit_number = app_fsm->bit_number;
    uint8_t bit_value = app_fsm->bit_value;
    uint8_t underline_x = (bit_number/8) * 12 + 16;
    uint8_t byte_ = app_fsm->dcf77_message[bit_number/8];
    char display_bits[9] = "00000000";

    if (app_fsm->buffer_swap_pending)
    {
        memcpy(app_fsm->dcf77_message, app_fsm->next_message, 8);
        canvas_draw_str_aligned(canvas, 112, 10, AlignCenter, AlignBottom, "*");
        app_fsm->buffer_swap_pending = false;
    }


    canvas_draw_frame(canvas, 0, 0, 128, 64);
    canvas_set_font(canvas, FontPrimary);
    snprintf(buffer, 64, "%1x.%1x=%01x", bit_number/8, (bit_number%8), bit_value);
    FuriHalRtcDateTime dt;
    furi_hal_rtc_get_datetime(&dt);
    //canvas_draw_str_aligned(canvas, 64, 10, AlignCenter, AlignBottom, "DCF77 emulator");
    snprintf(buffer, 64, "%02d:%02d %02d.%02d.%02d", app_fsm->tx_hour, app_fsm->tx_minute, app_fsm->tx_day, app_fsm->tx_month,
             app_fsm->tx_year);
    canvas_draw_str_aligned(canvas, 64, 10, AlignCenter, AlignBottom, buffer);

    if (app_fsm->debug_flag)
    {
        canvas_draw_str_aligned(canvas, 8, 10, AlignCenter, AlignBottom, "D");
    }

    snprintf(buffer, 64, "%1x.%1x=%01x", bit_number/8, (bit_number%8), bit_value);
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 64, 24 + (9 - yoffset), AlignCenter, AlignBottom, buffer);

    snprintf(buffer, 64, "%s (bit %d)", dcf77_bitnames[bit_number], bit_value);
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 64, 34 + (9 - yoffset), AlignCenter, AlignBottom, buffer);


    
    snprintf(buffer, 64, "%02x%02x%02x%02x%02x%02x%02x%02x",
             app_fsm->dcf77_message[0], app_fsm->dcf77_message[1], app_fsm->dcf77_message[2], app_fsm->dcf77_message[3],
             app_fsm->dcf77_message[4], app_fsm->dcf77_message[5], app_fsm->dcf77_message[6], app_fsm->dcf77_message[7] );
    canvas_set_font(canvas, FontKeyboard);
    canvas_draw_str_aligned(canvas, 64, 44 + (9 - yoffset), AlignCenter, AlignBottom, buffer);
     

    canvas_draw_line(canvas, underline_x, 45, underline_x+10, 45);
    for(int i = 0; i < 8; i++)
    {
        if ((byte_ & (1 << (7-i))) != 0)
        {
            display_bits[i] = '1';
        }
    }

    canvas_set_font(canvas, FontKeyboard);
    snprintf(buffer, 64, "%02x=%s", byte_, display_bits);
    canvas_draw_str_aligned(canvas, 64, 54 + (9 - yoffset), AlignCenter, AlignBottom, buffer);  // whole message
    underline_x = (bit_number%8) * 6 + 49;
    canvas_draw_line(canvas, underline_x, 55, underline_x+4, 55);   // current byte

    release_mutex((ValueMutex*)ctx, app_fsm);
}

static void input_callback(InputEvent* input_event, FuriMessageQueue* event_queue) {
    furi_assert(event_queue);

    AppEvent event = {.type = EventKeyPress, .input = *input_event};
    furi_message_queue_put(event_queue, &event, FuriWaitForever);
}

static void timer_tick_callback(FuriMessageQueue* event_queue) {
    furi_assert(event_queue);

    AppEvent event = {.type = EventTimerTick};
    furi_message_queue_put(event_queue, &event, 0);
}

static void app_init(AppFSM* const app_fsm, FuriMessageQueue* event_queue) {

    app_fsm->counter = 0;
    app_fsm->dcf77_message = &dcf77_message_buffer[0];
    app_fsm->next_message = &dcf77_next_buffer[0];

    dcf77_lf_init(LF_FREQ, app_fsm);
    gpio_init();

    app_fsm->dcf77_message = malloc(8);  // message had 60 bits since 1959 and is unlikely to change
    // app_fsm->next_message = malloc(8);
    update_dcf77_message_from_rtc(app_fsm);
    app_fsm->baseband_counter = 0;

    app_fsm->_event_queue = event_queue;
    FuriTimer* timer = furi_timer_alloc(timer_tick_callback, FuriTimerTypePeriodic, app_fsm->_event_queue);
    furi_timer_start(timer, furi_kernel_get_tick_frequency() / TIMER_HZ);
    app_fsm->_timer = timer;
}

static void app_deinit(AppFSM* const app_fsm) {
    dcf77_deinit();
    gpio_deinit();
    app_fsm->buffer_swap_pending = false;
    free(app_fsm->dcf77_message);
    furi_timer_free(app_fsm->_timer);
}

static void on_timer_tick(AppFSM* app_fsm)
{
    static uint8_t last_second = 61;
    static bool last_output = false;
    bool output = true;

    FuriHalRtcDateTime dt;
    furi_hal_rtc_get_datetime(&dt);
    app_fsm->bit_number = dt.second;
    app_fsm->bit_value = get_dcf_message_bit(app_fsm->dcf77_message, app_fsm->bit_number);

    if (dt.second != last_second)
    {
        app_fsm->baseband_counter = 0;
        output = true;
    }
    else
    {
        app_fsm->baseband_counter++;
    }

    /*
    if (app_fsm->baseband_counter < 8)
    {
        app_fsm->debug_flag = true;
    }
    else
    {
        app_fsm->debug_flag = false;
    }*/

    if (dt.second == 0 && app_fsm->baseband_counter == 3)
    {
        update_dcf77_message_from_rtc(app_fsm);
    }

    if (dt.second == 59)
    {
        output = true;
    }
    else
    {
        if (app_fsm->baseband_counter > TIME_ZERO && app_fsm->bit_value == 0)
        {
            output = false;

        }
        else if (app_fsm->baseband_counter > TIME_ONE && app_fsm->bit_value == 1)
        {
            output = false;
        }
    }


    if (last_output != output)
    {
        set_outputs(output, app_fsm);
        if (!output)
        {
            dcf77_space();
            gpio_space();
        }
        else
        {
            dcf77_mark(LF_FREQ);
            gpio_mark();
        }
    }

    last_second = dt.second;
    last_output = output;
}


int32_t dcf77_app_main(void* p) {
    UNUSED(p);

    FuriMessageQueue* event_queue = furi_message_queue_alloc(8, sizeof(AppEvent));
    AppFSM* app_fsm = malloc(sizeof(AppFSM));
    app_init(app_fsm, event_queue);

    ValueMutex state_mutex;
    if(!init_mutex(&state_mutex, app_fsm, sizeof(AppFSM))) {
        FURI_LOG_E(TAG, "cannot create mutex\r\n");
        free(app_fsm);
        return 255;
    }

    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, render_callback, &state_mutex);
    view_port_input_callback_set(view_port, input_callback, event_queue);

    // Open GUI and register view_port
    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);
    NotificationApp* notification = furi_record_open(RECORD_NOTIFICATION);
    notification_message_block(notification, &sequence_display_backlight_enforce_on);


    if (furi_hal_speaker_acquire(500))
    {
        ;
    }


    DOLPHIN_DEED(DolphinDeedPluginGameStart);

    AppEvent event;
    for(bool processing = true; processing;) {
        FuriStatus event_status = furi_message_queue_get(event_queue, &event, 100);

        AppFSM* app_fsm = (AppFSM*)acquire_mutex_block(&state_mutex);

        if(event_status == FuriStatusOk) {
            // kepress events
            if(event.type == EventKeyPress) {
                if(event.input.type == InputTypePress) {
                    switch(event.input.key) {
                    case InputKeyUp:
                        app_fsm->last_key = KeyUp;
                        break;
                    case InputKeyDown:
                        app_fsm->last_key = KeyDown;
                        break;
                    case InputKeyRight:
                        app_fsm->last_key = KeyRight;
                        break;
                    case InputKeyLeft:
                        app_fsm->last_key = KeyLeft;
                        break;
                    case InputKeyOk:
                        app_fsm->last_key = KeyOK;
                        app_fsm->counter = -5;
                        break;
                    case InputKeyBack:
                        processing = false;
                        break;
                    default:
                        break;
                    }
                }
            // user events
            } else if(event.type == EventTimerTick) {
                FURI_CRITICAL_ENTER();
                on_timer_tick(app_fsm);
                FURI_CRITICAL_EXIT();
            }
        } else {
            // event timeout
        }

        view_port_update(view_port);
        release_mutex(&state_mutex, app_fsm);
    }
    furi_hal_speaker_release();
    notification_message_block(notification, &seq_c_minor);

    // Wait for all notifications to be played and return backlight to normal state
    app_deinit(app_fsm);

    notification_message_block(notification, &sequence_display_backlight_enforce_auto);

    view_port_enabled_set(view_port, false);
    gui_remove_view_port(gui, view_port);
    furi_record_close(RECORD_GUI);
    furi_record_close(RECORD_NOTIFICATION);
    view_port_free(view_port);
    furi_message_queue_free(event_queue);
    delete_mutex(&state_mutex);
    free(app_fsm);

    return 0;
}
