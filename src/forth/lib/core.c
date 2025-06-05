
#include "../forth.h"
#include "../../worldgen.h"

#include <stdio.h>

static inline void forth_core_print(forth_context_t *ctx, void *closure) {
    (void) closure;
    forth_object_t obj = forth_pop(ctx);
    if (forth_is_number(obj)) {
        printf("%.14g", forth_to_number(obj));
    } else if (forth_is_tri(obj)) {
        tri_t tri = forth_to_tri(obj);
        color_t color = tri.color;
        printf("<tri: %.14g %.14g %.14g>", color.r, color.g, color.b);
    } else if (forth_is_function(obj)) {
        printf("<func: %s>", forth_to_function(obj).name);
    } else if (forth_is_string(obj)) {
        forth_string_t str = forth_to_string(obj);
        printf("<str: %zu {%.*s}>", str.len, (int) str.len, str.ptr);
    } else {
        printf("<unknown>");
    }
    printf("\n");
}

static inline void forth_core_dup(forth_context_t *ctx, void *closure) {
    (void) closure;
    forth_object_t obj = forth_pop(ctx);
    forth_push(ctx, obj);
    forth_push(ctx, obj);
}

static inline void forth_core_pop(forth_context_t *ctx, void *closure) {
    (void) closure;
    (void) forth_pop(ctx);
}

static inline void forth_core_serp(forth_context_t *ctx, void *closure) {
    (void) closure;
    double n = forth_pop_number(ctx);
    tri_t color = forth_pop_tri(ctx);
    tri_t object = forth_pop_tri(ctx);
    tri_t serp = world_gen_serpinski(ctx->table, (size_t) n, color, object);
    forth_push_tri(ctx, serp);
}

static inline void forth_core_times(forth_context_t *ctx, void *closure) {
    (void) closure;
    double n = forth_pop_number(ctx);
    forth_object_t obj = forth_pop(ctx);
    for (double i = 0.5; i < n; i++) {
        forth_push(ctx, obj);
    }
}

static inline void forth_core_tri(forth_context_t *ctx, void *closure) {
    (void) closure;
    tri_t center = forth_pop_tri(ctx);
    tri_t top = forth_pop_tri(ctx);
    tri_t left = forth_pop_tri(ctx);
    tri_t right = forth_pop_tri(ctx);
    tri_t tri = tri_join(ctx->table, center, top, left, right);
    forth_push_tri(ctx, tri);
}

static inline void forth_core_rgb(forth_context_t *ctx, void *closure) {
    (void) closure;
    double b = forth_pop_number(ctx);
    double g = forth_pop_number(ctx);
    double r = forth_pop_number(ctx);
    tri_t tri = tri_rgb(ctx->table, (float) r, (float) g, (float) b);
    forth_push_tri(ctx, tri);
}

static inline void forth_core_fill(forth_context_t *ctx, void *closure) {
    (void) closure;
    double n = forth_pop_number(ctx);
    tri_t base = forth_pop_tri(ctx);
    tri_t tri = world_gen_fill(ctx->table, base, (size_t) n);
    forth_push_tri(ctx, tri);
}

static inline void forth_core_def(forth_context_t *ctx, void *closure) {
    (void) closure;
    forth_object_t obj = forth_pop(ctx);
    forth_string_t str = forth_pop_string(ctx);
    forth_define(ctx, str.ptr, obj);
}

FORTH_LIBRARY_VAR(core) = {
    { ".", forth_core_print, NULL },
    { "dup", forth_core_dup, NULL },
    { "pop", forth_core_pop, NULL },
    { "serp", forth_core_serp, NULL },
    { "times", forth_core_times, NULL },
    { "tri", forth_core_tri, NULL },
    { "rgb", forth_core_rgb, NULL },
    { "fill", forth_core_fill, NULL },
    { "def", forth_core_def, NULL },
    { NULL, NULL, NULL },
};
