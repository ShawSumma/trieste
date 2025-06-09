
#pragma once

#include "../tris.h"

#include <stddef.h>
#include <stdbool.h>

typedef struct forth_object_t forth_object_t;
typedef struct forth_string_t forth_string_t;
typedef struct forth_function_t forth_function_t;

typedef struct forth_stack_t forth_stack_t;
typedef struct forth_binding_t forth_binding_t;
typedef struct forth_context_t forth_context_t;

typedef void (*forth_function_pointer_t)(forth_context_t *ctx, void *closure);

/// the name usually the binding name, but doesn't have to be.
struct forth_function_t {
    const char *name;
    forth_function_pointer_t ptr;
    void *closure;
};

struct forth_string_t {
    size_t len;
    const char *ptr;
};

/// any type for forth
struct forth_object_t {
    uint8_t tag;
    union forth_object_data_t {
        void *nil;
        double number;
        tri_t tri;
        forth_function_t function;
        forth_string_t string;
        // As per `type.inc` - add type here
    } data;
};

/// use `forth_define`
struct forth_binding_t {
    const char *name;
    forth_object_t object;
    forth_binding_t *next;
};

struct forth_stack_t {
    forth_object_t obj;
    forth_stack_t *next;
};

struct forth_context_t {
    tri_table_t *table;
    forth_binding_t *bindings;
    forth_stack_t *stack;
};


/// allocates a forth context
forth_context_t *forth_new(tri_table_t *table);
/// runs a file if it's content is not the same as last_data.
char *forth_reload_file(forth_context_t *ctx, const char *path, char *last_data);
/// adds binding, shadows any bindings with same name
void forth_define(forth_context_t *ctx, const char *name, forth_object_t object);
/// find the most recent define with a matching name as a reference,
/// if none can be found returns NULL
forth_object_t *forth_find(forth_context_t *ctx, const char *name);
/// similar to `forth_find`, but calls functions until they resolve to the right type or,
/// if none can be found, returns a nil object
forth_object_t forth_find_typed(forth_context_t *ctx, const char *name, uint8_t forth_type);

// libraries
void forth_use_library(forth_context_t *ctx, forth_function_t *lib);

#define FORTH_LIBRARY_VAR(name) forth_function_t forth_library_ ## name[]
#define FORTH_USE_LIBRARY(ctx, name) \
    { \
        extern FORTH_LIBRARY_VAR(name); \
        forth_use_library((ctx), forth_library_ ## name); \
    }

// stack manipulation
void forth_push(forth_context_t *ctx, forth_object_t obj);
forth_object_t forth_pop(forth_context_t *ctx);

/// Interprets given code over a context
///
/// `forth_exec(my_ctx, 2, "12junk data here :)")`
/// `forth_exec(my_ctx, -1, "/a null terminated string");`
void forth_exec(forth_context_t *ctx, ptrdiff_t len, const char *src);

/// Function for working different kinds of `forth_object_t`.
#define TYPE void
#define NAME nil
#include "type.inc"

#define TYPE double
#define NAME number
#include "type.inc"

#define TYPE tri_t
#define NAME tri
#include "type.inc"

#define TYPE forth_function_t
#define NAME function
#include "type.inc"

#define TYPE forth_string_t
#define NAME string
#include "type.inc"

// As per `type.inc` - add include here

#define CAT(x, y) PREPROC_CAT_(x, y)

#undef CAT
