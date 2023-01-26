/* Renault tires TPMS. Usually 443.92 Mhz FSK.
 *
 * Preamble + marshal-encoded bits. 9 Bytes in total if we don't
 * count the preamble. */

#include "../app.h"

#define USE_TEST_VECTOR 0
static const char *test_vector =
    "10101010" "10101010" "10101010" "10101001" // Preamble + sync.

    /* The following is marshal encoded, so each two characters are
     * actaully one bit. 01 = 1, 10 = 0. */
    "010110010110" // Flags.
    "10011001101010011001" // Pressure, multiply by 0.75 to obtain kpa.
                           // 244 kpa here.
    "1010010110011010"  // Temperature, subtract 30 to obtain celsius. 22C here.
    "1001010101101001"
    "0101100110010101"
    "1001010101100110"  // Tire ID. 0x7AD779 here.
    "0101010101010101"
    "0101010101010101"  // Two FF bytes (usually). Unknown.
    "0110010101010101"; // CRC8 with (poly 7, initialization 0).

static bool decode(uint8_t *bits, uint32_t numbytes, uint32_t numbits, ProtoViewMsgInfo *info) {

    if (USE_TEST_VECTOR) { /* Test vector to check that decoding works. */
        bitmap_set_pattern(bits,numbytes,test_vector);
        numbits = strlen(test_vector);
    }

    if (numbits < 13*8) return false;

    const char *sync_pattern = "10101010" "10101010" "10101010" "10101001";
    uint64_t off = bitmap_seek_bits(bits,numbytes,0,numbits,sync_pattern);
    if (off == BITMAP_SEEK_NOT_FOUND) return false;
    FURI_LOG_E(TAG, "Renault TPMS preamble+sync found");

    off += 32; /* Skip preamble. */

    uint8_t raw[9];
    uint32_t decoded =
        convert_from_line_code(raw,sizeof(raw),bits,numbytes,off,
            "10","01"); /* Manchester. */
    FURI_LOG_E(TAG, "Renault TPMS decoded bits: %lu", decoded);

    if (decoded < 8*9) return false; /* Require the full 9 bytes. */

    float kpa = 0.75 *((uint32_t)((raw[0]&3)<<8) | raw[1]);
    int temp = raw[2]-30;

    snprintf(info->name,sizeof(info->name),"%s","Renault TPMS");
    snprintf(info->raw,sizeof(info->raw),"%02X%02X%02X%02X%02X%02X%02X%02X%02X",
        raw[0],raw[1],raw[2],raw[3],raw[4],raw[5],
        raw[6],raw[7],raw[8]);
    snprintf(info->info1,sizeof(info->info1),"Pressure %.2f kpa", (double)kpa);
    snprintf(info->info2,sizeof(info->info2),"Temperature %d C", temp);
    return true;
}

ProtoViewDecoder RenaultTPMSDecoder = {
    "Renault TPMS", decode
};
