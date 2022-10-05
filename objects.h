#pragma once

#include "math.h"


vector2 sphere(vector3 ro, vector3 rd, float r) {
    float b = dot(ro, rd);
    float c = dot(ro, ro) - r * r;
    float h = b * b - c;
    if (h < 0.0) return vector2(-1.0f);
    h = sqrt(h);
    return vector2(-b - h, -b + h);
}

vector2 box(vector3 ro, vector3 rd, vector3 boxSize, vector3& outNormal) {
    vector3 m = vector3(1.0) / rd;
    vector3 n = m * ro;
    vector3 k = abs(m) * boxSize;
    vector3 t1 = -n - k;
    vector3 t2 = -n + k;
    float tN = fmax(fmax(t1.x, t1.y), t1.z);
    float tF = fmin(fmin(t2.x, t2.y), t2.z);
    if (tN > tF || tF < 0.0) return vector2(-1.0);
    vector3 yzx = vector3(t1.y, t1.z, t1.x);
    vector3 zxy = vector3(t1.z, t1.x, t1.y);
    outNormal = -sign(rd) * step(yzx, t1) * step(zxy, t1);
    return vector2(tN, tF);
}

float plane(vector3 ro, vector3 rd, vector3 p, float w) {
    return -(dot(ro, p) + w) / dot(rd, p);
}
