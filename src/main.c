
#include <inttypes.h>

#include "float.h"
#include "camera.h"
#include "worldgen.h"
#include "render.h"
#include "coord.h"

double screen_width = 1920/2;
double screen_height = 1080/2;

#if !defined(TEST_NONE) && !defined(TEST_FILL) && !defined(TEST_DRAW) && !defined(TEST_GEN) && !defined(TEST_FRAC)
#define TEST_FRAC
#endif

double triangle_sign(vector2_t p1, vector2_t p2, vector2_t p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool tri_bounds_has(tri_bounds_t bounds, vector2_t point) {
    double d1 = triangle_sign(point, bounds.top, bounds.left);
    double d2 = triangle_sign(point, bounds.left, bounds.right);
    double d3 = triangle_sign(point, bounds.right, bounds.top);

    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

tri_t tri_subst_points(tri_table_t *table, tri_t tri, size_t n, tri_bounds_t bounds, size_t len, vector2_t *points, tri_t type) {
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

tri_t tri_subst_point(tri_table_t *table, tri_t tri, size_t n, tri_bounds_t bounds, vector2_t point, tri_t type) {
    vector2_t points[1] = { point };
    return tri_subst_points(table, tri, n, bounds, 1, &points[0], type);
}

int main() {
    tri_table_t *table = tri_table_new();
    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screen_width, screen_height, "Trieste");
    camera_t camera = (camera_t) {
        .x = 0,
        .y = -0.2,
        .radius = 1,
    };
    double base_zoom_speed = 1;
    double base_move_speed = 1;

    char lines[16][81] = {0};

    #if defined(TEST_NONE)
        tri_t tri = world_gen(table);
    #elif defined(TEST_FILL)
        size_t mix_value = 0;
        tri_t base1 = STONE;
        tri_t base2 = AIR;
        size_t world_size = 7;
        ptrdiff_t step_size = 1;
        size_t iters_per_frame = 1;
    #elif defined(TEST_DRAW)
        // tri_t next = STONE;
        // tri_t next = world_gen_serpinski_meta(table, 12, 12, STONE, AIR);
        tri_t next = STONE;
        size_t world_size = 64;
        ptrdiff_t brush = 16;
        // camera.radius = 1.0 / pow(4, world_size-10);
        camera.radius = 0.001;
        tri_t tri = world_gen_fill(table, AIR, world_size);
        tri_t on_left = world_gen_serpinski(table, 16, STONE, AIR);
        tri_t on_right = world_gen_serpinski(table, 16, AIR, STONE);
    #elif defined(TEST_FRAC)
        camera.x = 0;
        camera.y = -1;
        camera.radius = 0.1;
        tri_t tri = world_gen_serpinski_opt(table, 24);
    #elif defined(TEST_GEN)
        tri_t tri = world_gen_test();
    #else
        #error test not implemented
    #endif


    vector2_t mouse_pos_last = (vector2_t) {
        .x = 0,
        .y = 0,
    };

    while (!WindowShouldClose()) {
        #if defined(TEST_FILL)
            tri_t tri;
            for (size_t i = 0; i < iters_per_frame; i++) {
                if (mix_value >= (1 << (world_size*2))) {
                    tri_t tmp1 = base1;
                    base1 = base2;
                    base2 = tmp1;
                    mix_value = 0;
                }
                size_t gas = mix_value;
                tri = world_gen_fill_low_mix(table, base1, base2, &gas, world_size);
                mix_value += step_size;
            }
        #endif

        // equal triangle of points on the unit circle
        vector2_t mouse = (vector2_t) {
            .x = GetMousePosition().x,
            .y = GetMousePosition().y,
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

        #if defined(TEST_DRAW)
        if (mouse_pos_last.x != 0 || mouse_pos_last.y != 0) {
            // if (IsKeyPressed(KEY_ONE)) {
            //     brush += 1;
            // } 
            // if (IsKeyPressed(KEY_TWO)) {
            //     brush -= 1;
            // }
            brush = 5+log2(1.0/camera.radius);
            vector2_t points[20];
            for (int i = 0; i < 20; i++) {
                points[i] = vector2_lerp(mouse_pos_last, mouse, (double) i / 20);
            }
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                tri = tri_subst_points(table, tri, brush, bounds, 20, points, on_left);
            }
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                tri = tri_subst_points(table, tri, brush, bounds, 20, points, on_right);
            }
        }
        #endif
        mouse_pos_last = mouse;

        // debug text generation
        {
            size_t line = 0;

            {
                snprintf(lines[line], 80, "FPS: %i\n", GetFPS());
                line += 1;
            }

            #if defined(TEST_DRAW)
                {
                    snprintf(lines[line], 80, "BRUSH: %zi\n", brush);
                    line += 1;
                }
            #endif

            {
                snprintf(lines[line], 80, "TRIS: %"PRIu64"\n", table->id);
                line += 1;
            }
        }

        // zoom & pan
        {
            double zoom_speed = pow(1 + base_zoom_speed, GetFrameTime());
            double move_speed = GetFrameTime() * base_move_speed;
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
                DrawText(lines[i], 10, 10 + 40 * i, 30, BLACK);
            }
        EndDrawing();

    }
    // woo
    TakeScreenshot("out.png");
    CloseWindow();
}
