/* Copyright (C) 2023 Salvatore Sanfilippo -- All Rights Reserved
 * See the LICENSE file for information about the license. */

#include <furi.h>
#include <furi_hal.h>
#include <storage/storage.h>
#include <input/input.h>
#include <gui/gui.h>
#include <stdlib.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <math.h>
#include <notification/notification.h>
#include <notification/notification_messages.h>

#define TAG "Asteroids" // Used for logging
#define DEBUG_MSG 1
#define SCREEN_XRES 128
#define SCREEN_YRES 64
#define GAME_START_LIVES 3
#define TTLBUL 30 /* Bullet time to live, in ticks. */
#define MAXBUL 5 /* Max bullets on the screen. */
#define MAXAST 32 /* Max asteroids on the screen. */
#define SHIP_HIT_ANIMATION_LEN 15
#define SAVING_DIRECTORY "/ext/apps/Games"
#define SAVING_FILENAME SAVING_DIRECTORY "/game_asteroids.save"
#ifndef PI
#define PI 3.14159265358979f
#endif

/* ============================ Data structures ============================= */

typedef struct Ship {
    float x, /* Ship x position. */
        y, /* Ship y position. */
        vx, /* x velocity. */
        vy, /* y velocity. */
        rot; /* Current rotation. 2*PI full ortation. */
} Ship;

typedef struct Bullet {
    float x, y, vx, vy; /* Fields like in ship. */
    uint32_t ttl; /* Time to live, in ticks. */
} Bullet;

typedef struct Asteroid {
    float x, y, vx, vy, rot, /* Fields like ship. */
        rot_speed, /* Angular velocity (rot speed and sense). */
        size; /* Asteroid size. */
    uint8_t shape_seed; /* Seed to give random shape. */
} Asteroid;

typedef struct AsteroidsApp {
    /* GUI */
    Gui* gui;
    ViewPort* view_port; /* We just use a raw viewport and we render
                                everything into the low level canvas. */
    FuriMessageQueue* event_queue; /* Keypress events go here. */

    /* Game state. */
    int running; /* Once false exists the app. */
    bool gameover; /* Gameover status. */
    uint32_t ticks; /* Game ticks. Increments at each refresh. */
    uint32_t score; /* Game score. */
    uint32_t highscore; /* Highscore. Shown on Game Over Screen */
    bool is_new_highscore; /* Is the last score a new highscore? */
    uint32_t lives; /* Number of lives in the current game. */
    uint32_t ship_hit; /* When non zero, the ship was hit by an asteroid
                               and we need to show an animation as long as
                               its value is non-zero (and decrease it's value
                               at each tick of animation). */

    /* Ship state. */
    struct Ship ship;

    /* Bullets state. */
    struct Bullet bullets[MAXBUL]; /* Each bullet state. */
    int bullets_num; /* Active bullets. */
    uint32_t last_bullet_tick; /* Tick the last bullet was fired. */

    /* Asteroids state. */
    Asteroid asteroids[MAXAST]; /* Each asteroid state. */
    int asteroids_num; /* Active asteroids. */

    uint32_t pressed[InputKeyMAX]; /* pressed[id] is true if pressed.
                                      Each array item contains the time
                                      in milliseconds the key was pressed. */
    bool fire; /* Short press detected: fire a bullet. */
} AsteroidsApp;

const NotificationSequence sequence_thrusters = {
    &message_vibro_on,
    &message_delay_10,
    &message_vibro_off,
    NULL,
};

const NotificationSequence sequence_brake = {
    &message_vibro_on,
    &message_delay_10,
    &message_delay_1,
    &message_delay_1,
    &message_vibro_off,
    NULL,
};

const NotificationSequence sequence_crash = {
    &message_red_255,

    &message_vibro_on,
    // &message_note_g5, // Play sound but currently disabled
    &message_delay_25,
    // &message_note_e5,
    &message_vibro_off,
    &message_sound_off,
    NULL,
};

const NotificationSequence sequence_bullet_fired = {
    &message_vibro_on,
    // &message_note_g5, // Play sound but currently disabled. Need On/Off menu setting
    &message_delay_10,
    &message_delay_1,
    &message_delay_1,
    &message_delay_1,
    &message_delay_1,
    &message_delay_1,

    // &message_note_e5,
    &message_vibro_off,
    &message_sound_off,
    NULL,
};

