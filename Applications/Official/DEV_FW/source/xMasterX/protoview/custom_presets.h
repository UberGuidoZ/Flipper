#include <cc1101.h>

static uint8_t protoview_subghz_tpms_async_regs[][2] = {
    /* GPIO GD0 */
    {CC1101_IOCFG0, 0x0D}, // GD0 as async serial data output/input

    /* Frequency Synthesizer Control */
    {CC1101_FSCTRL1, 0x06}, // IF = (26*10^6) / (2^10) * 0x06 = 152343.75Hz

    /* Packet engine */
    {CC1101_PKTCTRL0, 0x32}, // Async, continious, no whitening
    {CC1101_PKTCTRL1, 0x04},

    // // Modem Configuration
    {CC1101_MDMCFG0, 0x00},
    {CC1101_MDMCFG1, 0x02},
    {CC1101_MDMCFG2, 0x04}, // Format 2-FSK/FM, No preamble/sync, Disable (current optimized). Other code reading TPMS uses GFSK, but should be the same when in RX mode.
    {CC1101_MDMCFG3, 0x93}, // Data rate is 20kBaud
    {CC1101_MDMCFG4, 0x59}, // Rx bandwidth filter is 325 kHz
    {CC1101_DEVIATN, 0x41}, // Deviation 28.56 kHz

    /* Main Radio Control State Machine */
    {CC1101_MCSM0, 0x18}, // Autocalibrate on idle-to-rx/tx, PO_TIMEOUT is 64 cycles(149-155us)

    /* Frequency Offset Compensation Configuration */
    {CC1101_FOCCFG,
     0x16}, // no frequency offset compensation, POST_K same as PRE_K, PRE_K is 4K, GATE is off

    /* Automatic Gain Control */
    {CC1101_AGCCTRL0,
     0x91}, //10 - Medium hysteresis, medium asymmetric dead zone, medium gain ; 01 - 16 samples agc; 00 - Normal AGC, 01 - 8dB boundary
    {CC1101_AGCCTRL1,
     0x00}, // 0; 0 - LNA 2 gain is decreased to minimum before decreasing LNA gain; 00 - Relative carrier sense threshold disabled; 0000 - RSSI to MAIN_TARGET
    {CC1101_AGCCTRL2, 0x07}, // 00 - DVGA all; 000 - MAX LNA+LNA2; 111 - MAIN_TARGET 42 dB

    /* Wake on radio and timeouts control */
    {CC1101_WORCTRL, 0xFB}, // WOR_RES is 2^15 periods (0.91 - 0.94 s) 16.5 - 17.2 hours

    /* Frontend configuration */
    {CC1101_FREND0, 0x10}, // Adjusts current TX LO buffer
    {CC1101_FREND1, 0x56},

    /* End  */
    {0, 0},
};

