#pragma once
#include "../brainfuck_i.h"
#include <gui/view.h>

typedef void (*DevEnvOkCallback)(InputType type, void* context);

BFDevEnv* bf_dev_env_alloc(BFApp* application);

void bf_dev_set_file_path(FuriString* path);

void bf_dev_env_free(BFDevEnv* devEnv);

View* bf_dev_env_get_view(BFDevEnv* devEnv);

void bf_dev_env_set_ok(
    BFDevEnv* devEnv,
    DevEnvOkCallback callback,
    void* context);