/* ============================== Prototyeps ================================ */

// Only functions called before their definition are here.
bool load_game(AsteroidsApp* app);
void save_game(AsteroidsApp* app);
void restart_game_after_gameover(AsteroidsApp* app);
uint32_t key_pressed_time(AsteroidsApp* app, InputKey key);

/* ============================ 2D drawing ================================== */

/* This structure represents a polygon of at most POLY_MAX points.
 * The function draw_poly() is able to render it on the screen, rotated
 * by the amount specified. */
#define POLY_MAX 8
typedef struct Poly {
    float x[POLY_MAX];
    float y[POLY_MAX];
    uint32_t points; /* Number of points actually populated. */
} Poly;

/* Define the polygons we use. */
Poly ShipPoly = {{-3, 0, 3}, {-3, 6, -3}, 3};

Poly ShipFirePoly = {{-1.5, 0, 1.5}, {-3, -6, -3}, 3};

/* Rotate the point of the poligon 'poly' and store the new rotated
 * polygon in 'rot'. The polygon is rotated by an angle 'a', with
 * center at 0,0. */
void rotate_poly(Poly* rot, Poly* poly, float a) {
    /* We want to compute sin(a) and cos(a) only one time
     * for every point to rotate. It's a slow operation. */
    float sin_a = (float)sin(a);
    float cos_a = (float)cos(a);
    for(uint32_t j = 0; j < poly->points; j++) {
        rot->x[j] = poly->x[j] * cos_a - poly->y[j] * sin_a;
        rot->y[j] = poly->y[j] * cos_a + poly->x[j] * sin_a;
    }
    rot->points = poly->points;
}

/* This is an 8 bit LFSR we use to generate a predictable and fast
 * pseudorandom sequence of numbers, to give a different shape to
 * each asteroid. */
void lfsr_next(unsigned char* prev) {
    unsigned char lsb = *prev & 1;
    *prev = *prev >> 1;
    if(lsb == 1) *prev ^= 0b11000111;
    *prev ^= *prev << 7; /* Mix things a bit more. */
}

/* Render the polygon 'poly' at x,y, rotated by the specified angle. */
void draw_poly(Canvas* const canvas, Poly* poly, uint8_t x, uint8_t y, float a) {
    Poly rot;
    rotate_poly(&rot, poly, a);
    canvas_set_color(canvas, ColorBlack);
    for(uint32_t j = 0; j < rot.points; j++) {
        uint32_t a = j;
        uint32_t b = j + 1;
        if(b == rot.points) b = 0;
        canvas_draw_line(canvas, x + rot.x[a], y + rot.y[a], x + rot.x[b], y + rot.y[b]);
    }
}

/* A bullet is just a + pixels pattern. A single pixel is not
 * visible enough. */
void draw_bullet(Canvas* const canvas, Bullet* b) {
    canvas_draw_dot(canvas, b->x - 1, b->y);
    canvas_draw_dot(canvas, b->x + 1, b->y);
    canvas_draw_dot(canvas, b->x, b->y);
    canvas_draw_dot(canvas, b->x, b->y - 1);
    canvas_draw_dot(canvas, b->x, b->y + 1);
}

/* Draw an asteroid. The asteroid shapes is computed on the fly and
 * is not stored in a permanent shape structure. In order to generate
 * the shape, we use an initial fixed shape that we resize according
 * to the asteroid size, perturbate according to the asteroid shape
 * seed, and finally draw it rotated of the right amount. */
void draw_asteroid(Canvas* const canvas, Asteroid* ast) {
    Poly ap;

    /* Start with what is kinda of a circle. Note that this could be
     * stored into a template and copied here, to avoid computing
     * sin() / cos(). But the Flipper can handle it without problems. */
    uint8_t r = ast->shape_seed;
    for(int j = 0; j < 8; j++) {
        float a = (PI * 2) / 8 * j;

        /* Before generating the point, to make the shape unique generate
         * a random factor between .7 and 1.3 to scale the distance from
         * the center. However this asteroid should have its unique shape
         * that remains always the same, so we use a predictable PRNG
         * implemented by an 8 bit shift register. */
        lfsr_next(&r);
        float scaling = .7 + ((float)r / 255 * .6);

        ap.x[j] = (float)sin(a) * ast->size * scaling;
        ap.y[j] = (float)cos(a) * ast->size * scaling;
    }
    ap.points = 8;
    draw_poly(canvas, &ap, ast->x, ast->y, ast->rot);
}

