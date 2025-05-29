
#include "tris.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

tri_table_t *tri_table_new(void) {
    tri_table_t *ret = malloc(sizeof(tri_table_t));
    ret->id = MAX_TYPE;
    ret->prime = 1000 * 1000 * 10;
    ret->tris = calloc(ret->prime, sizeof(tri_t));
    return ret;
}

tri_t tri_join_from_ptr(tri_table_t *table, const tri_t *next) {
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

tri_t tri_join(tri_table_t *table, tri_t center, tri_t top, tri_t left, tri_t right) {
    tri_t arr[] = {
        [POS_CENTER] = center,
        [POS_TOP] = top,
        [POS_LEFT] = left,
        [POS_RIGHT] = right,
    };
    return tri_join_from_ptr(table, &arr[0]);
}

tri_t tri_color(tri_table_t *table, color_t color) {
    uint64_t id = table->id++;
    return (tri_t) {
        .id = id,
        .hash = id,
        .color = color,
        .type = TYPE_COLOR,
    };
}

tri_t tri_rgb(tri_table_t *table, float r, float g, float b) {
    return tri_color(table, (color_t) { r, g, b });
}
