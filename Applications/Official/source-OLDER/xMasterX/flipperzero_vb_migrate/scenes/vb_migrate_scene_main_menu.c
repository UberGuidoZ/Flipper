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
    SubmenuMainIndexRegister,
    SubmenuMainIndexSelect,
    SubmenuMainIndexAbout,
} SubmenuMainIndex;

static void vb_migrate_scene_main_menu_submenu_callback(void* context, uint32_t index) {
    VbMigrate* inst = context;

    view_dispatcher_send_custom_event(inst->view_dispatcher, index);
}

void vb_migrate_scene_main_menu_on_enter(void* context) {
    VbMigrate* inst = context;
    Submenu* submenu = inst->submenu;

    submenu_add_item(
        submenu,
        "Register Vital Bracelet",
        SubmenuMainIndexRegister,
        vb_migrate_scene_main_menu_submenu_callback,
        inst);
    submenu_add_item(
        submenu,
        "Select Vital Bracelet",
        SubmenuMainIndexSelect,
        vb_migrate_scene_main_menu_submenu_callback,
        inst);
    submenu_add_item(
        submenu, "About", SubmenuMainIndexAbout, vb_migrate_scene_main_menu_submenu_callback, inst);

    view_dispatcher_switch_to_view(inst->view_dispatcher, VbMigrateViewMenu);
}

bool vb_migrate_scene_main_menu_on_event(void* context, SceneManagerEvent event) {
    VbMigrate* inst = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SubmenuMainIndexRegister) {
            scene_manager_next_scene(inst->scene_manager, VbMigrateSceneRegister);
            consumed = true;
        } else if(event.event == SubmenuMainIndexSelect) {
            scene_manager_next_scene(inst->scene_manager, VbMigrateSceneSelect);
            consumed = true;
        } else if(event.event == SubmenuMainIndexAbout) {
            scene_manager_next_scene(inst->scene_manager, VbMigrateSceneAbout);
            consumed = true;
        }
    }
    return consumed;
}

void vb_migrate_scene_main_menu_on_exit(void* context) {
    VbMigrate* inst = context;

    submenu_reset(inst->submenu);
}
