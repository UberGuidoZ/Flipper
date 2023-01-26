#pragma once

#include <furi_hal.h>
#include "../helpers/time.h"

/// @brief Options of pomodoro stages
typedef enum
{
    Work,
    Rest,
} PomodoroStage;

/// @brief State of the pomodoro timer
typedef struct
{
    PomodoroStage stage;
    uint32_t started_at_timestamp;
} FlippPomodoroState;

/// @brief Generates initial state
/// @param state - pointer to the state of pomorodo.
/// @returns A new pre-populated state for pomodoro timer
FlippPomodoroState *flipp_pomodoro__new();

/// @brief Destroys state of timer and it's dependencies
void flipp_pomodoro__destroy(FlippPomodoroState *state);

/// @brief Get remaining stage time.
/// @param state - pointer to the state of pomorodo.
/// @returns Time difference to the end of current stage
TimeDifference flipp_pomodoro__stage_remaining_duration(FlippPomodoroState *state);

/// @brief Label of transition to the next stage
/// @param state - pointer to the state of pomorodo.
/// @returns string with the label of the "skipp" button
char *flipp_pomodoro__next_stage_label(FlippPomodoroState *state);

/// @brief Check if current stage is expired
/// @param state - pointer to the state of pomorodo.
/// @returns expriations status - true means stage is expired
bool flipp_pomodoro__is_stage_expired(FlippPomodoroState *state);

/// @brief Rotate stage of the timer
/// @param state - pointer to the state of pomorodo.
void flipp_pomodoro__toggle_stage(FlippPomodoroState *state);
