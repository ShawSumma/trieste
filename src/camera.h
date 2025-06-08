
#pragma once

typedef struct camera_t camera_t;

#include "vector2.h"

struct camera_t {
    double x;
    double y;
    double radius;
};

vector2_t world_to_screen(camera_t camera, vector2_t world);
vector2_t screen_to_world(camera_t camera, vector2_t screen);

extern double screen_width;
extern double screen_height;