/* Draw small ships in the top-right part of the screen, one for
 * each left live. */
void draw_left_lives(Canvas* const canvas, AsteroidsApp* app) {
    int lives = app->lives;
    int x = SCREEN_XRES - 5;

    Poly mini_ship = {{-2, 0, 2}, {-2, 4, -2}, 3};
    while(lives--) {
        draw_poly(canvas, &mini_ship, x, 6, PI);
        x -= 6;
    }
}

/* Given the current position, update it according to the velocity and
 * wrap it back to the other side if the object went over the screen. */
void update_pos_by_velocity(float* x, float* y, float vx, float vy) {
    /* Return back from one side to the other of the screen. */
    *x += vx;
    *y += vy;
    if(*x >= SCREEN_XRES)
        *x = 0;
    else if(*x < 0)
        *x = SCREEN_XRES - 1;
    if(*y >= SCREEN_YRES)
        *y = 0;
    else if(*y < 0)
        *y = SCREEN_YRES - 1;
}

/* Render the current game screen. */
void render_callback(Canvas* const canvas, void* ctx) {
    AsteroidsApp* app = ctx;

    /* Clear screen. */
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_box(canvas, 0, 0, SCREEN_XRES - 1, SCREEN_YRES - 1);

    /* Draw score. */
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontSecondary);
    char score[32];
    snprintf(score, sizeof(score), "%lu", app->score);
    canvas_draw_str(canvas, 0, 8, score);

    /* Draw left ships. */
    draw_left_lives(canvas, app);

    /* Draw ship, asteroids, bullets. */
    draw_poly(canvas, &ShipPoly, app->ship.x, app->ship.y, app->ship.rot);

    if(key_pressed_time(app, InputKeyUp) > 0) {
        notification_message(furi_record_open(RECORD_NOTIFICATION), &sequence_thrusters);
        draw_poly(canvas, &ShipFirePoly, app->ship.x, app->ship.y, app->ship.rot);
    }

    for(int j = 0; j < app->bullets_num; j++) draw_bullet(canvas, &app->bullets[j]);

    for(int j = 0; j < app->asteroids_num; j++) draw_asteroid(canvas, &app->asteroids[j]);

    /* Game over text. */
    if(app->gameover) {
        canvas_set_color(canvas, ColorBlack);
        canvas_set_font(canvas, FontPrimary);

        // TODO: if new highscore, display blinking "New High Score"
        // Display High Score
        if(app->is_new_highscore) {
            canvas_draw_str(canvas, 22, 9, "New High Score!");
        } else {
            canvas_draw_str(canvas, 36, 9, "High Score");
        }

        // Convert highscore to string
        int length = snprintf(NULL, 0, "%lu", app->highscore);
        char* str_high_score = malloc(length + 1);
        snprintf(str_high_score, length + 1, "%lu", app->highscore);

        // Get length to center on screen
        int nDigits = 0;
        if(app->highscore > 0) {
            nDigits = floor(log10(app->highscore)) + 1;
        }

        // Draw highscore centered
        canvas_draw_str(canvas, (SCREEN_XRES / 2) - (nDigits * 2), 20, str_high_score);
        free(str_high_score);

        canvas_draw_str(canvas, 28, 35, "GAME   OVER");
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 25, 50, "Press OK to restart");
    }
}

/* ============================ Game logic ================================== */

float distance(float x1, float y1, float x2, float y2) {
    float dx = x1 - x2;
    float dy = y1 - y2;
    return sqrt(dx * dx + dy * dy);
}

