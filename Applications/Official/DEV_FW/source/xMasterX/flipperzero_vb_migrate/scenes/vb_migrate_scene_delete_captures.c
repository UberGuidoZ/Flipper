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

static void vb_migrate_scene_delete_captures_widget_callback(
    GuiButtonType result,
    InputType type,
    void* context) {
    VbMigrate* inst = context;

    if(type == InputTypeShort) {
        view_dispatcher_send_custom_event(inst->view_dispatcher, result);
    }
}

void vb_migrate_scene_delete_captures_on_enter(void* context) {
    VbMigrate* inst = context;

    // Perform your setup here
    Widget* widget = inst->widget;
    vb_migrate_add_bg(widget, VbMigrateBgTypeLeftButton | VbMigrateBgTypeRightButton);
    widget_add_icon_element(widget, 11, 18, &I_Delete_32x20);
    widget_add_icon_element(widget, 48, 18, &I_TextClearCaptures_49x13);

    FuriString* temp_str = furi_string_alloc_printf("%d", inst->num_captured);
    widget_add_string_element(
        widget, 99, 24, AlignLeft, AlignTop, FontSecondary, furi_string_get_cstr(temp_str));
    furi_string_free(temp_str);

    widget_add_button_element(
        inst->widget,
        GuiButtonTypeLeft,
        "Cancel",
        vb_migrate_scene_delete_captures_widget_callback,
        inst);
    widget_add_button_element(
        inst->widget,
        GuiButtonTypeRight,
        "Delete",
        vb_migrate_scene_delete_captures_widget_callback,
        inst);

    view_dispatcher_switch_to_view(inst->view_dispatcher, VbMigrateViewWidget);
}

bool vb_migrate_scene_delete_captures_on_event(void* context, SceneManagerEvent event) {
    VbMigrate* inst = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == GuiButtonTypeLeft) {
            consumed = scene_manager_previous_scene(inst->scene_manager);
        } else if(event.event == GuiButtonTypeRight) {
            if(vb_migrate_delete(inst, inst->text_store, false)) {
                scene_manager_next_scene(inst->scene_manager, VbMigrateSceneDeleteCapturesSuccess);
                consumed = true;
            } else {
                consumed = scene_manager_previous_scene(inst->scene_manager);
            }
            inst->num_captured = vb_migrate_count_captured_mons(inst, inst->text_store);
        }
    }
    return consumed;
}

void vb_migrate_scene_delete_captures_on_exit(void* context) {
    VbMigrate* inst = context;

    // Perform your cleanup here
    widget_reset(inst->widget);
}
