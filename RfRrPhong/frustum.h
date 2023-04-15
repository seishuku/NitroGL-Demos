#ifndef __FRUSTUM_H__
#define __FRUSTUM_H__

void CalculateFrustum(float frustum[6][4]);
BOOL CubeInFrustum(float frustum[6][4], float x, float y, float z, float size);
BOOL SphereInFrustum(float frustum[6][4], float x, float y, float z, float radius);

#endif

