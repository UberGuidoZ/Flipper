/**
 * @file variable_item_list.h
 * GUI: VariableItemList view module API
 */

#pragma once

#include <gui/view.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct VariableItemListEx VariableItemListEx;
typedef struct VariableItemEx VariableItemEx;
typedef void (*VariableItemExChangeCallback)(VariableItemEx* item);
typedef void (*VariableItemListExEnterCallback)(void* context, uint32_t index);

/** Allocate and initialize VariableItemList
 *
 * @return     VariableItemList*
 */
VariableItemListEx* variable_item_list_ex_alloc();

/** Deinitialize and free VariableItemList
 *
 * @param      variable_item_list  VariableItemList instance
 */
void variable_item_list_ex_free(VariableItemListEx* variable_item_list);

/** Clear all elements from list
 *
 * @param      variable_item_list  VariableItemList instance
 */
void variable_item_list_ex_reset(VariableItemListEx* variable_item_list);

/** Get VariableItemList View instance
 *
 * @param      variable_item_list  VariableItemList instance
 *
 * @return     View instance
 */
View* variable_item_list_ex_get_view(VariableItemListEx* variable_item_list);

/** Add item to VariableItemList
 *
 * @param      variable_item_list  VariableItemList instance
 * @param      label               item name
 * @param      values_count        item values count
 * @param      change_callback     called on value change in gui
 * @param      context             item context
 *
 * @return     VariableItemEx* item instance
 */
VariableItemEx* variable_item_list_ex_add(
    VariableItemListEx* variable_item_list,
    const char* label,
    uint8_t values_count,
    VariableItemExChangeCallback change_callback,
    void* context,
    int32_t callback_index);

/** Set enter callback
 *
 * @param      variable_item_list  VariableItemList instance
 * @param      callback            VariableItemListEnterCallback instance
 * @param      context             pointer to context
 */
void variable_item_list_ex_set_enter_callback(
    VariableItemListEx* variable_item_list,
    VariableItemListExEnterCallback callback,
    void* context);

void variable_item_list_ex_set_selected_item(VariableItemListEx* variable_item_list, uint8_t index);

uint8_t variable_item_list_ex_get_selected_item_index(VariableItemListEx* variable_item_list);

/** Set item current selected index
 *
 * @param      item                 VariableItemEx* instance
 * @param      current_value_index  The current value index
 */
void variable_item_ex_set_current_value_index(VariableItemEx* item, uint8_t current_value_index);

/** Set number of values for item
 *
 * @param      item                 VariableItemEx* instance
 * @param      values_count         The new values count
 */
void variable_item_ex_set_values_count(VariableItemEx* item, uint8_t values_count);

/** Set item current selected text
 *
 * @param      item                VariableItemEx* instance
 * @param      current_value_text  The current value text
 */
void variable_item_ex_set_current_value_text(VariableItemEx* item, const char* current_value_text);

/** Get item current selected index
 *
 * @param      item  VariableItemEx* instance
 *
 * @return     uint8_t current selected index
 */
uint8_t variable_item_ex_get_current_value_index(VariableItemEx* item);

/** Get item context
 *
 * @param      item  VariableItemEx* instance
 *
 * @return     void* item context
 */
void* variable_item_ex_get_context(VariableItemEx* item);

#ifdef __cplusplus
}
#endif
