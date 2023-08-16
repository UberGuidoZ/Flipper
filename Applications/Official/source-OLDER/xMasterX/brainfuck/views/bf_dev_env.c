#include "bf_dev_env.h"
#include <gui/elements.h>

typedef struct BFDevEnv {
    View* view;
    DevEnvOkCallback callback;
    void* context;
    BFApp* appDev;
} BFDevEnv;

typedef struct {
    uint32_t row;
    uint32_t col;
} BFDevEnvModel;

typedef struct{
    int up;
    int down;
    int left;
    int right;
}bMapping;

static bool bf_dev_process_up(BFDevEnv* devEnv);
static bool bf_dev_process_down(BFDevEnv* devEnv);
static bool bf_dev_process_left(BFDevEnv* devEnv);
static bool bf_dev_process_right(BFDevEnv* devEnv);
static bool bf_dev_process_ok(BFDevEnv* devEnv, InputEvent* event);

BFApp* appDev;

char bfChars[9] = {'<', '>', '[', ']', '+', '-', '.', ',', 0x00};

int selectedButton = 0;
int saveNotifyCountdown = 0;
int execCountdown = 0;

char dspLine0[25] = {};
char dspLine1[25] = {};
char dspLine2[25] = {};

static bMapping buttonMappings[12] = {
    { 8,  8,  7,  1},  //0
    { 8,  8,  0,  2},  //1
    { 9,  9,  1,  3},  //2
    { 9,  9,  2,  4},  //3
    {10, 10,  3,  5},  //4
    {10, 10,  4,  6},  //5
    {11, 11,  5,  7},  //6
    {11, 11,  6,  0},  //7

    { 0,  0, 11,  9},  //8 
    { 3,  3,  8, 10},  //9
    { 5,  5,  9, 11},  //10
    { 6,  6, 10,  8}   //11
};

#define BT_X 14
#define BT_Y 14
static void bf_dev_draw_button(Canvas* canvas, int x, int y, bool selected, const char* lbl){
    UNUSED(lbl);

    if(selected){
        canvas_draw_rbox(canvas, x, y, BT_X, BT_Y, 3);
        canvas_invert_color(canvas);
        canvas_set_font(canvas, FontBatteryPercent);
        canvas_draw_str_aligned(canvas, x + (BT_X/2), y + (BT_Y/2) - 1, AlignCenter, AlignCenter, lbl);
        canvas_invert_color(canvas);
    }
    else{
        canvas_draw_rbox(canvas, x, y, BT_X, BT_Y, 3);
        canvas_invert_color(canvas);
        canvas_draw_rbox(canvas, x+2, y-1, BT_X - 2, BT_Y - 1, 3);
        canvas_invert_color(canvas);
        canvas_draw_rframe(canvas, x, y, BT_X, BT_Y, 3);
        canvas_set_font(canvas, FontBatteryPercent);
        canvas_draw_str_aligned(canvas, x + (BT_X/2), y + (BT_Y/2) - 1, AlignCenter, AlignCenter, lbl);
    }
}

