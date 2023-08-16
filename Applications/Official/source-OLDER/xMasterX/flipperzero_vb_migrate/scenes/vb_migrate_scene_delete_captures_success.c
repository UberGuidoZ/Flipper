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

static void vb_migrate_scene_delete_captures_success_widget_callback(
    GuiButtonType result,
    InputType type,
    void* context) {
    VbMigrate* inst = context;

    if(type == InputTypeShort) {
        if(result == GuiButtonTypeRight)
            view_dispatcher_send_custom_event(inst->view_dispatcher, 0);
    }
}

void vb_migrate_scene_delete_captures_success_on_enter(void* context) {
    VbMigrate* inst = context;

    // Perform your setup here
    Widget* widget = inst->widget;
    vb_migrate_add_bg(widget, VbMigrateBgTypeRightButton);
    widget_add_icon_element(widget, 11, 18, &I_Delete_32x20);
    widget_add_icon_element(widget, 48, 18, &I_TextCleared_47x6);
    widget_add_icon_element(widget, 9, 41, &I_PulsemonRightSad_15x15);
    widget_add_button_element(
        widget,
        GuiButtonTypeRight,
        "OK",
        vb_migrate_scene_delete_captures_success_widget_callback,
        inst);

    view_dispatcher_switch_to_view(inst->view_dispatcher, VbMigrateViewWidget);
}

bool vb_migrate_scene_delete_captures_success_on_event(void* context, SceneManagerEvent event) {
    VbMigrate* inst = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom || event.type == SceneManagerEventTypeBack) {
        consumed = scene_manager_search_and_switch_to_previous_scene(
            inst->scene_manager, VbMigrateSceneDevMenu);
    }
    return consumed;
}

void vb_migrate_scene_delete_captures_success_on_exit(void* context) {
    VbMigrate* inst = context;

    // Perform your cleanup here
    widget_reset(inst->widget);
}
