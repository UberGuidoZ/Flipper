#include <furi.h>
#include <furi_hal.h>
#include "../helpers/time.h"
#include "flipp_pomodoro.h"

char *next_stage_label[] = {
    [Work] = "Get Rest",
    [Rest] = "Start Work",
};

const PomodoroStage stage_rotaion_map[] = {
    [Work] = Rest,
    [Rest] = Work,
};

const PomodoroStage default_stage = Work;

void flipp_pomodoro__toggle_stage(FlippPomodoroState *state)
{
    furi_assert(state);
    state->stage = stage_rotaion_map[state->stage];
    state->started_at_timestamp = time_now();
};

char *flipp_pomodoro__next_stage_label(FlippPomodoroState *state)
{
    furi_assert(state);
    return next_stage_label[state->stage];
};

void flipp_pomodoro__destroy(FlippPomodoroState *state)
{
    furi_assert(state);
    free(state);
};

uint32_t flipp_pomodoro__current_stage_total_duration(FlippPomodoroState *state)
{
    const int32_t stage_duration_seconds_map[] = {
        [Work] = 25 * TIME_SECONDS_IN_MINUTE,
        [Rest] = 5 * TIME_SECONDS_IN_MINUTE,
    };

    return stage_duration_seconds_map[state->stage];
};

uint32_t flipp_pomodoro__stage_expires_timestamp(FlippPomodoroState *state)
{
    return state->started_at_timestamp + flipp_pomodoro__current_stage_total_duration(state);
};

TimeDifference flipp_pomodoro__stage_remaining_duration(FlippPomodoroState *state)
{
    const uint32_t stage_ends_at = flipp_pomodoro__stage_expires_timestamp(state);
    return time_difference_seconds(time_now(), stage_ends_at);
};

bool flipp_pomodoro__is_stage_expired(FlippPomodoroState *state)
{
    const uint32_t expired_by = flipp_pomodoro__stage_expires_timestamp(state);
    const uint8_t seamless_change_span_seconds = 1;
    return (time_now() - seamless_change_span_seconds) >= expired_by;
};

FlippPomodoroState *flipp_pomodoro__new()
{
    FlippPomodoroState *state = malloc(sizeof(FlippPomodoroState));
    const uint32_t now = time_now();
    state->started_at_timestamp = now;
    state->stage = default_stage;
    return state;
};