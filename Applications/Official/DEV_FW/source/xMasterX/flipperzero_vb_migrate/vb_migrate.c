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

#include "vb_migrate_i.h"

#define TAG "vb_migrate"

bool vb_migrate_custom_event_callback(void* context, uint32_t event) {
    furi_assert(context);
    VbMigrate* inst = context;
    return scene_manager_handle_custom_event(inst->scene_manager, event);
}

bool vb_migrate_back_event_callback(void* context) {
    furi_assert(context);
    VbMigrate* inst = context;
    return scene_manager_handle_back_event(inst->scene_manager);
}

void vb_migrate_blink_read(VbMigrate* inst) {
    notification_message(inst->notifications, &sequence_blink_start_cyan);
}

void vb_migrate_blink_emulate(VbMigrate* inst) {
    notification_message(inst->notifications, &sequence_blink_start_magenta);
}

void vb_migrate_blink_stop(VbMigrate* inst) {
    notification_message_block(inst->notifications, &sequence_blink_stop);
}

void vb_migrate_text_store_set(VbMigrate* inst, const char* text, ...) {
    va_list args;
    va_start(args, text);

    vsnprintf(inst->text_store, sizeof(inst->text_store), text, args);

    va_end(args);
}

void vb_migrate_text_store_clear(VbMigrate* inst) {
    memset(inst->text_store, 0, sizeof(inst->text_store));
}

bool vb_migrate_save_nfc(VbMigrate* inst, const char* dev_name, const char* file_name) {
    bool saved = false;
    FuriString* temp_str = furi_string_alloc();

    do {
        furi_string_printf(temp_str, "%s/%s", VB_MIGRATE_FOLDER, dev_name);
        if(!storage_simply_mkdir(inst->storage, furi_string_get_cstr(temp_str))) {
            dialog_message_show_storage_error(inst->dialogs, "Can not create\ndata folder");
            break;
        }
        furi_string_cat_printf(temp_str, "/%s", file_name);
        inst->nfc_dev->format = NfcDeviceSaveFormatMifareUl;
        saved = nfc_device_save(inst->nfc_dev, furi_string_get_cstr(temp_str));
    } while(false);

    furi_string_free(temp_str);
    return saved;
}

bool vb_migrate_load_nfc(VbMigrate* inst, const char* dev_name, const char* file_name) {
    bool saved = false;
    FuriString* temp_str =
        furi_string_alloc_printf("%s/%s/%s", VB_MIGRATE_FOLDER, dev_name, file_name);
    saved = nfc_device_load(inst->nfc_dev, furi_string_get_cstr(temp_str), true);

    furi_string_free(temp_str);
    return saved;
}

bool vb_migrate_delete(VbMigrate* inst, const char* dev_name, bool whole_vb) {
    bool deleted = false;
    FuriString* dir_path = furi_string_alloc_printf("%s/%s", VB_MIGRATE_FOLDER, dev_name);

    if(whole_vb) {
        deleted = storage_simply_remove_recursive(inst->storage, furi_string_get_cstr(dir_path));
    } else {
        File* dir_handle = storage_file_alloc(inst->storage);
        if(storage_dir_open(dir_handle, furi_string_get_cstr(dir_path))) {
            FileInfo file_info;
            char name[256];
            FuriString* file_path = furi_string_alloc();
            while(storage_dir_read(dir_handle, &file_info, name, sizeof(name))) {
                // Files that is .nfc, but is not template
                if(!(file_info.flags & FSF_DIRECTORY) && strstr(name, NFC_APP_EXTENSION) &&
                   !strstr(name, VB_MIGRATE_TEMPLATE_NAME)) {
                    furi_string_printf(file_path, "%s/%s", furi_string_get_cstr(dir_path), name);
                    deleted =
                        storage_simply_remove(inst->storage, furi_string_get_cstr(file_path));
                    if(!deleted) break;
                }
            }

            furi_string_free(file_path);
            storage_dir_close(dir_handle);
        }
        storage_file_free(dir_handle);
    }

    furi_string_free(dir_path);
    return deleted;
}

