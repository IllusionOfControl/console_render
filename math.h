#pragma once

#include <math.h>
#include "vec2.h"
#include "vec3.h"

float clamp(float value, float min, float max) { return fmax(fmin(value, max), min); }

// returning sign for (+, -) as (1; -1)
float sign(float a) { return (0.f < a) - (a < 0.f); }
// ????
float step(double edge, float x) { return x > edge; }

vector3 norm(vector3 v) { return v / v.lenght(); }
float dot(vector3 const& a, vector3 const& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
vector3 abs(vector3 const& v) { return vector3(fabs(v.x), fabs(v.y), fabs(v.z)); }
vector3 sign(vector3 const& v) { return vector3(sign(v.x), sign(v.y), sign(v.z)); }
vector3 step(vector3 const& edge, vector3 v) { return vector3(step(edge.x, v.x), step(edge.y, v.y), step(edge.z, v.z)); }
vector3 reflect(vector3 rd, vector3 n) { return rd - n * (2 * dot(n, rd)); }

vector3 rotate(vector3 v, vector3 r) {
	float sin_A = sinf(r.x); float cos_A = cosf(r.x);
	float sin_B = sinf(r.y); float cos_B = cosf(r.y);
	float sin_C = sinf(r.z); float cos_C = cosf(r.z);

	#define M(a,b,c) dot(v, vector3(a, b, c))
		v = { M(1.f, 0.f, 0.f), M(0.f, cos_A, sin_A), M(0, -sin_A, cos_A) };
		v = { M(cos_B, 0.f, -sin_B), M(0.f, 1.f, 0.f), M(sin_B, 0.f, cos_B) };

		//calculate matrix mul and fix the minus position
		v = { M(cos_C, -sin_C, 0.f), M(sin_C, cos_C, 0.f), M(0.f, 0.f, 1.f) };
	#undef M

	return v;
}

// Etalon? 
//vector3 rotateZ(vector3 a, double angle)
//{
//    vector3 b = a;
//    b.x = a.x * cos(angle) - a.y * sin(angle);
//    b.y = a.x * sin(angle) + a.y * cos(angle);
//    return b;
//}
//
