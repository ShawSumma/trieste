
#pragma once

#include "vector2.h"
#include "tris.h"

extern double screen_width;
extern double screen_height;

void render_draw_triangle(tri_bounds_t bounds, color_t color);
void render_draw_tri(tri_t tri, tri_bounds_t bounds);
