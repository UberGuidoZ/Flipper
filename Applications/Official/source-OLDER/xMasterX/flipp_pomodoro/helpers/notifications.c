#include <notification/notification_messages.h>

const NotificationSequence work_start_notification = {
    &message_display_backlight_on,

    &message_vibro_on,

    &message_note_b5,
    &message_delay_250,

    &message_note_d5,
    &message_delay_250,

    &message_sound_off,
    &message_vibro_off,

    &message_green_255,
    &message_delay_1000,
    &message_green_0,
    &message_delay_250,
    &message_green_255,
    &message_delay_1000,

    NULL,
};

const NotificationSequence rest_start_notification = {
    &message_display_backlight_on,

    &message_vibro_on,

    &message_note_d5,
    &message_delay_250,

    &message_note_b5,
    &message_delay_250,

    &message_sound_off,
    &message_vibro_off,

    &message_red_255,
    &message_delay_1000,
    &message_red_0,
    &message_delay_250,
    &message_red_255,
    &message_delay_1000,

    NULL,
};