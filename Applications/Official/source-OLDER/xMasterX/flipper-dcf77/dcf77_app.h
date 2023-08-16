#ifndef __ARHA_FLIPPERAPP_DEMO
#define __ARHA_FLIPPERAPP_DEMO

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_speaker.h>
#include <gui/gui.h>
#include <input/input.h>

#include <lib/subghz/receiver.h>
#include <lib/subghz/transmitter.h>
#include <lib/subghz/subghz_file_encoder_worker.h>
#include <notification/notification.h>
#include <notification/notification_messages.h>

#include "dcf77_util.h"

// the TAG is used for displaying a relevant prefix in logs. update it.
#define TAG "__ARHA_FLIPPERAPP"
#define TIMER_HZ 30
#define TIME_ZERO 24
#define TIME_ONE 27
#define LF_FREQ 77500 * 2
#define OUTPUT_PIN &gpio_ext_pc3
// #define TIME_ZERO 15
// #define TIME_ONE 5


uint8_t dcf77_message_buffer[8];
uint8_t dcf77_next_buffer[8];

typedef enum {
    KeyNone,
    KeyUp,
    KeyRight,
    KeyDown,
    KeyLeft,
    KeyOK
} KeyCode;


typedef enum {
    EventTimerTick,
    EventKeyPress,
} EventType;

typedef struct {
    EventType type;
    InputEvent input;
} AppEvent;

typedef struct {
    uint16_t len;
    KeyCode last_key;

    FuriTimer* _timer;
    FuriMessageQueue* _event_queue;

    int counter;

    uint8_t bit_number; // 0 - 59
    uint8_t bit_value;  // 0 or 1 for actual bits, 2 for end-of-minute marker
    uint8_t baseband_counter;   // 0 - 20, so we can generate 800 and 900 ms wide pulses (bit0 = 800ms = 16; bit1 = 900ms = 18; bit2 = 1000ms = 20)
    uint8_t* dcf77_message; // these are 8 bytes which encode, LSB, every bit in the DCF77 message. see set_dcf_message()
    uint8_t* next_message;

    bool buffer_swap_pending;
    bool debug_flag;

    uint8_t tx_minute;
    uint8_t tx_hour;
    uint8_t tx_day;
    uint8_t tx_month;
    uint8_t tx_year;
    uint8_t tx_dow;
} AppFSM;


const NotificationSequence seq_c_minor = {
        &message_note_c4,
        &message_delay_100,
        &message_sound_off,
        &message_delay_10,

        &message_note_ds4,
        &message_delay_100,
        &message_sound_off,
        &message_delay_10,

        &message_note_g4,
        &message_delay_100,
        &message_sound_off,
        &message_delay_10,

        &message_vibro_on,
        &message_delay_50,
        &message_vibro_off,
        NULL,
};

const char *dcf77_bitnames[] = {
        "Start minute",
        "Civil 1",
        "Civil 2",
        "Civil 3",
        "Civil 4",
        "Civil 5",
        "Civil 6",
        "Civil 7",
        "Civil 8",
        "Civil 9",
        "Civil 10",
        "Civil 11",
        "Civil 12",
        "Civil 13",
        "Civil 14",
        "Abnormal",
        "DST change",
        "UTC+02",
        "UTC+01",
        "Leap sec",
        "Start time",
        "Minutes 1",
        "Minutes 2",
        "Minutes 3",
        "Minutes 4",
        "Minutes 5",
        "Minutes 6",
        "Minutes 7",
        "Minutes P",
        "Hours 1",
        "Hours 2",
        "Hours 3",
        "Hours 4",
        "Hours 5",
        "Hours 6",
        "Hours P",
        "Day 1",
        "Day 2",
        "Day 3",
        "Day 4",
        "Day 5",
        "Day 6",
        "Weekday 1",
        "Weekday 2",
        "Weekday 4",
        "Month 1",
        "Month 2",
        "Month 3",
        "Month 4",
        "Month 5",
        "Year 1",
        "Year 2",
        "Year 3",
        "Year 4",
        "Year 5",
        "Year 6",
        "Year 7",
        "Year 8",
        "Date P",
        "End"
};

#endif