
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
    double x_offset = 0;
    double y_offset = 0;
    double size = 0;
    if (screen_width > screen_height) {
        size = screen_height;
        x_offset = (screen_width - size);
    } else {
        size = screen_width;
        y_offset = (screen_height - size);
    }
    return (vector2_t) {
        .x = double_map(world.x, camera.x - camera.radius, camera.x + camera.radius, 0, size) + x_offset * 0.5,
        .y = double_map(world.y, camera.y - camera.radius, camera.y + camera.radius, 0, size) + y_offset * 0.5,
    };
}

static inline vector2_t screen_to_world(camera_t camera, vector2_t screen) {
    double x_offset = 0;
    double y_offset = 0;
    double size = 0;
    if (screen_width > screen_height) {
        size = screen_height;
    } else {
        size = screen_width;
    }
    return (vector2_t) {
        .x = double_unmap(screen.x - x_offset * 0.5, camera.x - camera.radius, camera.x + camera.radius, 0, size),
        .y = double_unmap(screen.y - y_offset * 0.5, camera.y - camera.radius, camera.y + camera.radius, 0, size),
    };
}
