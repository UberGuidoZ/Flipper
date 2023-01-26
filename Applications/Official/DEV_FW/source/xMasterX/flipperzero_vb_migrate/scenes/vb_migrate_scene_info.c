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
#include "../vb_tag.h"

static void
    vb_migrate_scene_info_button_callback(GuiButtonType result, InputType type, void* context) {
    VbMigrate* inst = context;

    if(type == InputTypeShort) {
        if(result == GuiButtonTypeRight) {
            view_dispatcher_send_custom_event(inst->view_dispatcher, result);
        }
    }
}

void vb_migrate_scene_info_on_enter(void* context) {
    VbMigrate* inst = context;
    FuriString* temp_str = furi_string_alloc();

    // Build info scroll
    // Name
    furi_string_cat_printf(temp_str, "\ec%s\n", inst->text_store);

    // Type
    if(inst->orig_product == NULL)
        furi_string_cat(temp_str, "Unknown product\n");
    else
        furi_string_cat_printf(temp_str, "\e#%s\n", inst->orig_product->name);

    // Number of mons loaded
    furi_string_cat_printf(temp_str, "Charas. captured: %d", inst->num_captured);

    vb_migrate_add_bg(inst->widget, VbMigrateBgTypeRightButton);

    widget_add_text_scroll_element(inst->widget, 8, 16, 113, 33, furi_string_get_cstr(temp_str));
    furi_string_free(temp_str);

    widget_add_button_element(
        inst->widget, GuiButtonTypeRight, "More", vb_migrate_scene_info_button_callback, inst);
    view_dispatcher_switch_to_view(inst->view_dispatcher, VbMigrateViewWidget);
}

bool vb_migrate_scene_info_on_event(void* context, SceneManagerEvent event) {
    VbMigrate* inst = context;
    bool consumed = false;
    UNUSED(inst);

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == GuiButtonTypeRight) {
            // Reset menu selection index
            scene_manager_set_scene_state(inst->scene_manager, VbMigrateSceneDevMenu, 0);
            scene_manager_next_scene(inst->scene_manager, VbMigrateSceneDevMenu);
            consumed = true;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        uint32_t back_scenes[] = {VbMigrateSceneSelect, VbMigrateSceneMainMenu};
        consumed = scene_manager_search_and_switch_to_previous_scene_one_of(
            inst->scene_manager, back_scenes, COUNT_OF(back_scenes));
    }
    return consumed;
}

void vb_migrate_scene_info_on_exit(void* context) {
    VbMigrate* inst = context;

    // Perform your cleanup here
    widget_reset(inst->widget);
}