int vb_migrate_count_captured_mons(VbMigrate* inst, const char* dev_name) {
    int count = 0;

    FuriString* dir_path = furi_string_alloc_printf("%s/%s", VB_MIGRATE_FOLDER, dev_name);
    File* dir_handle = storage_file_alloc(inst->storage);
    if(storage_dir_open(dir_handle, furi_string_get_cstr(dir_path))) {
        FileInfo file_info;
        char name[256];
        while(storage_dir_read(dir_handle, &file_info, name, sizeof(name))) {
            // Files that is .nfc, but is not template
            if(!(file_info.flags & FSF_DIRECTORY) && strstr(name, NFC_APP_EXTENSION) &&
               !strstr(name, VB_MIGRATE_TEMPLATE_NAME))
                ++count;
        }

        storage_dir_close(dir_handle);
    }
    storage_file_free(dir_handle);
    furi_string_free(dir_path);

    return count;
}

int vb_migrate_get_next_id(VbMigrate* inst, const char* dev_name, int i, bool is_load) {
    FuriString* dir_path = furi_string_alloc_printf("%s/%s", VB_MIGRATE_FOLDER, dev_name);
    FuriString* file_path = furi_string_alloc();
    while(true) {
        furi_string_printf(
            file_path,
            "%s/" VB_MIGRATE_CAPTURE_FORMAT,
            furi_string_get_cstr(dir_path),
            i,
            NFC_APP_EXTENSION);
        bool exit_cond =
            storage_common_stat(inst->storage, furi_string_get_cstr(file_path), NULL) ==
            FSE_NOT_EXIST;
        if(is_load) exit_cond = !exit_cond;
        if(exit_cond) break;
        ++i;
    }

    furi_string_free(file_path);
    furi_string_free(dir_path);
    return i;
}

void vb_migrate_show_loading_popup(VbMigrate* inst, bool show) {
    TaskHandle_t timer_task = xTaskGetHandle(configTIMER_SERVICE_TASK_NAME);

    if(show) {
        // Raise timer priority so that animations can play
        vTaskPrioritySet(timer_task, configMAX_PRIORITIES - 1);
        view_dispatcher_switch_to_view(inst->view_dispatcher, VbMigrateViewLoading);
    } else {
        // Restore default timer priority
        vTaskPrioritySet(timer_task, configTIMER_TASK_PRIORITY);
    }
}

