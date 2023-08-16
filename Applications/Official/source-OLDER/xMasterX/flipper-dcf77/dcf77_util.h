#ifndef __arha_dcf77util_h
#define __arha_dcf77util_h

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

void set_dcf_message(uint8_t* dest, uint8_t minute, uint8_t hour,
                     uint8_t day, uint8_t month, uint8_t year, uint8_t dow,
                     bool dst, bool predst, bool abnormal, bool leap, uint16_t civbits);

#ifndef BBIT
#define BBIT(B,shift) (!!(B & (1 << shift)))
#endif



#endif