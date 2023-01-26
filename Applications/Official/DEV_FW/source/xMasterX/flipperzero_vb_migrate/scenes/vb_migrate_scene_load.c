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

#include "../vb_migrate_i.h"

#define TAG "vb_migrate_scene_load"

void vb_migrate_scene_load_on_enter(void* context) {
    VbMigrate* inst = context;

    // Perform your setup here
    vb_migrate_show_loading_popup(inst, true);
    view_dispatcher_send_custom_event(inst->view_dispatcher, 0);
}

bool vb_migrate_scene_load_on_event(void* context, SceneManagerEvent event) {
    VbMigrate* inst = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(vb_migrate_load_nfc(inst, inst->text_store, VB_MIGRATE_TEMPLATE_NAME)) {
            inst->num_captured = vb_migrate_count_captured_mons(inst, inst->text_store);

            BantBlock* bant = vb_tag_get_bant_block(&inst->nfc_dev->dev_data);
            const VbTagProduct* product = vb_tag_find_product(bant);
            inst->orig_product = product;
            if(product) {
                inst->orig_type = product->type;
            } else {
                inst->orig_type = VbTagTypeUnknown;
            }
            inst->override_type = inst->orig_type;

            scene_manager_next_scene(inst->scene_manager, VbMigrateSceneInfo);
            consumed = true;
        } else {
            consumed = scene_manager_previous_scene(inst->scene_manager);
        }
        vb_migrate_show_loading_popup(inst, false);
    }
    return consumed;
}

void vb_migrate_scene_load_on_exit(void* context) {
    UNUSED(context);
}
