#pragma once

#include <gui/view.h>
#include "../modules/flipp_pomodoro.h"

typedef struct FlippPomodoroTimerView FlippPomodoroTimerView;

typedef void (*FlippPomodoroTimerViewInputCb)(void *context);

FlippPomodoroTimerView *flipp_pomodoro_view_timer_alloc();

View *flipp_pomodoro_view_timer_get_view(FlippPomodoroTimerView *timer);

void flipp_pomodoro_view_timer_free(FlippPomodoroTimerView *timer);

void flipp_pomodoro_view_timer_set_state(View *view, FlippPomodoroState *state);

void flipp_pomodoro_view_timer_set_on_right_cb(FlippPomodoroTimerView *timer, FlippPomodoroTimerViewInputCb right_cb, void *right_cb_ctx);
