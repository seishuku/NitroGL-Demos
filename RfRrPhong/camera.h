#ifndef _CAMERA_H_
#define _CAMERA_H_

typedef struct
{
	float Position[3];
	float View[3];
	float UpVector[3];
	float Strafe[3];
	float Velocity[3];
	float Radius;
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

void LookAt(float eye[3], float center[3], float up[3]);

void CameraInit(Camera_t *Camera);
void CameraUpdate(Camera_t *Camera, float Time);
void CameraCheckCollision(Camera_t *Camera, float *Vertex, unsigned short *Face, int NumFace);
int CameraLoadPath(char *filename, CameraPath_t *Path);
void CameraInterpolatePath(CameraPath_t *Path, Camera_t *Camera, float TimeStep);
void CameraDeletePath(CameraPath_t *Path);

#endif
