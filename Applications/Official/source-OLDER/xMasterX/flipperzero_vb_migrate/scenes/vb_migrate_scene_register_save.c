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

#include <notification/notification_messages.h>

#include "../vb_migrate_i.h"
#include "../vb_tag.h"

// This thing doesn't know what a FuriString is
#include <gui/modules/validators.h>

typedef enum {
    RegisterSaveEventTextInput,
} RegisterSaveEvent;

static void vb_migrate_scene_register_save_text_input_callback(void* context) {
    VbMigrate* inst = context;

    view_dispatcher_send_custom_event(inst->view_dispatcher, RegisterSaveEventTextInput);
}

void vb_migrate_scene_register_save_on_enter(void* context) {
    VbMigrate* inst = context;

    // Setup view
    TextInput* text_input = inst->text_input;
    text_input_set_header_text(text_input, "Name this Vital Bracelet");
    text_input_set_result_callback(
        text_input,
        vb_migrate_scene_register_save_text_input_callback,
        inst,
        inst->text_store,
        VB_MIGRATE_MAX_DEV_NAME_LENGTH,
        true);

    // Set default name
    FuriString* temp_str;
    NfcDeviceData* dev_data = &inst->nfc_dev->dev_data;
    BantBlock* bant = vb_tag_get_bant_block(dev_data);
    const VbTagProduct* prod = vb_tag_find_product(bant);
    temp_str = furi_string_alloc_printf("%s_", prod->short_name);
    for(size_t i = 0; i < dev_data->nfc_data.uid_len; ++i) {
        furi_string_cat_printf(temp_str, "%02x", dev_data->nfc_data.uid[i]);
    }
    vb_migrate_text_store_set(inst, furi_string_get_cstr(temp_str));
    furi_string_free(temp_str);

    // We're validating whether folder exists
    ValidatorIsFile* validator_is_file =
        validator_is_file_alloc_init(VB_MIGRATE_FOLDER, "/" VB_MIGRATE_TEMPLATE_NAME, NULL);
    text_input_set_validator(text_input, validator_is_file_callback, validator_is_file);

    view_dispatcher_switch_to_view(inst->view_dispatcher, VbMigrateViewTextInput);
    notification_message(inst->notifications, &sequence_set_green_255);
}

bool vb_migrate_scene_register_save_on_event(void* context, SceneManagerEvent event) {
    VbMigrate* inst = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == RegisterSaveEventTextInput) {
            if(strlen(inst->text_store) != 0) {
                vb_migrate_show_loading_popup(inst, true);
                if(vb_migrate_save_nfc(inst, inst->text_store, VB_MIGRATE_TEMPLATE_NAME)) {
                    inst->num_captured = 0;

                    BantBlock* bant = vb_tag_get_bant_block(&inst->nfc_dev->dev_data);
                    const VbTagProduct* product = vb_tag_find_product(bant);
                    inst->orig_product = product;
                    if(product) {
                        inst->orig_type = product->type;
                    } else {
                        inst->orig_type = VbTagTypeUnknown;
                    }
                    inst->override_type = inst->orig_type;

                    // Go to success
                    scene_manager_next_scene(inst->scene_manager, VbMigrateSceneSaveSuccess);
                } else {
                    // Otherwise just stay here
                    view_dispatcher_switch_to_view(inst->view_dispatcher, VbMigrateViewTextInput);
                }
                vb_migrate_show_loading_popup(inst, false);

                consumed = true;
            }
        }
    }
    return consumed;
}

void vb_migrate_scene_register_save_on_exit(void* context) {
    VbMigrate* inst = context;

    // Clear view
    TextInput* text_input = inst->text_input;
    ValidatorIsFile* validator = text_input_get_validator_callback_context(text_input);
    text_input_set_validator(text_input, NULL, NULL);
    validator_is_file_free(validator);

    text_input_reset(text_input);
    notification_message_block(inst->notifications, &sequence_reset_green);
}