/* Detect a collision between the object at x1,y1 of radius r1 and
 * the object at x2, y2 of radius r2. A factor < 1 will make the
 * function detect the collision even if the objects are yet not
 * relly touching, while a factor > 1 will make it detect the collision
 * only after they are a bit overlapping. It basically is used to
 * rescale the distance.
 *
 * Note that in this simplified 2D world, objects are all considered
 * spheres (this is why this function only takes the radius). This
 * is, after all, kinda accurate for asteroids, for bullets, and
 * even for the ship "core" itself. */
bool objects_are_colliding(float x1, float y1, float r1, float x2, float y2, float r2, float factor) {
    /* The objects are colliding if the distance between object 1 and 2
     * is smaller than the sum of the two radiuses r1 and r2.
     * So it would be like: sqrt((x1-x2)^2+(y1-y2)^2) < r1+r2.
     * However we can avoid computing the sqrt (which is slow) by
     * squaring the second term and removing the square root, making
     * the comparison like this:
     *
     * (x1-x2)^2+(y1-y2)^2 < (r1+r2)^2. */
    float dx = (x1 - x2) * factor;
    float dy = (y1 - y2) * factor;
    float rsum = r1 + r2;
    return dx * dx + dy * dy < rsum * rsum;
}

/* Create a new bullet headed in the same direction of the ship. */
void ship_fire_bullet(AsteroidsApp* app) {
    if(app->bullets_num == MAXBUL) return;
    notification_message(furi_record_open(RECORD_NOTIFICATION), &sequence_bullet_fired);
    Bullet* b = &app->bullets[app->bullets_num];
    b->x = app->ship.x;
    b->y = app->ship.y;
    b->vx = -sin(app->ship.rot);
    b->vy = cos(app->ship.rot);

    /* Ship should fire from its head, not in the middle. */
    b->x += b->vx * 5;
    b->y += b->vy * 5;

    /* Give the bullet some velocity (for now the vector is just
     * normalized to 1). */
    b->vx *= 3;
    b->vy *= 3;

    /* It's more realistic if we add the velocity vector of the
     * ship, too. Otherwise if the ship is going fast the bullets
     * will be slower, which is not how the world works. */
    b->vx += app->ship.vx;
    b->vy += app->ship.vy;

    b->ttl = TTLBUL; /* The bullet will disappear after N ticks. */
    app->bullets_num++;
}

/* Remove the specified bullet by id (index in the array). */
void remove_bullet(AsteroidsApp* app, int bid) {
    /* Replace the top bullet with the empty space left
     * by the removal of this bullet. This way we always take the
     * array dense, which is an advantage when looping. */
    int n = --app->bullets_num;
    if(n && bid != n) app->bullets[bid] = app->bullets[n];
}

/* Create a new asteroid, away from the ship. Return the
 * pointer to the asteroid object, so that the caller can change
 * certain things of the asteroid if needed. */
Asteroid* add_asteroid(AsteroidsApp* app) {
    if(app->asteroids_num == MAXAST) return NULL;
    float size = 4 + rand() % 15;
    float min_distance = 20;
    float x, y;
    do {
        x = rand() % SCREEN_XRES;
        y = rand() % SCREEN_YRES;
    } while(distance(app->ship.x, app->ship.y, x, y) < min_distance + size);
    Asteroid* a = &app->asteroids[app->asteroids_num++];
    a->x = x;
    a->y = y;
    a->vx = 2 * (-.5 + ((float)rand() / RAND_MAX));
    a->vy = 2 * (-.5 + ((float)rand() / RAND_MAX));
    a->size = size;
    a->rot = 0;
    a->rot_speed = ((float)rand() / RAND_MAX) / 10;
    if(app->ticks & 1) a->rot_speed = -(a->rot_speed);
    a->shape_seed = rand() & 255;
    return a;
}

/* Remove the specified asteroid by id (index in the array). */
void remove_asteroid(AsteroidsApp* app, int id) {
    /* Replace the top asteroid with the empty space left
     * by the removal of this one. This way we always take the
     * array dense, which is an advantage when looping. */
    int n = --app->asteroids_num;
    if(n && id != n) app->asteroids[id] = app->asteroids[n];
}

/* Called when an asteroid was reached by a bullet. The asteroid
 * hit is the one with the specified 'id'. */
