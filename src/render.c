
#include "render.h"
#include "../raylib/include/raylib.h"

void render_draw_triangle(tri_bounds_t bounds, color_t color) {
    DrawTriangle(
        (Vector2) { (float) bounds.top.x, (float) bounds.top.y },
        (Vector2) { (float) bounds.left.x, (float) bounds.left.y },
        (Vector2) { (float) bounds.right.x, (float) bounds.right.y },
        (Color) { (uint8_t) color.r, (uint8_t) color.g, (uint8_t) color.b, 255 }
    );
    // render_set_pixel(top.x, top.y, color);
}

void render_draw_tri(tri_t tri, tri_bounds_t bounds) {
    if (tri.type == TYPE_RECURSIVE) {
        if (double_abs(bounds.right.x - bounds.left.x) < 5) {
            render_draw_triangle(bounds, tri.color);
            return;
        }
        if (bounds.left.y > bounds.top.y && bounds.left.y < 0) {
            return;
        }
        if (bounds.top.y > bounds.left.y && bounds.top.y < 0) {
            return;
        }
        if (bounds.left.x > bounds.right.x && bounds.left.x < 0) {
            return;
        }
        if (bounds.right.x > bounds.left.x && bounds.right.x < 0) {
            return;
        }
        if (bounds.left.y < bounds.top.y && bounds.left.y >= screen_height) {
            return;
        }
        if (bounds.top.y < bounds.left.y && bounds.top.y >= screen_height) {
            return;
        }
        if (bounds.left.x < bounds.right.x && bounds.left.x >= screen_width) {
            return;
        }
        if (bounds.right.x < bounds.left.x && bounds.right.x >= screen_width) {
            return;
        }
        vector2_t top_left = vector2_mid(bounds.top, bounds.left);
        vector2_t top_right = vector2_mid(bounds.top, bounds.right);
        vector2_t center = vector2_mid(bounds.left, bounds.right);
        render_draw_tri(
            tri.children[POS_CENTER],
            (tri_bounds_t) {
                center,
                top_right,
                top_left,
            }
        );
        render_draw_tri(
            tri.children[POS_TOP],
            (tri_bounds_t) {
                bounds.top,
                top_left,
                top_right,
            }
        );
        render_draw_tri(
            tri.children[POS_LEFT],
            (tri_bounds_t) {
                top_left,
                bounds.left,
                center,
            }
        );
        render_draw_tri(
            tri.children[POS_RIGHT],
            (tri_bounds_t) {
                top_right,
                center,
                bounds.right,
            }
        );
    } else {
        render_draw_triangle(bounds, tri.color);
    }
}
