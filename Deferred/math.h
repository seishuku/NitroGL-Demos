#ifndef __QUAT_H__
#define __QUAT_H__

#include <math.h>

unsigned int _NextPower2(unsigned int n);

#ifndef __GNUC__
float sinf(float x);
float cosf(float x);
float sqrtf(float x);
#endif

void QuatAngle(float angle, float x, float y, float z, float *out);
void QuatEuler(float roll, float pitch, float yaw, float *out);
void QuatMultiply(float a[4], float b[4], float *out);
void QuatMatrix(float in[4], float *out);

#endif
