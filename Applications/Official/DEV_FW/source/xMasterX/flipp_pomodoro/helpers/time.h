#pragma once

#include <furi.h>
#include <furi_hal.h>

extern const int TIME_SECONDS_IN_MINUTE;
extern const int TIME_MINUTES_IN_HOUR;

/// @brief Container for a time period
typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint32_t total_seconds;
} TimeDifference;

/// @brief Time by the moment of calling
/// @return A timestamp(seconds percision)
uint32_t time_now();

/// @brief Calculates difference between two provided timestamps
/// @param begin - start timestamp of the period
/// @param end - end timestamp of the period to measure
/// @return TimeDifference struct
TimeDifference time_difference_seconds(uint32_t begin, uint32_t end);
