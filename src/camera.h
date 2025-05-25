
#pragma once

#include "vector2.h"

extern double screen_width;
extern double screen_height;

typedef struct camera_t camera_t;

struct camera_t {
    double x;
    double y;
    double radius;
};

static inline vector2_t world_to_screen(camera_t camera, vector2_t world) {
    double size = double_min(screen_width, screen_height);
    return (vector2_t) {
        .x = double_map(world.x, camera.x - 1*camera.radius, camera.x + 1*camera.radius, 0, size) + (screen_height - size) * 0.5,
        .y = double_map(world.y, camera.y - 1*camera.radius, camera.y + 1*camera.radius, 0, size) + (screen_height - size) * 0.5,
    };
}

static inline vector2_t screen_to_world(camera_t camera, vector2_t screen) {
    double size = double_min(screen_width, screen_height);
    return (vector2_t) {
        .x = double_unmap(screen.x - (screen_width - size) * 0.5, camera.x - 1*camera.radius, camera.x + 1*camera.radius, 0, size),
        .y = double_unmap(screen.y - (screen_width - size) * 0.5,  camera.y - 1*camera.radius, camera.y + 1*camera.radius, 0, size),
    };
}
