
#include "worldgen.h"

tri_t world_gen_serpinski(tri_table_t *table, size_t n, tri_t color, tri_t object) {
    tri_t ret = object;
    tri_t base = color;
    for (size_t i = 0; i < n; i++) {
        ret = tri_join(table, base, ret, ret, ret);
        base = tri_join(table, base, base, base, base);
    }
    return ret;
}

tri_t world_gen_serpinski_meta(tri_table_t *table, size_t n, size_t m, tri_t color, tri_t object) {
    tri_t a = color;
    tri_t b = object;
    for (size_t i = 0; i < n; i++) {
        tri_t old_a = a;
        tri_t old_b = b;
        a = world_gen_serpinski(table, m, old_a, old_b);
        b = world_gen_serpinski(table, m, old_b, old_a);
    }
    return a;
}

tri_t world_gen_fill(tri_table_t *table, tri_t base, size_t n) {
    tri_t ret = base;
    for (size_t i = 0; i < n; i++) {
        ret = tri_join(table, ret, ret, ret, ret);
    }
    return ret;
}

tri_t world_gen_fill_low_mix(tri_table_t *table, tri_t base1, tri_t base2, size_t *ref_gas, size_t n) {
    if (n == 0) {
        if (*ref_gas > 0) {
            *ref_gas -= 1;
            return base1;
        } else {
            return base2;
        }
    } else {
        if (*ref_gas >= ((size_t)1 << (2*n))) {
            *ref_gas -= ((size_t)1 << (2*n));
            return world_gen_fill(table, base1, n);
        } else if (*ref_gas > 0) {
            tri_t center = world_gen_fill_low_mix(table, base1, base2, ref_gas, n-1);
            tri_t top = world_gen_fill_low_mix(table, base1, base2, ref_gas, n-1);
            tri_t left = world_gen_fill_low_mix(table, base1, base2, ref_gas, n-1);
            tri_t right = world_gen_fill_low_mix(table, base1, base2, ref_gas, n-1);
            return tri_join(table, center, top, left, right);
        } else {
            return world_gen_fill(table, base2, n);
        }
    }
}

tri_t world_gen_proc_range(tri_table_t *table, void *ctx, world_gen_proc_t *proc, size_t n, tri_bounds_t bounds) {
    if (n == 0) {
        return proc(ctx, bounds);
    } else {
        vector2_t top_left = vector2_mid(bounds.top, bounds.left);
        vector2_t top_right = vector2_mid(bounds.top, bounds.right);
        vector2_t center = vector2_mid(bounds.left, bounds.right);
        return tri_join(
            table,
            world_gen_proc_range(
                table,
                ctx, proc,
                n-1,
                (tri_bounds_t) {
                    center,
                    top_right,
                    top_left,
                }
            ),
            world_gen_proc_range(
                table,
                ctx, proc,
                n-1,
                (tri_bounds_t) {
                    bounds.top,
                    top_left,
                    top_right,
                }
            ),
            world_gen_proc_range(
                table,
                ctx, proc,
                n-1,
                (tri_bounds_t) {
                    top_left,
                    bounds.left,
                center,
                }
            ),
            world_gen_proc_range(
                table,
                ctx, proc,
                n-1,
                (tri_bounds_t) {
                    top_right,
                    center,
                    bounds.right,
                }
            )
        );
    }
}

tri_t world_gen_proc(tri_table_t *table, void *ctx, world_gen_proc_t *proc, size_t n) {
    return world_gen_proc_range(
        table,
        ctx,
        proc,
        n,
        (tri_bounds_t) {
            .top.x = 0,
            .top.y = -1,
            .left.x = -COS_7TURN_OVER_12,
            .left.y = 0.5,
            .right.x = COS_7TURN_OVER_12,
            .right.y = 0.5,
        }
    );
}
