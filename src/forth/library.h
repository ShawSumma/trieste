
#pragma once

#include "forth.h"

typedef struct forth_library_t forth_library_t;

/// a member of a library
struct forth_library_t {
    const char *name;
    forth_object_t object;
};

/// library of color names
extern forth_library_t forth_library_std[];

void forth_use_library(forth_context_t *ctx, forth_library_t *lib);

#define FORTH_LIBRARY_DEF(NAME) forth_library_t NAME[] = {
#define FORTH_LIBRARY_NUMBER(NAME, VALUE) (forth_library_t) { .name = (NAME), .object.tag = FORTH_NUMBER, .object.data.number = (double) (VALUE), },
#define FORTH_LIBRARY_FUNCTION(NAME, VALUE) (forth_library_t) { .name = (NAME), .object.tag = FORTH_FUNCTION, .object.data.function = (VALUE), },
#define FORTH_LIBRARY_END() (forth_library_t) { .name = NULL }, };
