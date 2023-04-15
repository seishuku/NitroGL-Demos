#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include "camera.h"

void CameraInit(Camera_t *Camera)
{
	int middleX=GetSystemMetrics(SM_CXSCREEN)>>1;
	int middleY=GetSystemMetrics(SM_CYSCREEN)>>1;
	SetCursorPos(middleX, middleY);

	Camera->Position[0]=0.0f;
	Camera->Position[1]=0.0f;
	Camera->Position[2]=50.0f;

	Camera->View[0]=0.0f;
	Camera->View[1]=0.0f;
	Camera->View[2]=1.0f;

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
	float NewView[3], View[3];
	float cosTheta=(float)cos(angle);
	float sinTheta=(float)sin(angle);

	View[0]=Camera->View[0]-Camera->Position[0];
	View[1]=Camera->View[1]-Camera->Position[1];
	View[2]=Camera->View[2]-Camera->Position[2];

	NewView[0]=(cosTheta+(1-cosTheta)*x*x)*View[0]+((1-cosTheta)*x*y-z*sinTheta)*View[1]+((1-cosTheta)*x*z+y*sinTheta)*View[2];
	NewView[1]=((1-cosTheta)*x*y+z*sinTheta)*View[0]+(cosTheta+(1-cosTheta)*y*y)*View[1]+((1-cosTheta)*y*z-x*sinTheta)*View[2];
	NewView[2]=((1-cosTheta)*x*z-y*sinTheta)*View[0]+((1-cosTheta)*y*z+x*sinTheta)*View[1]+(cosTheta+(1-cosTheta)*z*z)*View[2];

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

	if(GetKeyState('A')&0x80)
		Camera->Velocity[0]-=0.25f;

	if(GetKeyState('D')&0x80)
		Camera->Velocity[0]+=0.25f;

	if(GetKeyState('V')&0x80)
		Camera->Velocity[1]+=0.25f;

	if(GetKeyState('C')&0x80)
		Camera->Velocity[1]-=0.25f;

	if(GetKeyState('W')&0x80)
		Camera->Velocity[2]+=0.25f;

	if(GetKeyState('S')&0x80)
		Camera->Velocity[2]-=0.25f;

	Camera->Velocity[0]*=0.8f;
	Camera->Velocity[1]*=0.8f;
	Camera->Velocity[2]*=0.8f;

	LRCamera(Camera, speed*Camera->Velocity[0]);
	UDCamera(Camera, speed*Camera->Velocity[1]);
	FBCamera(Camera, speed*Camera->Velocity[2]);

	gluLookAt(Camera->Position[0], Camera->Position[1], Camera->Position[2], Camera->View[0], Camera->View[1], Camera->View[2], Camera->UpVector[0], Camera->UpVector[1], Camera->UpVector[2]);
}

float Blend(int k, int t, int *knots, float v)
{
	float b;

	if(t==1)
	{
		if((knots[k]<=v)&&(v<knots[k+1]))
			b=1.0f;
		else
			b=0.0f;
	}
	else
	{
		if((knots[k+t-1]==knots[k])&&(knots[k+t]==knots[k+1]))
			b=0.0f;
		else
		{
			if(knots[k+t-1]==knots[k])
				b=(knots[k+t]-v)/(knots[k+t]-knots[k+1])*Blend(k+1, t-1, knots, v);
			else
			{
				if(knots[k+t]==knots[k+1])
					b=(v-knots[k])/(knots[k+t-1]-knots[k])*Blend(k, t-1, knots, v);
				else
					b=(v-knots[k])/(knots[k+t-1]-knots[k])*Blend(k, t-1, knots, v)+(knots[k+t]-v)/(knots[k+t]-knots[k+1])*Blend(k+1, t-1, knots, v);
			}
		}
	}

	return b;
}

void CalculateKnots(int *knots, int n, int t)
{
	int i;
	
	for(i=0;i<=n+t;i++)
	{
		if(i<t)
			knots[i]=0;
		else
		{
			if((t<=i)&&(i<=n))
				knots[i]=i-t+1;
			else
			{
				if(i>n)
					knots[i]=n-t+2;
			}
		}
	}
}

void CalculatePoint(int *knots, int n, int t, float v, float *control, float *output)
{
	int k;
	float b;

	output[0]=output[1]=output[2]=0.0f;

	for(k=0;k<=n;k++)
	{
		b=Blend(k, t, knots, v);

		output[0]+=control[3*k]*b;
		output[1]+=control[3*k+1]*b;
		output[2]+=control[3*k+2]*b;
	}
}

int CameraLoadPath(char *filename, CameraPath_t *Path)
{
	FILE *stream;
	int i;

	Path->NumPoints=0;

	if((stream=fopen(filename, "rt"))==NULL)
		return FALSE;

	fscanf(stream, "%d", &Path->NumPoints);

	Path->Position=(float *)malloc(sizeof(float)*Path->NumPoints*3);
	Path->View=(float *)malloc(sizeof(float)*Path->NumPoints*3);

	for(i=0;i<Path->NumPoints;i++)
		fscanf(stream, "%f %f %f %f %f %f", &Path->Position[3*i], &Path->Position[3*i+1], &Path->Position[3*i+2], &Path->View[3*i], &Path->View[3*i+1], &Path->View[3*i+2]);

	fclose(stream);

	Path->Time=0.0f;
	Path->EndTime=(float)(Path->NumPoints-2);

	Path->Knots=(int *)malloc(sizeof(int)*Path->NumPoints*3);
	CalculateKnots(Path->Knots, Path->NumPoints-1, 3);

	return TRUE;
}

void CameraInterpolatePath(CameraPath_t *Path, Camera_t *Camera, float TimeStep)
{
	Path->Time+=TimeStep;

	if(Path->Time>Path->EndTime)
		Path->Time=0.0f;

	CalculatePoint(Path->Knots, Path->NumPoints-1, 3, Path->Time, Path->Position, Camera->Position);
	CalculatePoint(Path->Knots, Path->NumPoints-1, 3, Path->Time, Path->View, Camera->View);

	gluLookAt(Camera->Position[0], Camera->Position[1], Camera->Position[2], Camera->View[0], Camera->View[1], Camera->View[2], 0.0f, 1.0f, 0.0f);
}

void CameraDeletePath(CameraPath_t *Path)
{
	if(Path->Position)
		free(Path->Position);

	if(Path->View)
		free(Path->View);
}
