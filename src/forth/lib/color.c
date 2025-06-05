
#include "../forth.h"

#define CONCAT(a, b) a ## b

#define COLOR(name, r, g, b)  \
    static void CONCAT(lib_color_, name)(forth_context_t *ctx, void *closure) { \
        (void) closure; \
        static bool has_tri = false; \
        static tri_t tri; \
        if (!has_tri) { \
            has_tri = true; \
            tri = tri_rgb(ctx->table, (r), (g), (b)); \
        } \
        forth_push_tri(ctx, tri); \
    }
#include "colors.inc"

FORTH_LIBRARY_VAR(color) = {
    #define COLOR(name, r, g, b) { #name, CONCAT(lib_color_, name), NULL },
    #include "colors.inc"
    { NULL, NULL, NULL },
};
