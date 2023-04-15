#ifndef _QUAT_H_
#define _QUAT_H_

void QuatAngle(float out[4], float angle, float x, float y, float z);
void QuatEuler(float out[4], float roll, float pitch, float yaw);
void QuatMultiply(float a[4], float b[4], float out[4]);
void QuatMatrix(float in[4], float out[16]);

#endif