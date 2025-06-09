
#include "pen.h"

static inline double point_line_sign(vector2_t p1, vector2_t p2, vector2_t p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool tri_bounds_has(tri_bounds_t bounds, vector2_t point) {
    double d1 = point_line_sign(point, bounds.top, bounds.left);
    double d2 = point_line_sign(point, bounds.left, bounds.right);
    double d3 = point_line_sign(point, bounds.right, bounds.top);

    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

tri_t tri_pen_points(tri_table_t *table, tri_t tri, size_t n, tri_bounds_t bounds, size_t len, vector2_t *points, tri_t type) {
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
            tri_pen_points(
                table,
                tri.children[POS_CENTER],
                n-1,
                (tri_bounds_t) { center, top_right, top_left, },
                len,
                points,
                type
            ),
            tri_pen_points(
                table,
                tri.children[POS_TOP],
                n-1,
                (tri_bounds_t) { bounds.top, top_left, top_right, },
                len,
                points,
                type
            ),
            tri_pen_points(
                table,
                tri.children[POS_LEFT],
                n-1,
                (tri_bounds_t) { top_left, bounds.left, center, },
                len,
                points,
                type
            ),
            tri_pen_points(
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
