
#include "../vector2.h"
#include "../tris.h"
#include "../forth/forth.h"
#include <stdbool.h>
#include <stdio.h>

bool tri_bounds_has(tri_bounds_t bounds, vector2_t point);
tri_t tri_pen_points(tri_table_t *table, tri_t tri, size_t n, tri_bounds_t bounds, size_t len, vector2_t *points, tri_t type);
