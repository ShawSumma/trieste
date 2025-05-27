
#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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
    TYPE_AIR,
    TYPE_STONE,
    TYPE_BLUE,
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

static const uint32_t tri_table_primes[] = {
    #define X(name) name,
    #include "primes.inc"
};

static inline tri_table_t *restrict tri_table_new(void) {
    tri_table_t *restrict ret = malloc(sizeof(tri_table_t));
    ret->id = MAX_TYPE;
    ret->prime = tri_table_primes[15];
    ret->tris = calloc(ret->prime, sizeof(tri_t));
    return ret;
}

static inline tri_t tri_join_from_ptr(tri_table_t *table, const tri_t *next) {
    uint64_t hash = 0; 
    hash += next[POS_CENTER].hash * 305971;
    hash += next[POS_TOP].hash * 495071;
    hash += next[POS_LEFT].hash * 801061;
    hash += next[POS_RIGHT].hash * 1296137;

    uint64_t look = 0;
    tri_t *write;
    while (true) {
        write = &table->tris[look % table->prime];
        if (write->type == TYPE_DEAD) {
            break;
        }
        if (
            write->hash == hash
            && write->children[0].id == next[0].id
            && write->children[1].id == next[1].id
            && write->children[2].id == next[2].id
            && write->children[3].id == next[3].id
        ) {
            return *write;
        }
        look += 1;
    }

    write->id = table->id++;
    write->type = TYPE_RECURSIVE;
    write->hash = hash;

    write->children = malloc(sizeof(tri_t) * 4);

    write->children[POS_CENTER] = next[POS_CENTER];
    write->children[POS_TOP] = next[POS_TOP];
    write->children[POS_LEFT] = next[POS_LEFT];
    write->children[POS_RIGHT] = next[POS_RIGHT];

    {
        float value = 0;
        for (int i = 0; i < 4; i++) {
            float part = next[i].color.r * 0.00390625;
            value += part * part;
        }
        write->color.r = sqrt(value) * 128;
    }
    {
        float value = 0;
        for (int i = 0; i < 4; i++) {
            float part = next[i].color.g * 0.00390625;
            value += part * part;
        }
        write->color.g = sqrt(value) * 128;
    }
    {
        float value = 0;
        for (int i = 0; i < 4; i++) {
            float part = next[i].color.b * 0.00390625;
            value += part * part;
        }
        write->color.b = sqrt(value) * 128;
    }

    return *write;
}

static inline tri_t tri_join(tri_table_t *table, tri_t center, tri_t top, tri_t left, tri_t right) {
    tri_t arr[] = {
        [POS_CENTER] = center,
        [POS_TOP] = top,
        [POS_LEFT] = left,
        [POS_RIGHT] = right,
    };
    return tri_join_from_ptr(table, &arr[0]);
}

static const tri_t tri_basic[] = {
    [TYPE_AIR] = (tri_t) {
        .type = TYPE_AIR,
        .color.r = 255,
        .color.g = 255,
        .color.b = 255,
        .hash = TYPE_AIR,
        .id = TYPE_AIR,
    },
    [TYPE_STONE] = (tri_t) {
        .type = TYPE_STONE,
        .color.r = 0,
        .color.g = 0,
        .color.b = 0,
        .hash = TYPE_STONE,
        .id = TYPE_STONE,
    },
};

#define STONE (tri_basic[TYPE_STONE])
#define AIR (tri_basic[TYPE_AIR])
#define JOIN(c, t, l, r) (tri_join(table, c, t, l, r))
