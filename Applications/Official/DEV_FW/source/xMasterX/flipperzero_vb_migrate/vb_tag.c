// SPDX-License-Identifier: GPL-3.0-or-later
//
// VB Lab Migration Assistant for Flipper Zero
// Copyright (C) 2022  cyanic
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <stdlib.h>
#include "vb_tag.h"

#define VB_NAME_VBDM "VB Digital Monster"
#define VB_NAME_VBV "VB Digivice -V-"
#define VB_NAME_VBC "VB Characters"
#define VB_NAME_VH "Vital Hero"
#define VB_NAME_VBBE "Vital Bracelet BE"

#define VB_NAME_VBDM_SHORT "VBDM"
#define VB_NAME_VBV_SHORT "VBV"
#define VB_NAME_VBC_SHORT "VBC"
#define VB_NAME_VH_SHORT "VH"
#define VB_NAME_VBBE_SHORT "VBBE"

struct BantBlockCommon {
    uint32_t magic;
    // Note: this should be big endian, but for convenience, we'll treat them as little endian
    uint16_t item_id;
    uint16_t item_no;
    uint8_t status;
} __attribute__((packed));

struct BantBlockVb {
    uint8_t dim_no;
    uint8_t operation;
    uint8_t reserved;
    uint8_t app_flag;
    uint8_t padding[3];
} __attribute__((packed));

struct BantBlockBe {
    uint8_t operation;
    uint16_t dim_no;
    uint8_t app_flag;
    uint8_t nonce[3];
} __attribute__((packed));

struct BantBlock {
    struct BantBlockCommon common;
    union {
        struct BantBlockVb vb;
        struct BantBlockBe be;
    };
} __attribute__((packed));

static const VbTagProduct vb_tag_valid_products[] = {
    {.item_id = 0x0200,
     .item_no = 0x0100,
     .name = VB_NAME_VBDM,
     .short_name = VB_NAME_VBDM_SHORT,
     .type = VbTagTypeVBDM},
    {.item_id = 0x0200,
     .item_no = 0x0200,
     .name = VB_NAME_VBDM,
     .short_name = VB_NAME_VBDM_SHORT,
     .type = VbTagTypeVBDM},
    {.item_id = 0x0200,
     .item_no = 0x0300,
     .name = VB_NAME_VBDM,
     .short_name = VB_NAME_VBDM_SHORT,
     .type = VbTagTypeVBDM},
    {.item_id = 0x0200,
     .item_no = 0x0400,
     .name = VB_NAME_VBV,
     .short_name = VB_NAME_VBV_SHORT,
     .type = VbTagTypeVBV},
    {.item_id = 0x0200,
     .item_no = 0x0500,
     .name = VB_NAME_VBV,
     .short_name = VB_NAME_VBV_SHORT,
     .type = VbTagTypeVBV},
    {.item_id = 0x0200,
     .item_no = 0x0600,
     .name = VB_NAME_VH,
     .short_name = VB_NAME_VH_SHORT,
     .type = VbTagTypeVH},
    {.item_id = 0x0300,
     .item_no = 0x0100,
     .name = VB_NAME_VBC,
     .short_name = VB_NAME_VBC_SHORT,
     .type = VbTagTypeVBC},
    {.item_id = 0x0400,
     .item_no = 0x0100,
     .name = VB_NAME_VBBE,
     .short_name = VB_NAME_VBBE_SHORT,
     .type = VbTagTypeVBBE},
};

static const char* vb_tag_type_names[] = {
    "Unknown",
    VB_NAME_VBDM_SHORT,
    VB_NAME_VBV_SHORT,
    VB_NAME_VBC_SHORT,
    VB_NAME_VH_SHORT,
    VB_NAME_VBBE_SHORT,
};

BantBlock* vb_tag_get_bant_block(NfcDeviceData* dev) {
    return (BantBlock*)&dev->mf_ul_data.data[16];
}

const VbTagProduct* vb_tag_find_product(const BantBlock* bant) {
    for(size_t i = 0; i < COUNT_OF(vb_tag_valid_products); ++i) {
        const VbTagProduct* product = &vb_tag_valid_products[i];
        if(bant->common.item_id == product->item_id && bant->common.item_no == product->item_no)
            return product;
    }

    return NULL;
}

