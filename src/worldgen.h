
#pragma once

#include "tris.h"
#include "vector2.h"
#include <stddef.h>

typedef tri_t world_gen_proc_t(void *ctx, tri_bounds_t bounds);

tri_t world_gen_serpinski(tri_table_t *table, size_t n, tri_t color, tri_t object);
tri_t world_gen_serpinski_meta(tri_table_t *table, size_t n, size_t m, tri_t color, tri_t object);
tri_t world_gen_fill(tri_table_t *table, tri_t base, size_t n);
tri_t world_gen_fill_low_mix(tri_table_t *table, tri_t base1, tri_t base2, size_t *ref_gas, size_t n);
tri_t world_gen_proc_range(tri_table_t *table, void *ctx, world_gen_proc_t *proc, size_t n, tri_bounds_t bounds);
tri_t world_gen_proc(tri_table_t *table, void *ctx, world_gen_proc_t *proc, size_t n);
