
#pragma once

#include "../tris.h"

#include <stdbool.h>

typedef struct forth_list_t forth_list_t;
typedef struct forth_object_t forth_object_t;
typedef struct forth_binding_t forth_binding_t;
typedef struct forth_stack_t forth_stack_t;
typedef struct forth_context_t forth_context_t;

enum {
    FORTH_NIL,
    FORTH_NUMBER,
    FORTH_TRIANGLE,
    FORTH_FUNCTION,
};

typedef void (*forth_function_t)(forth_context_t *ctx);

struct forth_object_t {
    uint8_t tag;
    union {
        double number;
        tri_t tri;
        forth_function_t function;
    } data;
};

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

// contexts
forth_context_t *forth_new(tri_table_t *table);
void forth_define(forth_context_t *ctx, const char *name, forth_object_t object);

// stack
void forth_push(forth_context_t *ctx, forth_object_t obj);
forth_object_t forth_pop(forth_context_t *ctx);

// interp
void forth_exec(forth_context_t *ctx, const char *src);

// number objects
bool forth_object_is_number(forth_object_t obj);
forth_object_t forth_object_of_number(double n);
double forth_object_to_number(forth_object_t obj);
void forth_push_number(forth_context_t *ctx, double n);
double forth_pop_number(forth_context_t *ctx);
void forth_define_number(forth_context_t *ctx, const char *name, double n);

// tri objects
bool forth_object_is_tri(forth_object_t obj);
forth_object_t forth_object_of_tri(tri_t tri);
tri_t forth_object_to_tri(forth_object_t obj);
void forth_push_tri(forth_context_t *ctx, tri_t tri);
tri_t forth_pop_tri(forth_context_t *ctx);
void forth_define_tri(forth_context_t *ctx, const char *name, tri_t tri);

// function objects
bool forth_object_is_function(forth_object_t obj);
forth_object_t forth_object_of_function(forth_function_t function);
forth_function_t forth_object_to_function(forth_object_t obj);
void forth_push_function(forth_context_t *ctx, forth_function_t function);
forth_function_t forth_pop_function(forth_context_t *ctx);
void forth_define_function(forth_context_t *ctx, const char *name, forth_function_t function);
