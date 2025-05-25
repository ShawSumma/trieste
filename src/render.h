
#pragma once

#include "vector2.h"

#include <raylib.h>

static inline void draw_triangle(vector2_t top, vector2_t left, vector2_t right, color_t color) {
    DrawTriangle(
        (Vector2) { top.x, top.y },
        (Vector2) { left.x, left.y },
        (Vector2) { right.x, right.y },
        (Color) { color.r, color.g, color.b, 255 }
    );
}

static inline vector2_t vector2_mid(vector2_t a, vector2_t b) {
    return (vector2_t) {
        .x = (a.x + b.x) * 0.5,
        .y = (a.y + b.y) * 0.5,
    };
}

static inline void draw_tri(tri_t tri, vector2_t top, vector2_t left, vector2_t right) {
    if (tri.type == TYPE_RECURSIVE) {
        if (float_abs(right.x - left.x) < 6) {
            draw_triangle(top, left, right, tri.color);
            return;
        }
        if (left.y > top.y && left.y < 0) {
            return;
        }
        if (top.y > left.y && top.y < 0) {
            return;
        }
        if (left.x > right.x && left.x < 0) {
            return;
        }
        if (right.x > left.x && right.x < 0) {
            return;
        }
        if (left.y < top.y && left.y > screen_height) {
            return;
        }
        if (top.y < left.y && top.y > screen_height) {
            return;
        }
        if (left.x < right.x && left.x > screen_width) {
            return;
        }
        if (right.x < left.x && right.x > screen_width) {
            return;
        }
        vector2_t top_left = vector2_mid(top, left);
        vector2_t top_right = vector2_mid(top, right);
        vector2_t center = vector2_mid(left, right);
        draw_tri(tri.children[POS_CENTER], center, top_right, top_left);
        draw_tri(tri.children[POS_TOP], top, top_left, top_right);
        draw_tri(tri.children[POS_LEFT], top_left, left, center);
        draw_tri(tri.children[POS_RIGHT], top_right, center, right);
    } else {
        draw_triangle(top, left, right, tri.color);
    }
}
