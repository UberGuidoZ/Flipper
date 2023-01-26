#pragma once
#include <furi.h>

typedef enum {
    Device,
    Magnitude,
    MagnitudeTimeInterval,
    MagnitudeSpeed,
    Unknown,
} TCodeCommandType;

typedef enum {
    DeviceIdentification,
    TCodeVersion,
    ListAxesAndUserRangePreferences,
    Stop,
} DeviceCommand;

typedef enum {
    Linear,
    Rotate,
    Vibrate,
    Auxiliary,
} TCodeCommandMotionType;

typedef struct {
    TCodeCommandMotionType motion_type;
    uint8_t channel_id;
    float magnitude;
} TCodeMagnitudeCommand;

typedef struct {
    TCodeCommandMotionType motion_type;
    uint8_t channel_id;
    float magnitude;
    uint16_t time_interval_milliseconds;
} TCodeMagnitudeTimeIntervalCommand;

typedef struct {
    TCodeCommandMotionType motion_type;
    uint8_t channel_id;
    float magnitude;
    uint16_t speed_per_hundred_milliseconds;
} TCodeMagnitudeSpeedCommand;


typedef struct {
    TCodeCommandType command_type;
    union {
        DeviceCommand device_command;
        TCodeMagnitudeCommand magnitude_command;
        TCodeMagnitudeTimeIntervalCommand magnitude_time_interval_command;
        TCodeMagnitudeSpeedCommand magnitude_speed_command;
    } data;
} TCodeCommand;

typedef struct {
    uint16_t size;
    TCodeCommand *commands;
} TCodeCommandArray;


TCodeCommandArray tcode_decode(uint8_t *buffer, uint16_t size);