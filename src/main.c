
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include "../raylib/include/raylib.h"

#include "float.h"
#include "camera.h"
#include "render.h"
#include "forth/forth.h"
#include "serial/serial.h"
#include "vector2.h"

extern double screen_width;
extern double screen_height;

static inline double triangle_sign(vector2_t p1, vector2_t p2, vector2_t p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

static inline bool tri_bounds_has(tri_bounds_t bounds, vector2_t point) {
    double d1 = triangle_sign(point, bounds.top, bounds.left);
    double d2 = triangle_sign(point, bounds.left, bounds.right);
    double d3 = triangle_sign(point, bounds.right, bounds.top);

    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

static inline tri_t tri_subst_points(tri_table_t *table, tri_t tri, size_t n, tri_bounds_t bounds, size_t len, vector2_t *points, tri_t type) {
    bool skip = true;
    for (size_t i = 0; i < len; i++) {
        if (tri_bounds_has(bounds, points[i])) {
            skip = false;
            break;
        }
    }
    if (skip) {
        return tri;
    }
    if (tri.type == TYPE_RECURSIVE && n > 0) {
        vector2_t top_left = vector2_mid(bounds.top, bounds.left);
        vector2_t top_right = vector2_mid(bounds.top, bounds.right);
        vector2_t center = vector2_mid(bounds.left, bounds.right);
        return tri_join(
            table,
            tri_subst_points(
                table,
                tri.children[POS_CENTER],
                n-1,
                (tri_bounds_t) { center, top_right, top_left, },
                len,
                points,
                type
            ),
            tri_subst_points(
                table,
                tri.children[POS_TOP],
                n-1,
                (tri_bounds_t) { bounds.top, top_left, top_right, },
                len,
                points,
                type
            ),
            tri_subst_points(
                table,
                tri.children[POS_LEFT],
                n-1,
                (tri_bounds_t) { top_left, bounds.left, center, },
                len,
                points,
                type
            ),
            tri_subst_points(
                table,
                tri.children[POS_RIGHT],
                n-1,
                (tri_bounds_t) { top_right, center, bounds.right, },
                len,
                points,
                type
            )
        );
    } else {
        return type;
    }
}

static inline char *main_reload_file(forth_context_t *ctx, const char *path, char *last_data) {
    FILE *file = fopen(path, "r");
    fseek(file, 0, SEEK_END);
    size_t len = (size_t) ftell(file);
    fseek(file, 0, SEEK_SET);
    char *ret = malloc(len + 1);
    fread(ret, 1, len, file);
    ret[len] = '\0';

    if (last_data == NULL || !!strcmp(last_data, ret)) {
        forth_exec(ctx, (ptrdiff_t) len, ret);
    }

    if (last_data != NULL) {
        free((void *) last_data);
    }

    return (char *) ret;
}

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    // test();
    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow((int) screen_width, (int) screen_height, "Trieste");
    camera_t camera = (camera_t) {
        .x = 0,
        .y = -0.2,
        .radius = 0.1,
    };
    double base_zoom_speed = 10;
    double base_move_speed = 1;

    char lines[16][81] = {0};

    tri_table_t *table = tri_table_new();

    vector2_t mouse_pos_last = (vector2_t) {
        .x = 0,
        .y = 0,
    };
    (void) mouse_pos_last;

    tri_t black = tri_rgb(table, 0, 0, 0);
    tri_t white = tri_rgb(table, 255, 255, 255);
    (void) black;
    (void) white;

    const char *load_path = "test.trieste";
    tri_t tri = tri_rgb(table, 128, 64, 64);

    forth_context_t *ctx = forth_new(table);
    FORTH_USE_LIBARRY(ctx, core);
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-f")) {
            i += 1;
            load_path = argv[i];
        } else {
            forth_exec(ctx, -1, argv[i]);
        }
    }

    char *last_data = main_reload_file(ctx, load_path, NULL);

    forth_object_t obj = forth_find_typed(ctx, "on-init", forth_type_tri());
    if (obj.tag == forth_type_tri()) {
        tri = forth_to_tri(obj);
    }

    Font font = LoadFontEx("./UbuntuMono-B.ttf", 72, NULL, 0);

    size_t n = 0;

    while (!WindowShouldClose()) {
        screen_height = GetScreenHeight();
        screen_width = GetScreenWidth();

        // equal triangle of points on the unit circle
        vector2_t mouse = (vector2_t) {
            .x = (double) GetMousePosition().x,
            .y = (double) GetMousePosition().y,
        };

        tri_bounds_t bounds = (tri_bounds_t) {
            .top = world_to_screen(camera, (vector2_t) {
                .x = 0,
                .y = -1,
            }),
            .left = world_to_screen(camera, (vector2_t) {
                .x = -COS_7TURN_OVER_12,
                .y = 0.5,
            }),
            .right = world_to_screen(camera, (vector2_t) {
                .x = COS_7TURN_OVER_12,
                .y = 0.5,
            }),
        };

        if (IsKeyPressed(KEY_T)) {
            FILE *out = fopen("snap.trieste", "w");
            if (out != NULL) {
                file_write_tri(out, table, tri);
                fclose(out);
            }
        }

        if (IsMouseButtonPressed(KEY_R) || n % 60 == 0) {
            last_data = main_reload_file(ctx, load_path, last_data);
        }

        n += 1;

        if (mouse_pos_last.x != 0 || mouse_pos_last.y != 0) {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                size_t brush_size = (size_t) (5.0 + log2(1.0 / camera.radius));
                vector2_t points[20];
                for (int i = 0; i < 20; i++) {
                    points[i] = vector2_lerp(mouse_pos_last, mouse, (double) i / 20);
                }
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    forth_object_t on_left = forth_find_typed(ctx, "on-left", forth_type_tri());
                    if (on_left.tag == forth_type_tri()) {
                        tri = tri_subst_points(table, tri, brush_size, bounds, 20, points, forth_to_tri(on_left));
                    }
                }
                if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                    forth_object_t on_right = forth_find_typed(ctx, "on-right", forth_type_tri());
                    if (on_right.tag == forth_type_tri()) {
                        tri = tri_subst_points(table, tri, brush_size, bounds, 20, points, forth_to_tri(on_right));
                    }
                }
            }
        }

        mouse_pos_last = mouse;

        // debug text generation
        {
            size_t line = 0;

            {
                snprintf(lines[line], 80, "FPS: %i\n", GetFPS());
                line += 1;
            }

            // {
            //     snprintf(lines[line], 80, "TRIS: %"PRIu64"\n", table->id);
            //     line += 1;
            // }
        }

        // zoom & pan
        {
            double zoom_speed = pow(1 + base_zoom_speed, (double) GetFrameTime());
            double move_speed = (double) GetFrameTime() * base_move_speed;
            if (IsKeyDown(KEY_Q)) {
                camera.radius /= zoom_speed;
            }
            if (IsKeyDown(KEY_E)) {
                camera.radius *= zoom_speed;
            }
            if (IsKeyDown(KEY_W)) {
                camera.y -= move_speed * camera.radius;
            }
            if (IsKeyDown(KEY_S)) {
                camera.y += move_speed * camera.radius;
            }
            if (IsKeyDown(KEY_A)) {
                camera.x -= move_speed * camera.radius;
            }
            if (IsKeyDown(KEY_D)) {
                camera.x += move_speed * camera.radius;
            }
        }

        BeginDrawing();
            ClearBackground((Color) {180, 200, 200, 255});
            {
                render_draw_tri(tri, bounds);
            }
            // debug text render
            for (int i = 0; i < 16; i++) {
                DrawTextEx(font, lines[i], (Vector2) { 10, (float) (10 + 40 * i) }, 30, 5, BLACK);
            }
        EndDrawing();

    }

    FILE *out = fopen("snap.trieste", "w");
    if (out != NULL) {
        file_write_tri(out, table, tri);
        fclose(out);
    }

    TakeScreenshot("snap.png");
    CloseWindow();

    return 0;
}
