#ifndef _CAMERA_H_
#define _CAMERA_H_

typedef struct
{
	float Position[3];
	float View[3];
	float UpVector[3];
	float Strafe[3];
	float Velocity[3];
} Camera_t;

void CameraInit(Camera_t *Camera);
void CameraUpdate(Camera_t *Camera, float Time);

#endif
