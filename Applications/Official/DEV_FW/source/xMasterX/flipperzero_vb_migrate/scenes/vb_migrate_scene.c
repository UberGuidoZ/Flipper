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

#include "vb_migrate_scene.h"

// Generate scene on_enter handlers array
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_enter,
void (*const vb_migrate_on_enter_handlers[])(void*) = {
#include "vb_migrate_scene_config.h"
};
#undef ADD_SCENE

// Generate scene on_event handlers array
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_event,
bool (*const vb_migrate_on_event_handlers[])(void* context, SceneManagerEvent event) = {
#include "vb_migrate_scene_config.h"
};
#undef ADD_SCENE

// Generate scene on_exit handlers array
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_exit,
void (*const vb_migrate_on_exit_handlers[])(void* context) = {
#include "vb_migrate_scene_config.h"
};
#undef ADD_SCENE

// Initialize scene handlers configuration structure
const SceneManagerHandlers vb_migrate_scene_handlers = {
    .on_enter_handlers = vb_migrate_on_enter_handlers,
    .on_event_handlers = vb_migrate_on_event_handlers,
    .on_exit_handlers = vb_migrate_on_exit_handlers,
    .scene_num = VbMigrateSceneNum,
};