void asteroid_was_hit(AsteroidsApp* app, int id) {
    float sizelimit = 6; // Smaller than that polverize in one shot.
    Asteroid* a = &app->asteroids[id];

    /* Asteroid is large enough to break into fragments. */
    float size = a->size;
    float x = a->x, y = a->y;
    remove_asteroid(app, id);
    if(size > sizelimit) {
        int max_fragments = size / sizelimit;
        int fragments = 2 + rand() % max_fragments;
        float newsize = size / fragments;
        if(newsize < 2) newsize = 2;
        for(int j = 0; j < fragments; j++) {
            a = add_asteroid(app);
            if(a == NULL) break; // Too many asteroids on screen.
            a->x = x + -(size / 2) + rand() % (int)newsize;
            a->y = y + -(size / 2) + rand() % (int)newsize;
            a->size = newsize;
        }
    } else {
        app->score++;
        if(app->score > app->highscore) {
            app->is_new_highscore = true;
            app->highscore = app->score; // Show on Game Over Screen and future main menu
        }
    }
}

/* Set gameover state. When in game-over mode, the game displays a gameover
 * text with a background of many asteroids floating around. */
void game_over(AsteroidsApp* app) {
    if(app->is_new_highscore) save_game(app); // Save highscore but only on change
    app->gameover = true;
    app->lives = GAME_START_LIVES; // Show 3 lives in game over screen to match new game start
}

/* Function called when a collision between the asteroid and the
 * ship is detected. */
void ship_was_hit(AsteroidsApp* app) {
    app->ship_hit = SHIP_HIT_ANIMATION_LEN;
    if(app->lives) {
        app->lives--;
    } else {
        game_over(app);
    }
}

/* Restart game after the ship is hit. Will reset the ship position, bullets
 * and asteroids to restart the game. */
void restart_game(AsteroidsApp* app) {
    app->ship.x = SCREEN_XRES / 2;
    app->ship.y = SCREEN_YRES / 2;
    app->ship.rot = PI; /* Start headed towards top. */
    app->ship.vx = 0;
    app->ship.vy = 0;
    app->bullets_num = 0;
    app->last_bullet_tick = 0;
    app->asteroids_num = 0;
    app->ship_hit = 0;
}

/* Called after gameover to restart the game. This function
 * also calls restart_game(). */
void restart_game_after_gameover(AsteroidsApp* app) {
    app->gameover = false;
    app->ticks = 0;
    app->score = 0;
    app->is_new_highscore = false;
    app->lives = GAME_START_LIVES - 1;
    restart_game(app);
}

/* Move bullets. */
void update_bullets_position(AsteroidsApp* app) {
    for(int j = 0; j < app->bullets_num; j++) {
        update_pos_by_velocity(
            &app->bullets[j].x, &app->bullets[j].y, app->bullets[j].vx, app->bullets[j].vy);
        if(--app->bullets[j].ttl == 0) {
            remove_bullet(app, j);
            j--; /* Process this bullet index again: the removal will
                    fill it with the top bullet to take the array dense. */
        }
    }
}

/* Move asteroids. */
void update_asteroids_position(AsteroidsApp* app) {
    for(int j = 0; j < app->asteroids_num; j++) {
        update_pos_by_velocity(
            &app->asteroids[j].x, &app->asteroids[j].y, app->asteroids[j].vx, app->asteroids[j].vy);
        app->asteroids[j].rot += app->asteroids[j].rot_speed;
        if(app->asteroids[j].rot < 0)
            app->asteroids[j].rot = 2 * PI;
        else if(app->asteroids[j].rot > 2 * PI)
            app->asteroids[j].rot = 0;
    }
}

/* Collision detection and game state update based on collisions. */
void detect_collisions(AsteroidsApp* app) {
    /* Detect collision between bullet and asteroid. */
    for(int j = 0; j < app->bullets_num; j++) {
        Bullet* b = &app->bullets[j];
        for(int i = 0; i < app->asteroids_num; i++) {
            Asteroid* a = &app->asteroids[i];
            if(objects_are_colliding(a->x, a->y, a->size, b->x, b->y, 1.5, 1)) {
                asteroid_was_hit(app, i);
                remove_bullet(app, j);
                /* The bullet no longer exist. Break the loop.
                 * However we want to start processing from the
                 * same bullet index, since now it is used by
                 * another bullet (see remove_bullet()). */
                j--; /* Scan this j value again. */
                break;
            }
        }
    }

    /* Detect collision between ship and asteroid. */
    for(int j = 0; j < app->asteroids_num; j++) {
        Asteroid* a = &app->asteroids[j];
        if(objects_are_colliding(a->x, a->y, a->size, app->ship.x, app->ship.y, 4, 1)) {
            ship_was_hit(app);
            break;
        }
    }
}

