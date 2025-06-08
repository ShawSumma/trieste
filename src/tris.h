
#pragma once

#include <stdint.h>
#include <string.h>

#include "color.h"
#include "vector2.h"

typedef struct tri_t tri_t;
typedef struct tri_table_t tri_table_t;
typedef struct tri_bounds_t tri_bounds_t;

enum {
    POS_CENTER,
    POS_TOP,
    POS_LEFT,
    POS_RIGHT,
};

enum {
    TYPE_DEAD,
    TYPE_RECURSIVE,
    TYPE_COLOR,
    MAX_TYPE,
};

struct tri_t {
    uint64_t id;
    uint64_t hash;
    color_t color;
    uint32_t type;
    tri_t *children;
};

struct tri_table_t {
    tri_t *tris;
    uint64_t prime;
    uint64_t id;
};

struct tri_bounds_t {
    vector2_t top;
    vector2_t left;
    vector2_t right;
};

tri_table_t *tri_table_new(void);
tri_t tri_join_from_ptr(tri_table_t *table, const tri_t *next);
tri_t tri_join(tri_table_t *table, tri_t center, tri_t top, tri_t left, tri_t right);
tri_t tri_rgb(tri_table_t *table, float r, float g, float b);
tri_t tri_color(tri_table_t *table, color_t color);
