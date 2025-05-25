
#pragma once

#include "tris.h"

static inline tri_t world_gen_serpinski(tri_table_t *table) {
    tri_t ret = AIR;
    tri_t base = STONE;
    for (int i = 0; i < 24; i++) {
        ret = JOIN(base, ret, ret, ret);
        base = JOIN(base, base, base, base);
    }
    return ret;
}

#define world_gen(table) world_gen_serpinski(table)