/* This is the main game execution function, called 10 times for
 * second (with the Flipper screen latency, an higher FPS does not
 * make sense). In this function we update the position of objects based
 * on velocity. Detect collisions. Update the score and so forth.
 *
 * Each time this function is called, app->tick is incremented. */
void game_tick(void* ctx) {
    AsteroidsApp* app = ctx;

    /* There are two special screens:
     *
     * 1. Ship was hit, we frozen the game as long as ship_hit isn't zero
     * again, and show an animation of a rotating ship. */
    if(app->ship_hit) {
        notification_message(furi_record_open(RECORD_NOTIFICATION), &sequence_crash);
        app->ship.rot += 0.5;
        app->ship_hit--;
        view_port_update(app->view_port);
        if(app->ship_hit == 0) {
            restart_game(app);
        }
        return;
    } else if(app->gameover) {
        /* 2. Game over. We need to update only background asteroids. In this
     * state the game just displays a GAME OVER text with the floating
     * asteroids in backgroud. */

        if(key_pressed_time(app, InputKeyOk) > 100) {
            restart_game_after_gameover(app);
        }
        update_asteroids_position(app);
        view_port_update(app->view_port);
        return;
    }

    /* Handle keypresses. */
    if(app->pressed[InputKeyLeft]) app->ship.rot -= .35;
    if(app->pressed[InputKeyRight]) app->ship.rot += .35;
    if(app->pressed[InputKeyUp]) {
        app->ship.vx -= 0.5 * (float)sin(app->ship.rot);
        app->ship.vy += 0.5 * (float)cos(app->ship.rot);
    } else if(app->pressed[InputKeyDown]) {
        notification_message(furi_record_open(RECORD_NOTIFICATION), &sequence_brake);
        app->ship.vx *= 0.75;
        app->ship.vy *= 0.75;
    }

    /* Fire a bullet if needed. app->fire is set in
     * asteroids_update_keypress_state() since depends on exact
     * pressure timing. */
    if(app->fire) {
        uint32_t bullet_min_period = 200; // In milliseconds
        uint32_t now = furi_get_tick();
        if(now - app->last_bullet_tick >= bullet_min_period) {
            ship_fire_bullet(app);
            app->last_bullet_tick = now;
        }
        app->fire = false;
    }

    /* Update positions and detect collisions. */
    update_pos_by_velocity(&app->ship.x, &app->ship.y, app->ship.vx, app->ship.vy);
    update_bullets_position(app);
    update_asteroids_position(app);
    detect_collisions(app);

    /* From time to time, create a new asteroid. The more asteroids
     * already on the screen, the smaller probability of creating
     * a new one. */
    if(app->asteroids_num == 0 || (random() % 5000) < (30 / (1 + app->asteroids_num))) {
        add_asteroid(app);
    }

    app->ticks++;
    view_port_update(app->view_port);
}

/* ======================== Flipper specific code =========================== */

bool load_game(AsteroidsApp* app) {
    Storage* storage = furi_record_open(RECORD_STORAGE);

    File* file = storage_file_alloc(storage);
    uint16_t bytes_readed = 0;
    if(storage_file_open(file, SAVING_FILENAME, FSAM_READ, FSOM_OPEN_EXISTING)) {
        bytes_readed = storage_file_read(file, app, sizeof(AsteroidsApp));
    }
    storage_file_close(file);
    storage_file_free(file);

    furi_record_close(RECORD_STORAGE);

    return bytes_readed == sizeof(AsteroidsApp);
}

