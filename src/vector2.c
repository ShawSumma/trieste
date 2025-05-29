
#include "vector2.h"

vector2_t vector2_mid(vector2_t a, vector2_t b) {
    return (vector2_t) {
        .x = (a.x + b.x) * 0.5,
        .y = (a.y + b.y) * 0.5,
    };
}

vector2_t vector2_mid3(vector2_t a, vector2_t b, vector2_t c) {
    return (vector2_t) {
        .x = (a.x + b.x + c.x) * 0.333333333333,
        .y = (a.y + b.y + c.y) * 0.333333333333,
    };
}

vector2_t vector2_lerp(vector2_t a, vector2_t b, double n) {
    return (vector2_t) {
        .x = double_lerp(a.x, b.x, n),
        .y = double_lerp(a.y, b.y, n),
    };
}

vector2_t vector2_add(vector2_t a, vector2_t b) {
    return (vector2_t) {
        .x = a.x + b.x,
        .y = a.y + b.y,
    };
}

vector2_t vector2_sub(vector2_t a, vector2_t b) {
    return (vector2_t) {
        .x = a.x + b.x,
        .y = a.y + b.y,
    };
}

double vector2_len(vector2_t a) {
    return sqrt(a.x*a.x + a.y*a.y);
}

double vector2_dist(vector2_t a, vector2_t b) {
    return vector2_len(vector2_sub(a, b));
}
