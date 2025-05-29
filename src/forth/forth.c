
#include "forth.h"
#include "../worldgen.h"

#include <stdlib.h>
#include <stdio.h>

// contexts
forth_context_t *forth_new(tri_table_t *table) {
    forth_context_t *ctx = malloc(sizeof(forth_context_t));
    *ctx = (forth_context_t) {
        .table = table,
        .bindings = NULL,
    };
    return ctx;
}

void forth_define(forth_context_t *ctx, const char *name, forth_object_t object) {
    forth_binding_t *binding = malloc(sizeof(forth_binding_t));
    *binding = (forth_binding_t) {
        .name = name,
        .object = object,
        .next = ctx->bindings,
    };
    ctx->bindings = binding;
}

// stack manip
void forth_push(forth_context_t *ctx, forth_object_t obj) {
    forth_stack_t *next = malloc(sizeof(forth_stack_t));
    *next = (forth_stack_t) {
        .next = ctx->stack,
        .obj = obj,
    };
    ctx->stack = next;
}

forth_object_t forth_pop(forth_context_t *ctx) {
    forth_object_t obj = ctx->stack->obj;
    forth_stack_t *next = ctx->stack->next;
    free(ctx->stack);
    ctx->stack = next;
    return obj;
}

// interp
void forth_exec(forth_context_t *ctx, const char *src) {
    (void) ctx;
    char buf[64];
    size_t head = 0;

    while (true) {
        while (src[head] == ' ' || src[head] == '\n') {
            head += 1;
        }
        {
            int res = sscanf(&src[head], "%63[^ \n]s", buf);
            if (res <= 0) {
                break;
            }
        }
        head += strlen(buf);
        if (buf[0] == '\0') {
            break;
        }
        if ('0' <= buf[0] && buf[0] <= '9') {
            double n;
            {
                int res = sscanf(buf, "%lf", &n);
                if (res <= 0) {
                    break;
                }
            }
            forth_push_number(ctx, n);
        } else {
            if (!strcmp(buf, ".")) {
                printf("%.14g\n", forth_pop_number(ctx));
            } else if (!strcmp(buf, "dup")) {
                forth_object_t obj = forth_pop(ctx);
                forth_push(ctx, obj);
                forth_push(ctx, obj);
            } else if (!strcmp(buf, "pop")) {
                forth_pop(ctx);
            } else if (!strcmp(buf, "serp")) {
                double n = forth_pop_number(ctx);
                tri_t object = forth_pop_tri(ctx);
                tri_t color = forth_pop_tri(ctx);
                tri_t value = world_gen_serpinski(ctx->table, (size_t) n, color, object);
                forth_push_tri(ctx, value);
            } else if (!strcmp(buf, "times")) {
                double n = forth_pop_number(ctx);
                forth_object_t obj = forth_pop(ctx);
                for (double x = 0.5; x < n; x += 1.0) {
                    forth_push(ctx, obj);
                }
            } else if (!strcmp(buf, "tri")) {
                tri_t center = forth_pop_tri(ctx);
                tri_t top = forth_pop_tri(ctx);
                tri_t left = forth_pop_tri(ctx);
                tri_t right = forth_pop_tri(ctx);
                forth_push_tri(ctx, tri_join(ctx->table, center, top, left, right));
            } else if (!strcmp(buf, "rgb")) {
                color_t color = (color_t) {
                    .b = (float) forth_pop_number(ctx),
                    .g = (float) forth_pop_number(ctx),
                    .r = (float) forth_pop_number(ctx),
                };
                forth_push_tri(ctx, tri_color(ctx->table, color));
            } else {
                forth_binding_t *binding = ctx->bindings;
                while (true) {
                    if (binding == NULL) {
                        printf("unknown word: %s\n", buf);
                        break;
                    }
                    if (!strcmp(binding->name, buf)) {
                        forth_object_t obj = binding->object;
                        if (forth_object_is_function(obj)) {
                            obj.data.function(ctx);
                        } else {
                            forth_push(ctx, obj);
                        }
                        break;
                    } else {
                        binding = binding->next;
                    }
                }
            }
        }
    }
}

// numbers
bool forth_object_is_number(forth_object_t obj) {
    return obj.tag == FORTH_NUMBER;
}

forth_object_t forth_object_of_number(double n) {
    return (forth_object_t) {
        .tag = FORTH_NUMBER,
        .data.number = n,
    };
}

double forth_object_to_number(forth_object_t obj) {
    return obj.data.number;
}

bool forth_object_is_tri(forth_object_t obj) {
    return obj.tag == FORTH_TRIANGLE;
}

forth_object_t forth_object_of_tri(tri_t tri) {
    return (forth_object_t) {
        .tag = FORTH_TRIANGLE,
        .data.tri = tri,
    };
}

tri_t forth_object_to_tri(forth_object_t obj) {
    return obj.data.tri;
}

void forth_push_number(forth_context_t *ctx, double n) {
    forth_push(ctx, forth_object_of_number(n));
}

double forth_pop_number(forth_context_t *ctx) {
    return forth_object_to_number(forth_pop(ctx));
}

void forth_define_number(forth_context_t *ctx, const char *name, double n) {
    forth_define(ctx, name, forth_object_of_number(n));
}

// tris
void forth_push_tri(forth_context_t *ctx, tri_t tri) {
    forth_push(ctx, forth_object_of_tri(tri));
}

tri_t forth_pop_tri(forth_context_t *ctx) {
    return forth_object_to_tri(forth_pop(ctx));
}

void forth_define_tri(forth_context_t *ctx, const char *name, tri_t tri) {
    forth_define(ctx, name, forth_object_of_tri(tri));
}

// function objects
bool forth_object_is_function(forth_object_t obj) {
    return obj.tag == FORTH_FUNCTION;
}

forth_object_t forth_object_of_function(forth_function_t function) {
    return (forth_object_t) {
        .tag = FORTH_FUNCTION,
        .data.function = function,
    };
}

forth_function_t forth_object_to_function(forth_object_t obj) {
    return obj.data.function;
}

void forth_push_function(forth_context_t *ctx, forth_function_t function) {
    forth_push(ctx, forth_object_of_function(function));
}

forth_function_t forth_pop_function(forth_context_t *ctx) {
    return forth_object_to_function(forth_pop(ctx));
}

void forth_define_function(forth_context_t *ctx, const char *name, forth_function_t function) {
    forth_define(ctx, name, forth_object_of_function(function));
}
