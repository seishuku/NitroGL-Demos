#ifndef __QUAT_H__
#define __QUAT_H__

#include <math.h>

//#ifndef __GNUC__
//float sinf(float x);
//float cosf(float x);
//float tanf(float x);
//float sqrtf(float x);
//#endif

float fsinf(float x);
float fcosf(float x);
float ftanf(float x);

float fact(int n);

unsigned int NextPower2(unsigned int value);
int ComputeLog(unsigned int value);

void Normalize(float *v);
void Cross(float v0[3], float v1[3], float *n);

void QuatAngle(float angle, float x, float y, float z, float *out);
void QuatEuler(float roll, float pitch, float yaw, float *out);
void QuatMultiply(float a[4], float b[4], float *out);
void QuatMatrix(float in[4], float *out);

void MatrixMult(float a[16], float b[16], float *out);
void MatrixInverse(float in[16], float *out);
void MatrixTranspose(float in[16], float *out);
void Matrix4x4MultVec4(float in[4], float m[16], float *out);
void Matrix4x4MultVec3(float in[3], float m[16], float *out);
void Matrix3x3MultVec3(float in[3], float m[16], float *out);

void InfPerspectiveDxDy(float fovy, float aspect, float zNear, float Dx, float Dy);
void InfPerspective(float fovy, float aspect, float zNear);
void PerspectiveDxDy(float fovy, float aspect, float zNear, float zFar, float Dx, float Dy);
void Perspective(float fovy, float aspect, float zNear, float zFar);

#endif