void save_game(AsteroidsApp* app) {
    Storage* storage = furi_record_open(RECORD_STORAGE);

    if(storage_common_stat(storage, SAVING_DIRECTORY, NULL) == FSE_NOT_EXIST) {
        if(!storage_simply_mkdir(storage, SAVING_DIRECTORY)) {
            return;
        }
    }

    File* file = storage_file_alloc(storage);
    if(storage_file_open(file, SAVING_FILENAME, FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
        storage_file_write(file, app, sizeof(AsteroidsApp));
    }
    storage_file_close(file);
    storage_file_free(file);

    furi_record_close(RECORD_STORAGE);
}

/* Here all we do is putting the events into the queue that will be handled
 * in the while() loop of the app entry point function. */
void input_callback(InputEvent* input_event, void* ctx) {
    AsteroidsApp* app = ctx;
    furi_message_queue_put(app->event_queue, input_event, FuriWaitForever);
}

/* Allocate the application state and initialize a number of stuff.
 * This is called in the entry point to create the application state. */
AsteroidsApp* asteroids_app_alloc() {
    AsteroidsApp* app = malloc(sizeof(AsteroidsApp));

    load_game(app);

    app->gui = furi_record_open(RECORD_GUI);
    app->view_port = view_port_alloc();
    view_port_draw_callback_set(app->view_port, render_callback, app);
    view_port_input_callback_set(app->view_port, input_callback, app);
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);
    app->event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    app->running = 1; /* Turns 0 when back is pressed. */

    restart_game_after_gameover(app);
    memset(app->pressed, 0, sizeof(app->pressed));
    return app;
}

/* Free what the application allocated. It is not clear to me if the
 * Flipper OS, once the application exits, will be able to reclaim space
 * even if we forget to free something here. */
void asteroids_app_free(AsteroidsApp* app) {
    furi_assert(app);

    // View related.
    view_port_enabled_set(app->view_port, false);
    gui_remove_view_port(app->gui, app->view_port);
    view_port_free(app->view_port);
    furi_record_close(RECORD_GUI);
    furi_message_queue_free(app->event_queue);
    app->gui = NULL;

    free(app);
}

/* Return the time in milliseconds the specified key is continuously
 * pressed. Or 0 if it is not pressed. */
uint32_t key_pressed_time(AsteroidsApp* app, InputKey key) {
    return app->pressed[key] == 0 ? 0 : furi_get_tick() - app->pressed[key];
}

/* Handle keys interaction. */
void asteroids_update_keypress_state(AsteroidsApp* app, InputEvent input) {
    // Allow Rapid fire
    if(input.key == InputKeyOk) {
        app->fire = true;
    }

    if(input.type == InputTypePress) {
        app->pressed[input.key] = furi_get_tick();
    } else if(input.type == InputTypeRelease) {
        app->pressed[input.key] = 0;
    }
}

int32_t asteroids_app_entry(void* p) {
    UNUSED(p);
    AsteroidsApp* app = asteroids_app_alloc();

    /* Create a timer. We do data analysis in the callback. */
    FuriTimer* timer = furi_timer_alloc(game_tick, FuriTimerTypePeriodic, app);
    furi_timer_start(timer, furi_kernel_get_tick_frequency() / 10);

    /* This is the main event loop: here we get the events that are pushed
     * in the queue by input_callback(), and process them one after the
     * other. */
    InputEvent input;
    while(app->running) {
        FuriStatus qstat = furi_message_queue_get(app->event_queue, &input, 100);
        if(qstat == FuriStatusOk) {
            if(DEBUG_MSG)
                FURI_LOG_E(TAG, "Main Loop - Input: type %d key %u", input.type, input.key);

            /* Handle navigation here. Then handle view-specific inputs
             * in the view specific handling function. */
            if(input.type == InputTypeLong && input.key == InputKeyBack) {
                // Save High Score even if player didn't finish game
                if(app->is_new_highscore) save_game(app); // Save highscore but only on change
                app->running = 0;
            } else {
                asteroids_update_keypress_state(app, input);
            }
        } else {
            /* Useful to understand if the app is still alive when it
             * does not respond because of bugs. */
            if(DEBUG_MSG) {
                static int c = 0;
                c++;
                if(!(c % 20)) FURI_LOG_E(TAG, "Loop timeout");
            }
        }
    }

    furi_timer_free(timer);
    asteroids_app_free(app);
    return 0;
}
