#include <furi.h>
#include <furi_hal.h>
#include "time.h"

const int TIME_SECONDS_IN_MINUTE = 60;
const int TIME_MINUTES_IN_HOUR = 60;

uint32_t time_now()
{
    return furi_hal_rtc_get_timestamp();
};

TimeDifference time_difference_seconds(uint32_t begin, uint32_t end)
{
    const uint32_t duration_seconds = end - begin;

    uint32_t minutes = (duration_seconds / TIME_MINUTES_IN_HOUR) % TIME_MINUTES_IN_HOUR;
    uint32_t seconds = duration_seconds % TIME_SECONDS_IN_MINUTE;

    return (TimeDifference){.total_seconds = duration_seconds, .minutes = minutes, .seconds = seconds};
};