bool vb_tag_validate_product(NfcDeviceData* dev) {
    // Must be NTAG I2C Plus 1k
    if(dev->protocol != NfcDeviceProtocolMifareUl) return false;
    if(dev->mf_ul_data.type != MfUltralightTypeNTAGI2CPlus1K) return false;
    // Must match one of the known product IDs
    BantBlock* bant = vb_tag_get_bant_block(dev);
    if(bant->common.magic != BANT_MAGIC) return false;
    return vb_tag_find_product(bant) != NULL;
}

VbTagStatus vb_tag_get_status(const BantBlock* bant) {
    return bant->common.status;
}

void vb_tag_set_status(BantBlock* bant, VbTagStatus status) {
    bant->common.status = status;
}

VbTagOperation vb_tag_get_operation(const BantBlock* bant) {
    return vb_tag_is_vbbe(bant) ? bant->be.operation : bant->vb.operation;
}

void vb_tag_set_operation(BantBlock* bant, VbTagOperation operation) {
    uint8_t* p_op = vb_tag_is_vbbe(bant) ? &bant->be.operation : &bant->vb.operation;
    *p_op = operation;
}

const VbTagProduct* vb_tag_get_default_product(VbTagType type) {
    // IMPORTANT: Update when vb_tag_valid_products changes
    switch(type) {
    case VbTagTypeVBDM:
        return &vb_tag_valid_products[2];
    case VbTagTypeVBV:
        return &vb_tag_valid_products[4];
    case VbTagTypeVBC:
        return &vb_tag_valid_products[6];
    case VbTagTypeVH:
        return &vb_tag_valid_products[5];
    case VbTagTypeVBBE:
        return &vb_tag_valid_products[7];

    default:
        return NULL;
    }
}

void vb_tag_set_item_id_no(BantBlock* bant, const VbTagProduct* product) {
    bant->common.item_id = product->item_id;
    bant->common.item_no = product->item_no;
}

const char* vb_tag_get_tag_type_name(VbTagType type) {
    if(type < VbTagTypeMax) {
        return vb_tag_type_names[type];
    } else {
        return NULL;
    }
}

bool vb_tag_get_app_flag(const BantBlock* bant) {
    uint8_t app_flag = vb_tag_is_vbbe(bant) ? bant->be.app_flag : bant->vb.app_flag;
    return app_flag == 1;
}

void vb_tag_set_app_flag(BantBlock* bant, bool value) {
    uint8_t* app_flag = vb_tag_is_vbbe(bant) ? &bant->be.app_flag : &bant->vb.app_flag;
    *app_flag = value ? 1 : 0xff;
}

// Lookup is expensive, let's check tag ID directly
bool vb_tag_is_vbbe(const BantBlock* bant) {
    return bant->common.item_id == 0x0400;
}

uint32_t vb_tag_get_nonce(const BantBlock* bant) {
    if(vb_tag_is_vbbe(bant)) {
        return (bant->be.nonce[0] << 16) | (bant->be.nonce[1] << 8) | bant->be.nonce[2];
    } else {
        return 0;
    }
}

void vb_tag_set_nonce(BantBlock* bant, uint32_t value) {
    if(vb_tag_is_vbbe(bant)) {
        uint8_t* nonce = bant->be.nonce;
        nonce[0] = value >> 16;
        nonce[1] = value >> 8;
        nonce[2] = value;
    }
}

void vb_tag_set_random_nonce(BantBlock* bant) {
    if(vb_tag_is_vbbe(bant)) {
        uint32_t orig_nonce = vb_tag_get_nonce(bant);
        uint32_t new_nonce;
        do {
            new_nonce = rand() & 0xffffff;
            // Original nonce is generated as ((rand() % 0xffff) << 8) | (rand() % 0xff),
            // so don't inclue 0xffff** and 0x****ff as valid values
            if((new_nonce & 0xff) == 0xff) --new_nonce;
            if(new_nonce >= 0xffff00) new_nonce -= 0x100;
        } while(new_nonce == 0 || new_nonce == orig_nonce);
        vb_tag_set_nonce(bant, new_nonce);
    }
}
