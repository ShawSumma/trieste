
#include "library.h"
#include "forth.h"

void forth_use_library(forth_context_t *ctx, forth_library_t *lib) {
    while (lib->name != NULL) {
        forth_define(ctx, lib->name, lib->object);
        lib = &lib[1];
    }
}

static void color_red(forth_context_t *ctx) {
    forth_push_tri(ctx, tri_rgb(ctx->table, 255, 0, 0));
}

static void color_yellow(forth_context_t *ctx) {
    forth_push_tri(ctx, tri_rgb(ctx->table, 255, 255, 0));
}

static void color_green(forth_context_t *ctx) {
    forth_push_tri(ctx, tri_rgb(ctx->table, 0, 255, 0));
}

static void color_blue(forth_context_t *ctx) {
    forth_push_tri(ctx, tri_rgb(ctx->table, 0, 0, 255));
}

static void color_black(forth_context_t *ctx) {
    forth_push_tri(ctx, tri_rgb(ctx->table, 0, 0, 0));
}

static void color_grey(forth_context_t *ctx) {
    forth_push_tri(ctx, tri_rgb(ctx->table, 120, 120, 130));
}

static void color_white(forth_context_t *ctx) {
    forth_push_tri(ctx, tri_rgb(ctx->table, 255, 255, 255));
}

FORTH_LIBRARY_DEF(forth_library_std)
FORTH_LIBRARY_FUNCTION("red", color_red)
FORTH_LIBRARY_FUNCTION("yellow", color_yellow)
FORTH_LIBRARY_FUNCTION("green", color_green)
FORTH_LIBRARY_FUNCTION("blue", color_blue)
FORTH_LIBRARY_FUNCTION("white", color_white)
FORTH_LIBRARY_FUNCTION("grey", color_grey)
FORTH_LIBRARY_FUNCTION("black", color_black)
FORTH_LIBRARY_END()
