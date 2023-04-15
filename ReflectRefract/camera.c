#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include "camera.h"

#define BEHIND 0
#define INTERSECTS 1
#define FRONT 2

void CameraInit(Camera_t *Camera)
{
	Camera->Position[0]=0.0f;
	Camera->Position[1]=0.0f;
	Camera->Position[2]=200.0f;

	Camera->View[0]=0.0f;
	Camera->View[1]=0.0f;
	Camera->View[2]=-1.0f;

	Camera->UpVector[0]=0.0f;
	Camera->UpVector[1]=1.0f;
	Camera->UpVector[2]=0.0f;

	Camera->Velocity[0]=0.0f;
	Camera->Velocity[1]=0.0f;
	Camera->Velocity[2]=0.0f;
}

void Cross(float Vector1[3], float Vector2[3], float *Normal)
{
	Normal[0]=((Vector1[1]*Vector2[2])-(Vector1[2]*Vector2[1]));
	Normal[1]=((Vector1[2]*Vector2[0])-(Vector1[0]*Vector2[2]));
	Normal[2]=((Vector1[0]*Vector2[1])-(Vector1[1]*Vector2[0]));
}

void Normalize(float *Vector)
{
	float mag=(float)sqrt((Vector[0]*Vector[0])+(Vector[1]*Vector[1])+(Vector[2]*Vector[2]));

	if(mag!=0.0f)
	{
		mag=1/mag;
		Vector[0]*=mag;
		Vector[1]*=mag;
		Vector[2]*=mag;
	}
}

void RotateView(Camera_t *Camera, float angle, float x, float y, float z)
{
	float rsin=(float)sin(-angle), rcos=(float)cos(-angle);
	float xx, yy, zz, xy, yz, zx;
	float NewView[3], View[3];

	View[0]=Camera->View[0]-Camera->Position[0];
	View[1]=Camera->View[1]-Camera->Position[1];
	View[2]=Camera->View[2]-Camera->Position[2];

	xx=x*x;
	yy=y*y;
	zz=z*z;
	xy=(x*y*(1.0f-rcos));
	yz=(y*z*(1.0f-rcos));
	zx=(z*x*(1.0f-rcos));

	NewView[0]=(View[0]*(xx+rcos*(1.0f-xx)))+(View[1]*(xy+z*rsin))+(View[2]*(zx-y*rsin));
	NewView[1]=(View[0]*(xy-z*rsin))+(View[1]*(yy+rcos*(1.0f-yy)))+(View[2]*(yz+x*rsin));
	NewView[2]=(View[0]*(zx+y*rsin))+(View[1]*(yz-x*rsin))+(View[2]*(zz+rcos*(1.0f-zz)));

	Camera->View[0]=Camera->Position[0]+NewView[0];
	Camera->View[1]=Camera->Position[1]+NewView[1];
	Camera->View[2]=Camera->Position[2]+NewView[2];
}

void SetViewByMouse(Camera_t *Camera)
{
	POINT mousePos;
	int middleX=GetSystemMetrics(SM_CXSCREEN)>>1;
	int middleY=GetSystemMetrics(SM_CYSCREEN)>>1;
	float angleY=0.0f, angleZ=0.0f;
	static float currentRotX=0.0f;

	GetCursorPos(&mousePos);

	if((mousePos.x==middleX)&&(mousePos.y==middleY))
		return;

	SetCursorPos(middleX, middleY);

	angleY=(float)((middleX-mousePos.x))/500.0f;
	angleZ=(float)((middleY-mousePos.y))/500.0f;
	currentRotX-=angleZ;

	if(currentRotX>1.57f)
		currentRotX=1.57f;
	else
	{
		if(currentRotX<-1.57f)
			currentRotX=-1.57f;
		else
		{
			float Axis[3], temp[3];

			temp[0]=Camera->View[0]-Camera->Position[0];
			temp[1]=Camera->View[1]-Camera->Position[1];
			temp[2]=Camera->View[2]-Camera->Position[2];

			Cross(temp, Camera->UpVector, Axis);
			Normalize(Axis);
			RotateView(Camera, angleZ, Axis[0], Axis[1], Axis[2]);
		}
	}

	RotateView(Camera, angleY, 0.0f, 1.0f, 0.0f);
}

void UDCamera(Camera_t *Camera, float speed)
{
	Camera->Position[1]+=speed;
	Camera->View[1]+=speed;
}

void LRCamera(Camera_t *Camera, float speed)
{
	Camera->Position[0]+=Camera->Strafe[0]*speed;
	Camera->Position[2]+=Camera->Strafe[2]*speed;
	Camera->View[0]+=Camera->Strafe[0]*speed;
	Camera->View[2]+=Camera->Strafe[2]*speed;
}

void FBCamera(Camera_t *Camera, float speed)
{
	float Vector[3];

	Vector[0]=Camera->View[0]-Camera->Position[0];
	Vector[1]=Camera->View[1]-Camera->Position[1];
	Vector[2]=Camera->View[2]-Camera->Position[2];

	Normalize(Vector);

	Camera->Position[0]+=Vector[0]*speed;
	Camera->Position[1]+=Vector[1]*speed;
	Camera->Position[2]+=Vector[2]*speed;
	Camera->View[0]+=Vector[0]*speed;
	Camera->View[1]+=Vector[1]*speed;
	Camera->View[2]+=Vector[2]*speed;
}

void CameraUpdate(Camera_t *Camera, float Time)
{
	float speed=200.0f*Time;
	float temp[3];

	temp[0]=Camera->View[0]-Camera->Position[0];
	temp[1]=Camera->View[1]-Camera->Position[1];
	temp[2]=Camera->View[2]-Camera->Position[2];

	Cross(temp, Camera->UpVector, Camera->Strafe);
	Normalize(Camera->Strafe);

	SetViewByMouse(Camera);

	if(GetKeyState('D')&0x80)
		Camera->Velocity[0]+=0.25f;

	if(GetKeyState('A')&0x80)
		Camera->Velocity[0]-=0.25f;

	if(GetKeyState('V')&0x80)
		Camera->Velocity[1]+=0.25f;

	if(GetKeyState('C')&0x80)
		Camera->Velocity[1]-=0.25f;

	if(GetKeyState('W')&0x80)
		Camera->Velocity[2]+=0.25f;

	if(GetKeyState('S')&0x80)
		Camera->Velocity[2]-=0.25f;

	Camera->Velocity[0]*=0.91f;
	Camera->Velocity[1]*=0.91f;
	Camera->Velocity[2]*=0.91f;

	LRCamera(Camera, speed*Camera->Velocity[0]);
	UDCamera(Camera, speed*Camera->Velocity[1]);
	FBCamera(Camera, speed*Camera->Velocity[2]);

	gluLookAt(Camera->Position[0], Camera->Position[1], Camera->Position[2], Camera->View[0], Camera->View[1], Camera->View[2], Camera->UpVector[0], Camera->UpVector[1], Camera->UpVector[2]);
}
