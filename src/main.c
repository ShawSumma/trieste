
#include <inttypes.h>

#include "float.h"
#include "camera.h"
#include "worldgen.h"
#include "render.h"

#define TURN (PI * 2)

double screen_width = 1920/2;
double screen_height = 1080/2;

int main() {
    tri_table_t *table = tri_table_new();
    tri_t tri = world_gen(table);
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(screen_width, screen_height, "Trieste");
    SetTargetFPS(60);
    printf("Tris: %"PRIu64"\n", table->id);
    camera_t camera = (camera_t) {
        .x = 0,
        .y = 0,
        .radius = 1,
    };
    double base_zoom_speed = 1;
    double base_move_speed = 1;

    while (!WindowShouldClose()) {
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
        BeginDrawing();
        ClearBackground((Color) {180, 200, 200, 255});
        vector2_t top = world_to_screen(camera, (vector2_t) {
            .x = cos(TURN * 3/12),
            .y = -sin(TURN * 3/12),
        });
        vector2_t left = world_to_screen(camera, (vector2_t) {
            .x = cos(TURN * 7/12),
            .y = -sin(TURN * 7/12),
        });
        vector2_t right = world_to_screen(camera, (vector2_t) {
            .x = cos(TURN * 11/12),
            .y = -sin(TURN * 11/12),
        });
        #if 0
            printf("\n");
            printf("%f %f\n", top.x, top.y);
            printf("%f %f\n", left.x, left.y);
            printf("%f %f\n", right.x, right.y);
        #endif
        draw_tri(tri, top, left, right);
        DrawFPS(10, 10);
        EndDrawing();
    }
    TakeScreenshot("out.png");
    CloseWindow();
}
