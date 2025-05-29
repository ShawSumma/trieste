
#pragma once

#define COS_7TURN_OVER_12 0.86602540378

float float_lerp(float a, float b, float n);
float float_min(float x, float y);
float float_max(float x, float y);
float float_abs(float x);

double double_lerp(double a, double b, double n);
double double_min(double x, double y);
double double_max(double x, double y);
double double_abs(double x);
double double_map(double value, double low1, double high1, double low2, double high2);
double double_unmap(double value, double low1, double high1, double low2, double high2);
