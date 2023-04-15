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

typedef struct
{
	float Time;
	float EndTime;
	float *Position;
	float *View;
	int NumPoints;
	int *Knots;
} CameraPath_t;

void CameraInit(Camera_t *Camera);
void CameraUpdate(Camera_t *Camera, float Time);
int CameraLoadPath(char *filename, CameraPath_t *Path);
void CameraInterpolatePath(CameraPath_t *Path, Camera_t *Camera, float TimeStep);
void CameraDeletePath(CameraPath_t *Path);

#endif