static void bf_dev_draw_callback(Canvas* canvas, void* _model) {
    UNUSED(_model);

    if(execCountdown > 0){
        execCountdown--;
        canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignCenter, "RUNNING...");
        if(execCountdown == 0){
            initWorker(appDev);
            beginWorker();
            text_box_set_text(appDev->text_box, workerGetOutput());
            scene_manager_next_scene(appDev->scene_manager, brainfuckSceneExecEnv);
        }
        return;
    }

    if(saveNotifyCountdown > 0){
        canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignCenter, "SAVED");
        saveNotifyCountdown--;
        return;
    }

    bf_dev_draw_button(canvas, 1, 36, (selectedButton == 0), "+");    //T 0
    bf_dev_draw_button(canvas, 17, 36, (selectedButton == 1),  "-");  //T 1
    bf_dev_draw_button(canvas, 33, 36, (selectedButton == 2),  "<");  //T 2
    bf_dev_draw_button(canvas, 49, 36, (selectedButton == 3),  ">");  //T 3
    bf_dev_draw_button(canvas, 65, 36, (selectedButton == 4),  "[");  //B 0
    bf_dev_draw_button(canvas, 81, 36, (selectedButton == 5),  "]");  //B 1
    bf_dev_draw_button(canvas, 97, 36, (selectedButton == 6),  ".");  //B 2
    bf_dev_draw_button(canvas, 113, 36, (selectedButton == 7),  ","); //B 3

    //backspace, input, run, save
    canvas_draw_icon(canvas, 1, 52, (selectedButton == 8) ? &I_KeyBackspaceSelected_24x11 : &I_KeyBackspace_24x11);
    canvas_draw_icon(canvas, 45, 52, (selectedButton == 9) ? &I_KeyInputSelected_30x11 : &I_KeyInput_30x11);
    canvas_draw_icon(canvas, 77, 52, (selectedButton == 10) ? &I_KeyRunSelected_24x11 : &I_KeyRun_24x11);
    canvas_draw_icon(canvas, 103, 52, (selectedButton == 11) ? &I_KeySaveSelected_24x11 : &I_KeySave_24x11);

    if(saveNotifyCountdown > 0){
        canvas_draw_icon(canvas, 98, 54, &I_ButtonRightSmall_3x5);
        saveNotifyCountdown--;
    }

    //textbox
    //grossly overcomplicated. not fixing it.
    canvas_draw_rframe(canvas, 1, 1, 125, 33, 2);
    canvas_set_font(canvas, FontBatteryPercent);

    int dbOffset = 0;
    if(appDev->dataSize > 72){
        dbOffset = (appDev->dataSize - 72);
    }
 
    memset(dspLine0, 0x00, 25);
    memset(dspLine1, 0x00, 25);
    memset(dspLine2, 0x00, 25);

    int tpM = 0;
    int tp0 = 0;
    int tp1 = 0;
    int tp2 = 0;

    for(int p = dbOffset; p < appDev->dataSize; p++){
        if(tpM < 24 * 1){
            dspLine0[tp0] = appDev->dataBuffer[p];
            tp0++;
        }
        else if(tpM < 24 * 2){
            dspLine1[tp1] = appDev->dataBuffer[p];
            tp1++;
        }
        else if(tpM < 24 * 3){
            dspLine2[tp2] = appDev->dataBuffer[p];
            tp2++;
        }
        tpM++;
    }

    canvas_draw_str_aligned(canvas, 3, 8, AlignLeft, AlignCenter, dspLine0);
    canvas_draw_str_aligned(canvas, 3, 17, AlignLeft, AlignCenter, dspLine1);
    canvas_draw_str_aligned(canvas, 3, 26, AlignLeft, AlignCenter, dspLine2);
}


static bool bf_dev_input_callback(InputEvent* event, void* context) {
    furi_assert(context);
    BFDevEnv* devEnv = context;
    bool consumed = false;

    if(event->type == InputTypeShort) {
        if(event->key == InputKeyRight) {
            consumed = bf_dev_process_right(devEnv);
        } else if(event->key == InputKeyLeft) {
            consumed = bf_dev_process_left(devEnv);
        } else if(event->key == InputKeyUp) {
            consumed = bf_dev_process_up(devEnv);
        } else if(event->key == InputKeyDown) {
            consumed = bf_dev_process_down(devEnv);
        }
    } else if(event->key == InputKeyOk) {
        consumed = bf_dev_process_ok(devEnv, event);
    }

    return consumed;
}

static bool bf_dev_process_up(BFDevEnv* devEnv) {
    UNUSED(devEnv);
    selectedButton = buttonMappings[selectedButton].up;
    return true;
}

static bool bf_dev_process_down(BFDevEnv* devEnv) {
    UNUSED(devEnv);
    selectedButton = buttonMappings[selectedButton].down;
    return true;
}

static bool bf_dev_process_left(BFDevEnv* devEnv) {
    UNUSED(devEnv);
    selectedButton = buttonMappings[selectedButton].left;
    return true;
}

static bool bf_dev_process_right(BFDevEnv* devEnv) {
    UNUSED(devEnv);
    selectedButton = buttonMappings[selectedButton].right;
    return true;
}

