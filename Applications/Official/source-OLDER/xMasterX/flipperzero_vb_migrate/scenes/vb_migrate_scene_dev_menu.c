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

typedef enum {
    SubmenuDevMenuIndexTransferFromApp,
    SubmenuDevMenuIndexTransferToApp,
    SubmenuDevMenuClearAccountId,
    SubmenuDevMenuIndexSpoof,
    SubmenuDevMenuIndexClearCaptures,
    SubmenuDevMenuIndexDeleteVb,
    // -----
    SubmenuDevMenuClearAccountIdOff,
    SubmenuDevMenuClearAccountIdOn,
    // -----
    SubmenuDevMenuIndexSpoofSelection, // Always keep this last because we add tag type to it
} SubmenuDevMenuIndex;

static void vb_migrate_scene_dev_menu_var_list_enter_callback(void* context, uint32_t index) {
    VbMigrate* inst = context;

    view_dispatcher_send_custom_event(inst->view_dispatcher, index);
}

static VbTagType
    vb_migrate_scene_dev_menu_spoof_set_item_by_index(VariableItemEx* item, uint8_t index) {
    VbTagType type;
    if(index >= 2) { // VBC
        type = (VbTagType)(index + 2);
    } else {
        type = (VbTagType)(index + 1);
    }

    variable_item_ex_set_current_value_index(item, index);
    variable_item_ex_set_current_value_text(item, vb_tag_get_tag_type_name(type));
    return type;
}

static uint8_t
    vb_migrate_scene_dev_menu_spoof_set_item_by_type(VariableItemEx* item, VbTagType type) {
    uint8_t index;
    if(type >= VbTagTypeVH) {
        index = (uint8_t)type - 2;
    } else {
        index = (uint8_t)type - 1;
    }

    variable_item_ex_set_current_value_index(item, index);
    variable_item_ex_set_current_value_text(item, vb_tag_get_tag_type_name(type));
    return index;
}

static void vb_migrate_scene_dev_menu_spoof_change_callback(VariableItemEx* item) {
    VbMigrate* inst = variable_item_ex_get_context(item);
    uint8_t index = variable_item_ex_get_current_value_index(item);

    VbTagType tag_type = vb_migrate_scene_dev_menu_spoof_set_item_by_index(item, index);
    view_dispatcher_send_custom_event(
        inst->view_dispatcher, SubmenuDevMenuIndexSpoofSelection + tag_type);
}

static void vb_migrate_scene_dev_menu_clear_account_id_change_callback(VariableItemEx* item) {
    VbMigrate* inst = variable_item_ex_get_context(item);
    uint8_t index = variable_item_ex_get_current_value_index(item);

    variable_item_ex_set_current_value_text(item, index ? "On" : "Off");
    view_dispatcher_send_custom_event(
        inst->view_dispatcher,
        index ? SubmenuDevMenuClearAccountIdOn : SubmenuDevMenuClearAccountIdOff);
}

void vb_migrate_scene_dev_menu_on_enter(void* context) {
    VbMigrate* inst = context;
    VariableItemListEx* variable_list = inst->variable_list;
    VariableItemEx* item;

    variable_item_list_ex_set_enter_callback(
        variable_list, vb_migrate_scene_dev_menu_var_list_enter_callback, inst);
    variable_item_list_ex_add(
        variable_list, "Transfer App > Flipper", 0, NULL, NULL, SubmenuDevMenuIndexTransferFromApp);

    if(inst->num_captured != 0) {
        variable_item_list_ex_add(
            variable_list,
            "Transfer Flipper > App",
            0,
            NULL,
            NULL,
            SubmenuDevMenuIndexTransferToApp);
    }

    item = variable_item_list_ex_add(
        variable_list,
        "Unlink Account",
        2,
        vb_migrate_scene_dev_menu_clear_account_id_change_callback,
        inst,
        SubmenuDevMenuClearAccountId);
    variable_item_ex_set_current_value_index(item, inst->clear_account_id ? 1 : 0);
    variable_item_ex_set_current_value_text(item, inst->clear_account_id ? "On" : "Off");

    if(inst->orig_type == VbTagTypeVBDM || inst->orig_type == VbTagTypeVBV ||
       inst->orig_type == VbTagTypeVH) {
        item = variable_item_list_ex_add(
            variable_list,
            "Spoof Version",
            VbTagTypeMax - 1 - 2, // Removing VBC from list, skip VBBE
            vb_migrate_scene_dev_menu_spoof_change_callback,
            inst,
            SubmenuDevMenuIndexSpoof);
        vb_migrate_scene_dev_menu_spoof_set_item_by_type(item, inst->override_type);
    }

    if(inst->num_captured != 0) {
        variable_item_list_ex_add(
            variable_list, "Clear Captures", 0, NULL, NULL, SubmenuDevMenuIndexClearCaptures);
    }

    variable_item_list_ex_add(
        variable_list, "Delete Vital Bracelet", 0, NULL, NULL, SubmenuDevMenuIndexDeleteVb);

    // variable_item_list_ex_set_selected_item(
    //     variable_list, scene_manager_get_scene_state(inst->scene_manager, VbMigrateSceneDevMenu));

    view_dispatcher_switch_to_view(inst->view_dispatcher, VbMigrateViewVariableItemList);
}

bool vb_migrate_scene_dev_menu_on_event(void* context, SceneManagerEvent event) {
    VbMigrate* inst = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        // if(event.event < SubmenuDevMenuIndexSpoofSelection) {
        //     scene_manager_set_scene_state(inst->scene_manager, VbMigrateSceneDevMenu, event.event);
        // }

        if(event.event == SubmenuDevMenuIndexTransferFromApp) {
            scene_manager_next_scene(inst->scene_manager, VbMigrateSceneFromApp);
            consumed = true;
        } else if(event.event == SubmenuDevMenuIndexTransferToApp) {
            scene_manager_next_scene(inst->scene_manager, VbMigrateSceneToApp);
            consumed = true;
        } else if(event.event == SubmenuDevMenuIndexClearCaptures) {
            scene_manager_next_scene(inst->scene_manager, VbMigrateSceneDeleteCaptures);
            consumed = true;
        } else if(event.event == SubmenuDevMenuIndexDeleteVb) {
            scene_manager_next_scene(inst->scene_manager, VbMigrateSceneDelete);
            consumed = true;
        } else if(event.event == SubmenuDevMenuClearAccountIdOff) {
            inst->clear_account_id = false;
            consumed = true;
        } else if(event.event == SubmenuDevMenuClearAccountIdOn) {
            inst->clear_account_id = true;
            consumed = true;
        } else if(event.event >= SubmenuDevMenuIndexSpoofSelection) {
            // Note: skipping SubmenuDevMenuIndexSpoof because there's nothing to do on enter
            inst->override_type = event.event - SubmenuDevMenuIndexSpoofSelection;
            consumed = true;
        }
    }
    return consumed;
}

void vb_migrate_scene_dev_menu_on_exit(void* context) {
    VbMigrate* inst = context;

    variable_item_list_ex_reset(inst->variable_list);
}
