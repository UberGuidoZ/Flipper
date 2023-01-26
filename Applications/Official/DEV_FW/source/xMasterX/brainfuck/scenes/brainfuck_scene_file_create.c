#include "../brainfuck_i.h"

void file_name_text_input_callback(void* context) {
    BFApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, brainfuckCustomEventTextInputDone);
}

char tmpName[64] = {};
byte empty[1] = {0x00};
void brainfuck_scene_file_create_on_enter(void* context) {
    BFApp* app = context;
    TextInput* text_input = app->text_input;

    text_input_set_header_text(text_input, "New script name");
    text_input_set_result_callback(
        text_input,
        file_name_text_input_callback,
        app,
        tmpName,
        64,
        true);

    view_dispatcher_switch_to_view(app->view_dispatcher, brainfuckViewTextInput);
}

bool brainfuck_scene_file_create_on_event(void* context, SceneManagerEvent event) {
    BFApp* app = context;
    UNUSED(app);
    
    bool consumed = false;
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == brainfuckCustomEventTextInputDone) {
            furi_string_cat_printf(app->BF_file_path, "/ext/brainfuck/%s.b", tmpName);

            //remove old file
            Storage* storage = furi_record_open(RECORD_STORAGE);
            storage_simply_remove(storage, furi_string_get_cstr(app->BF_file_path));

            //save new file
            Stream* stream = buffered_file_stream_alloc(storage);
            buffered_file_stream_open(stream, furi_string_get_cstr(app->BF_file_path), FSAM_WRITE, FSOM_CREATE_ALWAYS);
            stream_write(stream, (const uint8_t*)empty, 1);
            buffered_file_stream_close(stream);

            //scene_manager_next_scene(app->scene_manager, brainfuckSceneFileSelect);
            scene_manager_next_scene(app->scene_manager, brainfuckSceneDevEnv);
        }
    }
    return consumed;
}

void brainfuck_scene_file_create_on_exit(void* context) {
    UNUSED(context);
}
