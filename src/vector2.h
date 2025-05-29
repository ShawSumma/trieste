
#pragma once

#include <stdint.h>
#include <math.h>
#include "float.h"

typedef struct vector2_t vector2_t;

struct vector2_t {
    double x;
    double y;
};

vector2_t vector2_mid(vector2_t a, vector2_t b);
vector2_t vector2_mid3(vector2_t a, vector2_t b, vector2_t c);

vector2_t vector2_lerp(vector2_t a, vector2_t b, double n);

vector2_t vector2_add(vector2_t a, vector2_t b);

vector2_t vector2_sub(vector2_t a, vector2_t b);

double vector2_len(vector2_t a);
double vector2_dist(vector2_t a, vector2_t b);
