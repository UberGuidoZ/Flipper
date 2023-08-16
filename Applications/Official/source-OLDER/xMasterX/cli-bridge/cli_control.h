#pragma once

#include <furi.h>
#include <furi_hal.h>
extern void latch_tx_handler();
extern void unlatch_tx_handler(bool persist);
extern FuriStreamBuffer* tx_stream;
extern FuriStreamBuffer* rx_stream;