VbMigrate* vb_migrate_alloc() {
    VbMigrate* inst = malloc(sizeof(VbMigrate));

    inst->view_dispatcher = view_dispatcher_alloc();
    inst->scene_manager = scene_manager_alloc(&vb_migrate_scene_handlers, inst);
    view_dispatcher_enable_queue(inst->view_dispatcher);
    view_dispatcher_set_event_callback_context(inst->view_dispatcher, inst);
    view_dispatcher_set_custom_event_callback(
        inst->view_dispatcher, vb_migrate_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(
        inst->view_dispatcher, vb_migrate_back_event_callback);

    inst->clear_account_id = false;

    // GUI
    inst->gui = furi_record_open(RECORD_GUI);

    // Storage
    inst->storage = furi_record_open(RECORD_STORAGE);

    // Notifications service
    inst->notifications = furi_record_open(RECORD_NOTIFICATION);

    // Dialogs
    inst->dialogs = furi_record_open(RECORD_DIALOGS);

    // NFC
    inst->nfc_dev = nfc_device_alloc();
    inst->worker = nfc_worker_alloc();

    // Submenu
    inst->submenu = submenu_alloc();
    view_dispatcher_add_view(
        inst->view_dispatcher, VbMigrateViewMenu, submenu_get_view(inst->submenu));

    // Popup
    inst->popup = popup_alloc();
    view_dispatcher_add_view(
        inst->view_dispatcher, VbMigrateViewPopup, popup_get_view(inst->popup));

    // Widget
    inst->widget = widget_alloc();
    view_dispatcher_add_view(
        inst->view_dispatcher, VbMigrateViewWidget, widget_get_view(inst->widget));

    // File select
    inst->file_select = file_select_alloc();
    view_dispatcher_add_view(
        inst->view_dispatcher, VbMigrateViewFileSelect, file_select_get_view(inst->file_select));

    // Text input
    inst->text_input = text_input_alloc();
    view_dispatcher_add_view(
        inst->view_dispatcher, VbMigrateViewTextInput, text_input_get_view(inst->text_input));

    // Loading
    inst->loading = loading_alloc();
    view_dispatcher_add_view(
        inst->view_dispatcher, VbMigrateViewLoading, loading_get_view(inst->loading));

    // Variable item list
    inst->variable_list = variable_item_list_ex_alloc();
    view_dispatcher_add_view(
        inst->view_dispatcher,
        VbMigrateViewVariableItemList,
        variable_item_list_ex_get_view(inst->variable_list));

    // Dialog ex
    // inst->dialog_ex = dialog_ex_alloc();
    // view_dispatcher_add_view(
    //     inst->view_dispatcher, VbMigrateViewDialogEx, dialog_ex_get_view(inst->dialog_ex));

    return inst;
}

void vb_migrate_free(VbMigrate* inst) {
    // Dialog ex
    // view_dispatcher_remove_view(inst->view_dispatcher, VbMigrateViewDialogEx);
    // dialog_ex_free(inst->dialog_ex);

    // Variable item list
    view_dispatcher_remove_view(inst->view_dispatcher, VbMigrateViewVariableItemList);
    variable_item_list_ex_free(inst->variable_list);

    // Loading
    view_dispatcher_remove_view(inst->view_dispatcher, VbMigrateViewLoading);
    loading_free(inst->loading);

    // Text input
    view_dispatcher_remove_view(inst->view_dispatcher, VbMigrateViewTextInput);
    text_input_free(inst->text_input);

    // File select
    view_dispatcher_remove_view(inst->view_dispatcher, VbMigrateViewFileSelect);
    file_select_free(inst->file_select);

    // Widget
    view_dispatcher_remove_view(inst->view_dispatcher, VbMigrateViewWidget);
    widget_free(inst->widget);

    // Popup
    view_dispatcher_remove_view(inst->view_dispatcher, VbMigrateViewPopup);
    popup_free(inst->popup);

    // Submenu
    view_dispatcher_remove_view(inst->view_dispatcher, VbMigrateViewMenu);
    submenu_free(inst->submenu);

    // NFC
    nfc_worker_free(inst->worker);
    nfc_device_free(inst->nfc_dev);

    furi_record_close(RECORD_DIALOGS);
    furi_record_close(RECORD_NOTIFICATION);
    furi_record_close(RECORD_STORAGE);
    furi_record_close(RECORD_GUI);

    view_dispatcher_free(inst->view_dispatcher);
    scene_manager_free(inst->scene_manager);

    free(inst);
}

int32_t vb_migrate_app(void* p) {
    UNUSED(p);

    VbMigrate* inst = vb_migrate_alloc();
    view_dispatcher_attach_to_gui(inst->view_dispatcher, inst->gui, ViewDispatcherTypeFullscreen);

    if(storage_simply_mkdir(inst->storage, VB_MIGRATE_FOLDER)) {
        scene_manager_next_scene(inst->scene_manager, VbMigrateSceneMainMenu);
        view_dispatcher_run(inst->view_dispatcher);
    } else {
        dialog_message_show_storage_error(inst->dialogs, "Can not create\napp folder");
    }

    vb_migrate_free(inst);
    return 0;
}

void vb_migrate_add_bg(Widget* widget, VbMigrateBgType type) {
    widget_add_icon_element(widget, 0, 0, &I_Background_128x64);
    if(type & VbMigrateBgTypeLeftButton) {
        widget_add_icon_element(widget, 0, 49, &I_LeftButtonBg_54x15);
    }
    if(type & VbMigrateBgTypeRightButton) {
        widget_add_icon_element(widget, 74, 49, &I_RightButtonBg_54x15);
    }
}
