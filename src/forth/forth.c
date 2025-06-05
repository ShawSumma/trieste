
#include "forth.h"

#include <stddef.h>
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

void forth_use_library(forth_context_t *ctx, forth_function_t *lib) {
    while (lib->name != NULL) {
        forth_define_function(ctx, lib[0].name, lib[0]);
        lib = &lib[1];
    }
}

void forth_define(forth_context_t *ctx, const char *name, forth_object_t object) {
    size_t name_len = strlen(name);
    forth_binding_t *binding = malloc(sizeof(forth_binding_t) + name_len + 1);
    char *dest = (char *) &binding[1];
    memcpy(dest, name, name_len + 1);
    *binding = (forth_binding_t) {
        .name = dest,
        .object = object,
        .next = ctx->bindings,
    };
    ctx->bindings = binding;
}

forth_object_t *forth_find(forth_context_t *ctx, const char *name) {
    forth_binding_t *binding = ctx->bindings;
    while (true) {
        if (binding == NULL) {
            printf("unknown word: %s\n", name);
            return NULL;
        }
        if (!strcmp(binding->name, name)) {
            return &binding->object;
        }
        binding = binding->next;
    }
}

forth_object_t forth_resolve_tagged(forth_context_t *ctx, const char *name, uint8_t forth_tag) {
    forth_object_t *ref_obj = forth_find(ctx, name);
    if (ref_obj != NULL) {
        forth_object_t obj = *ref_obj;
        while (forth_is_function(obj)) {
            forth_function_t func = forth_to_function(obj);
            func.ptr(ctx, func.closure);
            obj = forth_pop(ctx);
        }
        if (obj.tag == forth_tag) {
            return obj;
        }
    }
    return forth_of_nil();
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
    if (ctx->stack != NULL) {
        forth_object_t obj = ctx->stack->obj;
        forth_stack_t *next = ctx->stack->next;
        free(ctx->stack);
        ctx->stack = next;
        return obj;
    }
    return forth_of_nil();
}

static inline bool forth_char_is_space(char c) {
    return c == ' '
        || c == '\n'
        || c == '\r'
        || c == '\t'
    ;
}

static inline void forth_exec_func(forth_context_t *ctx, void *closure) {
    forth_exec(ctx, -1, (const char *) closure);
}

// interp
void forth_exec(forth_context_t *ctx, ptrdiff_t len_arg, const char *src) {
    (void) ctx;

    size_t len;
    if (len_arg < 0) {
        len = strlen(src);
    } else {
        len = (size_t) len_arg;
    }

    char buf[128];
    size_t head = 0;

    while (true) {
        while (head < len && forth_char_is_space(src[head])) {
            head += 1;
        }
        if (!(head < len)) {
            return;
        }
        if (src[head] == '#') {
            while (head < len && src[head] != '\n') {
                head += 1;
            }
            continue;
        }
        if (src[head] == '{') {
            head += 1;
            const char *start = &src[head];
            size_t depth = 1;
            size_t part_len = 0;
            while (head < len && depth != 0) {
                if (src[head] == '{') {
                    depth += 1;
                }
                if (src[head] == '}') {
                    depth -= 1;
                }
                part_len += 1;
                head += 1;
            }
            part_len -= 1;
            char *buf = malloc(sizeof(char) * (part_len + 1));
            memcpy(buf, start, part_len);
            buf[part_len] = '\0';
            forth_push_function(ctx, (forth_function_t) {
                .name = "lambda",
                .ptr = &forth_exec_func,
                .closure = buf,
            });
            continue;
        }
        {
            size_t index = 0;
            while (!(head >= len) && !forth_char_is_space(src[head])) {
                buf[index++] = src[head];
                head += 1;
            }
            buf[index] = '\0';
        }
        if (buf[0] == '\0' || head >= len) {
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
        } else if (buf[0] == '/') {
            size_t len = strlen(&buf[1]);
            char *ptr = malloc(sizeof(char) * (len + 1));
            memcpy(ptr, &buf[1], len);
            ptr[len] = '\0';
            forth_push_string(ctx, (forth_string_t) {
                .len = len,
                .ptr = ptr,
            });
        } else {
            forth_object_t *obj = forth_find(ctx, buf);
            if (obj == NULL) {
                fprintf(stderr, "unknown word: %s\n", buf);
                break;
            } else if (forth_is_function(*obj)) {
                forth_function_t func = forth_to_function(*obj);
                func.ptr(ctx, func.closure);
            } else {
                forth_push(ctx, *obj);
            }
        }
    }
}
