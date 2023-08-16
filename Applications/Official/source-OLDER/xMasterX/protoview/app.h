/* Copyright (C) 2022-2023 Salvatore Sanfilippo -- All Rights Reserved
 * See the LICENSE file for information about the license. */

#pragma once

#include <furi.h>
#include <furi_hal.h>
#include <input/input.h>
#include <gui/gui.h>
#include <stdlib.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/submenu.h>
#include <gui/modules/variable_item_list.h>
#include <gui/modules/widget.h>
#include <notification/notification_messages.h>
#include <lib/subghz/subghz_setting.h>
#include <lib/subghz/subghz_worker.h>
#include <lib/subghz/receiver.h>
#include <lib/subghz/transmitter.h>
#include <lib/subghz/registry.h>
#include "app_buffer.h"

#define TAG "ProtoView"
#define PROTOVIEW_RAW_VIEW_DEFAULT_SCALE 100
#define BITMAP_SEEK_NOT_FOUND UINT32_MAX

#define DEBUG_MSG 1

typedef struct ProtoViewApp ProtoViewApp;

/* Subghz system state */
typedef enum {
    TxRxStateIDLE,
    TxRxStateRx,
    TxRxStateSleep,
} TxRxState;

/* Currently active view. */
typedef enum {
    ViewRawPulses,
    ViewInfo,
    ViewFrequencySettings,
    ViewModulationSettings,
    ViewLast, /* Just a sentinel to wrap around. */
} ProtoViewCurrentView;

typedef struct {
    const char *name;
    FuriHalSubGhzPreset preset;
    uint8_t *custom;
} ProtoViewModulation;

extern ProtoViewModulation ProtoViewModulations[]; /* In app_subghz.c */

/* This is the context of our subghz worker and associated thread.
 * It receives data and we get our protocol "feed" callback called
 * with the level (1 or 0) and duration. */
struct ProtoViewTxRx {
    SubGhzWorker* worker;       /* Our background worker. */
    SubGhzEnvironment* environment;
    SubGhzReceiver* receiver;
    TxRxState txrx_state; /* Receiving, idle or sleeping? */
};

typedef struct ProtoViewTxRx ProtoViewTxRx;

/* This stucture is filled by the decoder for specific protocols with the
 * informations about the message. ProtoView will display such information
 * in the message info view. */
#define PROTOVIEW_MSG_STR_LEN 32
typedef struct ProtoViewMsgInfo {
    char name[PROTOVIEW_MSG_STR_LEN]; /* Protocol name and version. */
    char raw[PROTOVIEW_MSG_STR_LEN]; /* Protocol specific raw representation.*/
    /* The following is what the decoder wants to show to user. Each decoder
     * can use the number of fileds it needs. */
    char info1[PROTOVIEW_MSG_STR_LEN]; /* Protocol specific info line 1. */
    char info2[PROTOVIEW_MSG_STR_LEN]; /* Protocol specific info line 2. */
    char info3[PROTOVIEW_MSG_STR_LEN]; /* Protocol specific info line 3. */
    uint64_t len;       /* Bits consumed from the stream. */
} ProtoViewMsgInfo;

struct ProtoViewApp {
    /* GUI */
    Gui *gui;
    ViewPort *view_port;     /* We just use a raw viewport and we render
                                everything into the low level canvas. */
    ProtoViewCurrentView current_view;  /* Active view ID. */
    FuriMessageQueue *event_queue;  /* Keypress events go here. */

    /* Radio related. */
    ProtoViewTxRx *txrx;     /* Radio state. */
    SubGhzSetting *setting;  /* A list of valid frequencies. */

    /* Generic app state. */
    int running;             /* Once false exists the app. */
    uint32_t signal_bestlen; /* Longest coherent signal observed so far. */
    bool signal_decoded;     /* Was the current signal decoded? */
    ProtoViewMsgInfo signal_info; /* Decoded message, if signal_decoded true. */

    /* Raw view apps state. */
    uint32_t us_scale;       /* microseconds per pixel. */
    uint32_t signal_offset;  /* Long press left/right panning in raw view. */

    /* Configuration view app state. */
    uint32_t frequency;      /* Current frequency. */
    uint8_t modulation;      /* Current modulation ID, array index in the
                                ProtoViewModulations table. */
};

typedef struct ProtoViewDecoder {
    const char *name;   /* Protocol name. */
    /* The decode function takes a buffer that is actually a bitmap, with
     * high and low levels represented as 0 and 1. The number of high/low
     * pulses represented by the bitmap is passed as the 'numbits' argument,
     * while 'numbytes' represents the total size of the bitmap pointed by
     * 'bits'. So 'numbytes' is mainly useful to pass as argument to other
     * functions that perform bit extraction with bound checking, such as
     * bitmap_get() and so forth. */
    bool (*decode)(uint8_t *bits, uint32_t numbytes, uint32_t numbits, ProtoViewMsgInfo *info);
} ProtoViewDecoder;

extern RawSamplesBuffer *RawSamples, *DetectedSamples;

/* app_radio.c */
void radio_begin(ProtoViewApp* app);
uint32_t radio_rx(ProtoViewApp* app);
void radio_idle(ProtoViewApp* app);
void radio_rx_end(ProtoViewApp* app);
void radio_sleep(ProtoViewApp* app);

/* signal.c */
uint32_t duration_delta(uint32_t a, uint32_t b);
void reset_current_signal(ProtoViewApp *app);
void scan_for_signal(ProtoViewApp *app);
bool bitmap_get(uint8_t *b, uint32_t blen, uint32_t bitpos);
void bitmap_set(uint8_t *b, uint32_t blen, uint32_t bitpos, bool val);
void bitmap_set_pattern(uint8_t *b, uint32_t blen, const char *pat);
void bitmap_invert_bytes_bits(uint8_t *p, uint32_t len);
bool bitmap_match_bits(uint8_t *b, uint32_t blen, uint32_t bitpos, const char *bits);
uint32_t bitmap_seek_bits(uint8_t *b, uint32_t blen, uint32_t startpos, uint32_t maxbits, const char *bits);
uint32_t convert_from_line_code(uint8_t *buf, uint64_t buflen, uint8_t *bits, uint32_t len, uint32_t offset, const char *zero_pattern, const char *one_pattern);

/* view_*.c */
void render_view_raw_pulses(Canvas *const canvas, ProtoViewApp *app);
void process_input_raw_pulses(ProtoViewApp *app, InputEvent input);
void render_view_settings(Canvas *const canvas, ProtoViewApp *app);
void process_input_settings(ProtoViewApp *app, InputEvent input);
void render_view_info(Canvas *const canvas, ProtoViewApp *app);
void process_input_info(ProtoViewApp *app, InputEvent input);

/* ui.c */
void canvas_draw_str_with_border(Canvas* canvas, uint8_t x, uint8_t y, const char* str, Color text_color, Color border_color);