static bool bf_dev_process_ok(BFDevEnv* devEnv, InputEvent* event) {
    UNUSED(devEnv);
    UNUSED(event);

    if(event->type != InputTypePress){ return false; }

    switch(selectedButton){
        case 0:
        {
            if(appDev->dataSize < BF_INST_BUFFER_SIZE){ 
                appDev->dataBuffer[appDev->dataSize] = (uint32_t)'+'; 
                appDev->dataSize++; }
            break;
        }

        case 1:
        {
            if(appDev->dataSize < BF_INST_BUFFER_SIZE){ 
                appDev->dataBuffer[appDev->dataSize] = (uint32_t)'-'; 
                appDev->dataSize++; }
            break;
        }

        case 2:
        {
            if(appDev->dataSize < BF_INST_BUFFER_SIZE){ 
                appDev->dataBuffer[appDev->dataSize] = (uint32_t)'<'; 
                appDev->dataSize++; }
            break;
        }

        case 3:
        {
            if(appDev->dataSize < BF_INST_BUFFER_SIZE){ 
                appDev->dataBuffer[appDev->dataSize] = (uint32_t)'>'; 
                appDev->dataSize++; }
            break;
        }

        case 4:
        {
            if(appDev->dataSize < BF_INST_BUFFER_SIZE){ 
                appDev->dataBuffer[appDev->dataSize] = (uint32_t)'['; 
                appDev->dataSize++; }
            break;
        }

        case 5:
        {
            if(appDev->dataSize < BF_INST_BUFFER_SIZE){ 
                appDev->dataBuffer[appDev->dataSize] = (uint32_t)']'; 
                appDev->dataSize++; }
            break;
        }

        case 6:
        {
            if(appDev->dataSize < BF_INST_BUFFER_SIZE){ 
                appDev->dataBuffer[appDev->dataSize] = (uint32_t)'.'; 
                appDev->dataSize++; }
            break;
        }

        case 7:
        {
            if(appDev->dataSize < BF_INST_BUFFER_SIZE){ 
                appDev->dataBuffer[appDev->dataSize] = (uint32_t)','; 
                appDev->dataSize++; }
            break;
        }

        case 8:
        {
            if(appDev->dataSize > 0){
                appDev->dataSize--; 
                appDev->dataBuffer[appDev->dataSize] = (uint32_t)0x00;}
            break;
        }

        case 9:
        {
            //todo: input
            scene_manager_next_scene(appDev->scene_manager, brainfuckSceneSetInput);
            break;
        }

        case 10:
        {
            execCountdown = 3;
            break;
        }

        case 11:
        {
            //remove old file
            Storage* storage = furi_record_open(RECORD_STORAGE);
            storage_simply_remove(storage, furi_string_get_cstr(appDev->BF_file_path));

            //save new file
            Stream* stream = buffered_file_stream_alloc(storage);
            buffered_file_stream_open(stream, furi_string_get_cstr(appDev->BF_file_path), FSAM_WRITE, FSOM_CREATE_ALWAYS);
            stream_write(stream, (const uint8_t*)appDev->dataBuffer, appDev->dataSize);
            buffered_file_stream_close(stream);

            //notify
            saveNotifyCountdown = 3;
            break;
        }
    }

    bool consumed = false;
    return consumed;
}

static void bf_dev_enter_callback(void* context) {
    furi_assert(context);
    BFDevEnv* devEnv = context;

    with_view_model(
        devEnv->view,
        BFDevEnvModel* model,
        {
            model->col = 0;
            model->row = 0;
        },
        true);

    appDev = devEnv->appDev;
    selectedButton = 0;

    //clear the bf instruction buffer
    memset(appDev->dataBuffer, 0x00, BF_INST_BUFFER_SIZE * sizeof(char));

    //open the file
    Storage* storage = furi_record_open(RECORD_STORAGE);
    Stream* stream = buffered_file_stream_alloc(storage);
    buffered_file_stream_open(stream, furi_string_get_cstr(appDev->BF_file_path), FSAM_READ, FSOM_OPEN_EXISTING);

    //read into the buffer
    appDev->dataSize = stream_size(stream);
    stream_read(stream, (uint8_t*)appDev->dataBuffer, appDev->dataSize);
    buffered_file_stream_close(stream);

    //replaces any invalid characters with an underscore. strips out newlines, comments, etc
    for(int i = 0; i < appDev->dataSize; i++){
        if(!strchr(bfChars, appDev->dataBuffer[i])){
            appDev->dataBuffer[i] = '_';
        }
    }

    //find the end of the file to begin editing
    int tptr = 0;
    while(appDev->dataBuffer[tptr] != 0x00){ tptr++; }
    appDev->dataSize = tptr;
}

BFDevEnv* bf_dev_env_alloc(BFApp* appDev) {
    BFDevEnv* devEnv = malloc(sizeof(BFDevEnv));

    devEnv->view = view_alloc();
    devEnv->appDev = appDev;
    view_allocate_model(devEnv->view, ViewModelTypeLocking, sizeof(BFDevEnvModel));

    with_view_model(
        devEnv->view,
        BFDevEnvModel* model,
        {
            model->col = 0;
            model->row = 0;
        },
        true);

    view_set_context(devEnv->view, devEnv);
    view_set_draw_callback(devEnv->view, bf_dev_draw_callback);
    view_set_input_callback(devEnv->view, bf_dev_input_callback);
    view_set_enter_callback(devEnv->view, bf_dev_enter_callback);
    return devEnv;
}

void bf_dev_env_free(BFDevEnv* devEnv) {
    furi_assert(devEnv);
    view_free(devEnv->view);
    free(devEnv);
}

View* bf_dev_env_get_view(BFDevEnv* devEnv) {
    furi_assert(devEnv);
    return devEnv->view;
}
