#include "dcf77_util.h"

void set_dcf_message(uint8_t* dest, uint8_t minute, uint8_t hour,
                     uint8_t day, uint8_t month, uint8_t year, uint8_t dow,
                     bool dst, bool predst, bool abnormal, bool leap, uint16_t civbits)
{
    uint8_t bcd_minute = 0;
    uint8_t bcd_hour = 0;
    uint8_t bcd_day = 0;
    uint8_t bcd_month = 0;
    uint8_t bcd_year = 0;

    /*  converting binary to bcd (which looks like hex, without the a-f digits)
        this is very common in old-school electronics */
    bcd_minute =    ((minute / 10) << 4)    | (minute % 10);
    bcd_hour =      ((hour / 10) << 4)      | (hour % 10);
    bcd_day =       ((day / 10) << 4)       | (day % 10);
    bcd_month =     ((month / 10) << 4)     | (month % 10);
    bcd_year =      ((year / 10) << 4)      | (year % 10);

    /*  parity bits */
    bool p_minute = 1;
    bool p_hour = 1;
    bool p_date = 1;

    for (int i = 0; i < 8; i++)
    {
        p_minute ^= !!(bcd_minute & (1 << i));
        p_hour   ^= !!(bcd_hour & (1 << i));

        p_date = !!(bcd_day & (1 << i));
        p_date = !!(bcd_month & (1 << i));
        p_date = !!(bcd_year & (1 << i));
        p_date = !!((dow & 0x07) & (1 << i));
    }

    dest[0] = (uint8_t)(civbits >> 7);
    dest[1] = ((civbits & 0x7F) << 1) | abnormal;

    // d2: 16-23;  16=adst, 17=dst, 18=!dsst, 19=leap, 20=1, 21-22-23 are minute lsb 1, 2, 3
    dest[2] = (predst << 7) | (dst << 6) | (!dst << 5) | (leap << 4) | (1 << 3);
    dest[2]|= BBIT(bcd_minute, 0) << 2;
    dest[2]|= BBIT(bcd_minute, 1) << 1;
    dest[2]|= BBIT(bcd_minute, 2) << 0;
    //dest[2]|= (bcd_minute & (1 << 2));

    dest[3] =0;
    // DCF bit 24 is byte 3, bit 7
    dest[3]|= BBIT(bcd_minute, 3) << 7;
    // DCF bit 25 is byte 3, bit 6
    dest[3]|= BBIT(bcd_minute, 4) << 6;
    // DCF bit 26 is byte 3, bit 5
    dest[3]|= BBIT(bcd_minute, 5) << 5;
    // DCF bit 27 is byte 3, bit 4
    dest[3]|= BBIT(bcd_minute, 6) << 4;
    // DCF bit 28 is byte 3, bit 3  (E parity, minutes)
    dest[3]|= p_minute << 3;
    // DCF bit 29 is byte 3, bit 2
    dest[3]|= BBIT(bcd_hour, 0) << 2;
    // DCF bit 30 is byte 3, bit 1
    dest[3]|= BBIT(bcd_hour, 1) << 1;
    // DCF bit 31 is byte 3, bit 0
    dest[3]|= BBIT(bcd_hour, 2) << 0;

    // d3: 24-31, 24-27 min lsb 3-6; 28=p, 29-31: hours lsb 0-2
    //dest[3] = bcd_minute << 3 | (p_minute << 3);

    // d4: 32-39
    dest[4] =0;
    // DCF bit 32 is byte 4, bit 7
    dest[4]|= BBIT(bcd_hour, 3) << 7;
    // DCF bit 33 is byte 4, bit 6
    dest[4]|= BBIT(bcd_hour, 4) << 6;
    // DCF bit 34 is byte 4, bit 5
    dest[4]|= BBIT(bcd_hour, 5)  << 5;
    // DCF bit 35 is byte 4, bit 4
    dest[4]|= p_hour << 4;
    // DCF bit 36 is byte 4, bit 3
    dest[4]|= BBIT(bcd_day, 0) << 3;
    // DCF bit 37 is byte 4, bit 2
    dest[4]|= BBIT(bcd_day, 1) << 2;
    // DCF bit 38 is byte 4, bit 1
    dest[4]|= BBIT(bcd_day, 2) << 1;
    // DCF bit 39 is byte 4, bit 0
    dest[4]|= BBIT(bcd_day, 3) << 0;

    // d5: 40-47, 40-41: day lsb 4-5, 42-44: dow; 45-47: month lsb 0-2
    dest[5] =0;
    // DCF bit 40 is byte 5, bit 7
    dest[5]|= BBIT(bcd_day, 4) << 7;
    // DCF bit 41 is byte 5, bit 6
    dest[5]|= BBIT(bcd_day, 5) << 6;
    // DCF bit 42 is byte 5, bit 5
    dest[5]|= BBIT(dow, 0) << 5;
    // DCF bit 43 is byte 5, bit 4
    dest[5]|= BBIT(dow, 1) << 4;
    // DCF bit 44 is byte 5, bit 3
    dest[5]|= BBIT(dow, 2) << 3;
    // DCF bit 45 is byte 5, bit 2
    dest[5]|= BBIT(bcd_month, 0) << 2;
    // DCF bit 46 is byte 5, bit 1
    dest[5]|= BBIT(bcd_month, 1) << 1;
    // DCF bit 47 is byte 5, bit 0
    dest[5]|= BBIT(bcd_month, 2) << 0;


    // d6: 48-55; 48-49 month lsb 3-4; 50-55: year lsb 0-5
    dest[6] =0;
    // DCF bit 48 is byte 6, bit 7
    dest[6]|= BBIT(bcd_month, 3) << 7;
    // DCF bit 49 is byte 6, bit 6
    dest[6]|= BBIT(bcd_month, 4) << 6;
    // DCF bit 50 is byte 6, bit 5
    dest[6]|= BBIT(bcd_year, 0) << 5;
    // DCF bit 51 is byte 6, bit 4
    dest[6]|= BBIT(bcd_year, 1) << 4;
    // DCF bit 52 is byte 6, bit 3
    dest[6]|= BBIT(bcd_year, 2) << 3;
    // DCF bit 53 is byte 6, bit 2
    dest[6]|= BBIT(bcd_year, 3) << 2;
    // DCF bit 54 is byte 6, bit 1
    dest[6]|= BBIT(bcd_year, 4) << 1;
    // DCF bit 55 is byte 6, bit 0
    dest[6]|= BBIT(bcd_year, 5) << 0;


    // d7: 56-63; 56-57: year lsb 6-7; 58: date parity; 59: special minute marker
    dest[7] =0;
    // DCF bit 56 is byte 7, bit 7
    dest[7]|= BBIT(bcd_year, 6) << 7;
    // DCF bit 57 is byte 7, bit 6
    dest[7]|= BBIT(bcd_year, 7) << 6;
    // DCF bit 58 is byte 7, bit 5
    dest[7]|= p_date << 5;
}