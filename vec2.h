#pragma once

#include <math.h>


struct vector2 {
    float x, y;

    vector2(float value) : x(value), y(value) {}
    vector2(float _x, float _y) : x(_x), y(_y) {}

    vector2 operator+(const vector2 other) { return vector2(x + other.x, y + other.y); }
    vector2 operator-(const vector2 other) { return vector2(x - other.x, y - other.y); }
    vector2 operator*(const vector2 other) { return vector2(x * other.x, y * other.y); }
    vector2 operator/(const vector2 other) { return vector2(x / other.x, y / other.y); }

    vector2 lenght() { return sqrtf(x * x + y * y); };
};
