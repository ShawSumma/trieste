
#include "float.h"

float float_lerp(float a, float b, float n) {
    return a*(1 - n) + b*n;
}

float float_min(float x, float y) {
    if (x < y) {
        return x;
    } else {
        return y;
    }
}

float float_max(float x, float y) {
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

float float_abs(float x) {
    if (x < 0) {
        return -x;
    } else {
        return x;
    }
}

double double_lerp(double a, double b, double n) {
    return a * (1 - n) + b * n;
}

double double_min(double x, double y) {
    if (x < y) {
        return x;
    } else {
        return y;
    }
}

double double_max(double x, double y) {
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

double double_abs(double x) {
    if (x < 0) {
        return -x;
    } else {
        return x;
    }
}

double double_map(double value, double low1, double high1, double low2, double high2) {
    return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}

double double_unmap(double value, double low1, double high1, double low2, double high2) {
    return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}
