#include <gui/scene_manager.h>

// Generate scene id and total number
#define ADD_SCENE(prefix, name, id) FlippPomodoroScene##id,
typedef enum
{
#include "config/flipp_pomodoro_scene_config.h"
    FlippPomodoroSceneNum,
} FlippPomodoroScene;
#undef ADD_SCENE

extern const SceneManagerHandlers flipp_pomodoro_scene_handlers;

// Generate scene on_enter handlers declaration
#define ADD_SCENE(prefix, name, id) void prefix##_scene_##name##_on_enter(void *);
#include "config/flipp_pomodoro_scene_config.h"
#undef ADD_SCENE

// Generate scene on_event handlers declaration
#define ADD_SCENE(prefix, name, id) \
    bool prefix##_scene_##name##_on_event(void *context, SceneManagerEvent event);
#include "config/flipp_pomodoro_scene_config.h"
#undef ADD_SCENE

// Generate scene on_exit handlers declaration
#define ADD_SCENE(prefix, name, id) void prefix##_scene_##name##_on_exit(void *context);
#include "config/flipp_pomodoro_scene_config.h"
#undef ADD_SCENE
