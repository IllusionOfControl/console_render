#pragma once

#define _USE_MATH_DEFINES


#include <math.h>

const float PI = M_PI;

const int SCREEN_WIDTH = 78;
const int SCREEN_HEIGHT = 40;

const int THOR_SEGMENTS = 90;
const int CIRCLE_SEGMENTS = 90; //314
const float THOR_RADIUS = 2.0f;
const float CIRCLE_RADIUS = 1.0f;

const float Z_RANGE = 20.f;
const float RENDER_SCALE = 0.1f;

#define VIEW_DISTANCE ((float)(SCREEN_WIDTH * Z_RANGE / THOR_RADIUS * RENDER_SCALE))

#undef _USE_MATH_DEFINES
