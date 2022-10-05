#pragma once

#include <math.h>
#include "vec2.h"


struct vector3 {
	float x, y, z;

	vector3(float _value) : x(_value), y(_value), z(_value) {};
	vector3(float _x, vector2 const& v) : x(_x), y(v.x), z(v.y) {};
	vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};

	vector3 operator+(vector3 const& other) { return vector3(x + other.x, y + other.y, z + other.z); }
	vector3 operator-(vector3 const& other) { return vector3(x - other.x, y - other.y, z - other.z); }
	vector3 operator*(vector3 const& other) { return vector3(x * other.x, y * other.y, z * other.z); }
	vector3 operator/(vector3 const& other) { return vector3(x / other.x, y / other.y, z / other.z); }
	vector3 operator-() { return vector3(-x, -y, -z); }

	vector3 lenght() { return sqrtf(x * x + y * y + z * z); }
};
