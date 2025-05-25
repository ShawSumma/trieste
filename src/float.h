
#pragma once

static inline float float_min(float x, float y) {
    if (x < y) {
        return x;
    } else {
        return y;
    }
}

static inline float float_max(float x, float y) {
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

static inline float float_abs(float x) {
    if (x < 0) {
        return -x;
    } else {
        return x;
    }
}

static inline float float_map(float value, float low1, float high1, float low2, float high2) {
    return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}

static inline float float_unmap(float value, float low1, float high1, float low2, float high2) {
    return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}

static inline double double_min(double x, double y) {
    if (x < y) {
        return x;
    } else {
        return y;
    }
}

static inline double double_max(double x, double y) {
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

static inline double double_abs(double x) {
    if (x < 0) {
        return -x;
    } else {
        return x;
    }
}

static inline double double_map(double value, double low1, double high1, double low2, double high2) {
    return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}

static inline double double_unmap(double value, double low1, double high1, double low2, double high2) {
    return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}
