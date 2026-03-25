// ####################################################
// #  scene_freq.c - frequency selector submenu       #
// #  By: UberGuidoZ | https://github.com/UberGuidoZ/ #
// #  v1.11                                           #
// ####################################################

#include "smart_meter.h"

// Frequencies must stay in sync with k_frequencies[] in scene_monitor.c.
static const char* const k_freq_menu_labels[SM_FREQ_COUNT] = {
    "433.92 MHz  (some water)",
    "868.30 MHz  (EU meters)",
    "915.00 MHz  (US meters)",
};

static void sm_freq_cb(void* context, uint32_t index) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);
    app->freq_index = (uint8_t)index;
    // Pop back to the menu after selection.
    scene_manager_previous_scene(app->scene_manager);
}

void scene_freq_on_enter(void* context) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);

    submenu_reset(app->submenu_freq);
    submenu_set_header(app->submenu_freq, "Select Frequency");

    for(uint8_t i = 0U; i < SM_FREQ_COUNT; i++) {
        submenu_add_item(app->submenu_freq, k_freq_menu_labels[i], i, sm_freq_cb, app);
    }
    // Highlight the currently selected frequency.
    submenu_set_selected_item(app->submenu_freq, app->freq_index);

    view_dispatcher_switch_to_view(app->view_dispatcher, SmViewFreq);
}

bool scene_freq_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void scene_freq_on_exit(void* context) {
    SmartMeterApp* app = (SmartMeterApp*)context;
    furi_check(app != NULL);
    submenu_reset(app->submenu_freq);
}