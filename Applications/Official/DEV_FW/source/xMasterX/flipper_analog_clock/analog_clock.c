#include <math.h>
#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <input/input.h>
#include <locale/locale.h>

#define PI 3.14

typedef struct {
    uint8_t x;
    uint8_t y;
} Vector2;

typedef enum {
    EventTypeTick,
    EventTypeInput,
} EventType;

typedef struct {
    EventType type;
    InputEvent input;
} AppEvent;

typedef struct {
    FuriMutex* mutex;
    FuriHalRtcDateTime date_time;
} ClockApp;

static Vector2 angle_to_vector2(float angle_in_degrees, uint8_t distance, Vector2 center) {
    float radians = (angle_in_degrees - 90) * (PI / 180);

    Vector2 vec = {
        .x = center.x + cos(radians) * distance,
        .y = center.y + sin(radians) * distance,
    };

    return vec;
}

static void analog_clock_app_draw_callback(Canvas* canvas, void* context) {
    furi_assert(context);
    ClockApp* app = context;
    furi_mutex_acquire(app->mutex, FuriWaitForever);

    uint8_t width = canvas_width(canvas);
    uint8_t height = canvas_height(canvas);
    Vector2 clock_center = {
        .x = 28 + width / 2,
        .y = height / 2,
    };
    uint8_t radius = MIN(width, height) / 2 - 2;

    canvas_draw_circle(canvas, clock_center.x, clock_center.y, radius);

    FuriString* str = furi_string_alloc();

    for(uint8_t i = 3; i <= 12; i += 3) {
        Vector2 pos = angle_to_vector2(360 / 12 * i, radius - 4, clock_center);

        furi_string_printf(str, "%i", i);

        canvas_draw_str_aligned(
            canvas, pos.x, pos.y, AlignCenter, AlignCenter, furi_string_get_cstr(str));
    }

    Vector2 hour_vec =
        angle_to_vector2(((app->date_time.hour % 12) / 12.f * 360.f), radius - 8, clock_center);
    canvas_draw_line(canvas, clock_center.x, clock_center.y, hour_vec.x, hour_vec.y);

    Vector2 minute_vec =
        angle_to_vector2((app->date_time.minute / 60.f * 360.f), radius - 4, clock_center);
    canvas_draw_line(canvas, clock_center.x, clock_center.y, minute_vec.x, minute_vec.y);

    Vector2 second_vec =
        angle_to_vector2((app->date_time.second / 60.f * 360.f), radius - 2, clock_center);
    canvas_draw_line(canvas, clock_center.x, clock_center.y, second_vec.x, second_vec.y);

    canvas_set_font(canvas, FontSecondary);

    locale_format_date(str, &app->date_time, locale_get_date_format(), ".");
    uint16_t date_str_width = canvas_string_width(canvas, furi_string_get_cstr(str));
    canvas_draw_frame(canvas, 0, 51, date_str_width + 6, 13);
    canvas_draw_str(canvas, 3, 61, furi_string_get_cstr(str));

    furi_string_free(str);
    furi_mutex_release(app->mutex);
}

static void analog_clock_app_input_callback(InputEvent* event, void* context) {
    furi_assert(context);
    FuriMessageQueue* event_queue = context;
    AppEvent app_event = {.type = EventTypeInput, .input = *event};
    furi_message_queue_put(event_queue, &app_event, FuriWaitForever);
}

static void analog_clock_app_tick(void* context) {
    furi_assert(context);
    FuriMessageQueue* event_queue = context;
    AppEvent app_event = {.type = EventTypeTick};
    furi_message_queue_put(event_queue, &app_event, 0);
}

int32_t analog_clock_app(void* p) {
    UNUSED(p);

    ClockApp* app = malloc(sizeof(ClockApp));
    furi_hal_rtc_get_datetime(&app->date_time);

    app->mutex = furi_mutex_alloc(FuriMutexTypeNormal);

    FuriMessageQueue* event_queue = furi_message_queue_alloc(8, sizeof(AppEvent));

    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, analog_clock_app_draw_callback, app);
    view_port_input_callback_set(view_port, analog_clock_app_input_callback, event_queue);

    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    FuriTimer* timer = furi_timer_alloc(analog_clock_app_tick, FuriTimerTypePeriodic, event_queue);
    furi_timer_start(timer, furi_kernel_get_tick_frequency() / 10); // 10 times per second

    AppEvent event;
    for(bool running = true; running;) {
        FuriStatus event_status = furi_message_queue_get(event_queue, &event, 100);
        if(event_status == FuriStatusOk) {
            if(event.type == EventTypeInput) {
                if(event.input.type == InputTypeShort && event.input.key == InputKeyBack) {
                    running = false;
                }
            } else if(event.type == EventTypeTick) {
                furi_mutex_acquire(app->mutex, FuriWaitForever);

                furi_hal_rtc_get_datetime(&app->date_time);

                furi_mutex_release(app->mutex);
                view_port_update(view_port);
            }
        }
    }

    furi_timer_free(timer);

    gui_remove_view_port(gui, view_port);
    furi_record_close(RECORD_GUI);
    view_port_free(view_port);
    furi_message_queue_free(event_queue);

    furi_mutex_free(app->mutex);
    free(app);

    return 0;
}
