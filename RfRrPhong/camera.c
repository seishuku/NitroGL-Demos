#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <malloc.h>
#include "camera.h"

void Cross(float Vector1[3], float Vector2[3], float *Normal)
{
	Normal[0]=(Vector1[1]*Vector2[2])-(Vector1[2]*Vector2[1]);
	Normal[1]=(Vector1[2]*Vector2[0])-(Vector1[0]*Vector2[2]);
	Normal[2]=(Vector1[0]*Vector2[1])-(Vector1[1]*Vector2[0]);
}

float Dot(float Vector1[3], float Vector2[3])
{
	return Vector1[0]*Vector2[0]+Vector1[1]*Vector2[1]+Vector1[2]*Vector2[2];
}

void Normalize(float *Vector)
{
	float mag=(float)sqrt(Dot(Vector, Vector));

	if(mag!=0.0f)
	{
		mag=1/mag;
		Vector[0]*=mag;
		Vector[1]*=mag;
		Vector[2]*=mag;
	}
	else
		Vector[0]=Vector[1]=Vector[2]=0.0f;
}

float Distance(float Vector1[3], float Vector2[3])
{
	float Vector[3]={ Vector2[0]-Vector1[0], Vector2[1]-Vector1[1], Vector2[2]-Vector1[2] };

	return (float)sqrt(Dot(Vector, Vector));
}

int ClassifySphere(float Center[3], float Normal[3], float Point[3], float radius, float *distance)
{
	*distance=Dot(Normal, Center)-Dot(Normal, Point);

	if(fabs(*distance)<radius)
		return 1;
	else
	{
		if(*distance>=radius)
			return 2;
	}

	return 0;
}

float AngleBetweenVectors(float Vector1[3], float Vector2[3])
{
	return (float)acos(Dot(Vector1, Vector2)/(sqrt(Dot(Vector1, Vector1))*sqrt(Dot(Vector2, Vector2))));
}

int InsidePolygon(float Intersection[3], float Tri[3][3])
{
	float Angle=0.0f;
	float A[3], B[3], C[3];

	A[0]=Tri[0][0]-Intersection[0];
	A[1]=Tri[0][1]-Intersection[1];
	A[2]=Tri[0][2]-Intersection[2];

	B[0]=Tri[1][0]-Intersection[0];
	B[1]=Tri[1][1]-Intersection[1];
	B[2]=Tri[1][2]-Intersection[2];

	C[0]=Tri[2][0]-Intersection[0];
	C[1]=Tri[2][1]-Intersection[1];
	C[2]=Tri[2][2]-Intersection[2];

	Angle+=AngleBetweenVectors(A, B);
	Angle+=AngleBetweenVectors(B, C);
	Angle+=AngleBetweenVectors(C, A);

	if(Angle>=6.220353348f)
		return 1;

	return 0;
}

void ClosestPointOnLine(float A[3], float B[3], float Point[3], float *ClosestPoint)
{
	float Vector1[3]={ Point[0]-A[0], Point[1]-A[1], Point[2]-A[2] };
	float Vector2[3]={ B[0]-A[0], B[1]-A[1], B[2]-A[2] };
	float d=Distance(A, B), t;

	Normalize(Vector2);

	t=Dot(Vector1, Vector2);

	if(t<=0.0f)
	{
		ClosestPoint[0]=A[0];
		ClosestPoint[1]=A[1];
		ClosestPoint[2]=A[2];

		return;
	}

	if(t>=d)
	{
		ClosestPoint[0]=B[0];
		ClosestPoint[1]=B[1];
		ClosestPoint[2]=B[2];

		return;
	}

	ClosestPoint[0]=A[0]+(Vector2[0]*t);
	ClosestPoint[1]=A[1]+(Vector2[1]*t);
	ClosestPoint[2]=A[2]+(Vector2[2]*t);
}

int EdgeSphereCollision(float *Center, float Tri[3][3], float radius)
{
	int i;
	float Point[3], distance;

	for(i=0;i<3;i++)
	{
		ClosestPointOnLine(Tri[i], Tri[(i+1)%3], Center, Point);

		distance=Distance(Point, Center);

		if(distance<radius)
			return 1;
	}

	return 0;
}

void GetCollisionOffset(float Normal[3], float radius, float distance, float *Offset)
{
	if(distance>0.0f)
	{
		float distanceOver=radius-distance;

		Offset[0]=Normal[0]*distanceOver;
		Offset[1]=Normal[1]*distanceOver;
		Offset[2]=Normal[2]*distanceOver;

		return;
	}
	else
	{
		float distanceOver=radius+distance;

		Offset[0]=Normal[0]*-distanceOver;
		Offset[1]=Normal[1]*-distanceOver;
		Offset[2]=Normal[2]*-distanceOver;

		return;
	}

	Offset[0]=0.0f;
	Offset[1]=0.0f;
	Offset[2]=0.0f;
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

void fsincos(float x, float *s, float *c)
{
	float cos_x, sin_x;

	__asm
	{
		fld dword ptr [x];
		fsincos;
		fstp dword ptr [cos_x];
		fstp dword ptr [sin_x];
	}

	*s=sin_x;
	*c=cos_x;
}

void RotateView(Camera_t *Camera, float angle, float x, float y, float z)
{
	float NewView[3], View[3];
	float cosTheta, sinTheta;

	fsincos(angle, &sinTheta, &cosTheta);

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

	if(currentRotX>1.570795f)
		currentRotX=1.570795f;
	else
	{
		if(currentRotX<-1.570795f)
			currentRotX=-1.570795f;
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

void LookAt(float eye[3], float center[3], float up[3])
{
	float forward[3], side[3], temp[3];
	float m[16]={ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	forward[0]=center[0]-eye[0];
	forward[1]=center[1]-eye[1];
	forward[2]=center[2]-eye[2];

	Normalize(forward);

	Cross(forward, up, side);
	Normalize(side);

	Cross(side, forward, temp);

	m[0]=side[0];
	m[4]=side[1];
	m[8]=side[2];

	m[1]=temp[0];
	m[5]=temp[1];
	m[9]=temp[2];

	m[2]=-forward[0];
	m[6]=-forward[1];
	m[10]=-forward[2];

	glMultMatrixf(m);
	glTranslatef(-eye[0], -eye[1], -eye[2]);
}

void CameraInit(Camera_t *Camera)
{
	Camera->Position[0]=0.0f;
	Camera->Position[1]=0.0f;
	Camera->Position[2]=50.0f;

	Camera->View[0]=0.0f;
	Camera->View[1]=0.0f;
	Camera->View[2]=0.0f;

	Camera->UpVector[0]=0.0f;
	Camera->UpVector[1]=1.0f;
	Camera->UpVector[2]=0.0f;

	Camera->Velocity[0]=0.0f;
	Camera->Velocity[1]=0.0f;
	Camera->Velocity[2]=0.0f;

	Camera->Radius=10.0f;
}

void CameraUpdate(Camera_t *Camera, float Time)
{
	float Speed=150.0f*Time;
	float temp[3];

	temp[0]=Camera->View[0]-Camera->Position[0];
	temp[1]=Camera->View[1]-Camera->Position[1];
	temp[2]=Camera->View[2]-Camera->Position[2];

	Cross(temp, Camera->UpVector, Camera->Strafe);
	Normalize(Camera->Strafe);

	SetViewByMouse(Camera);

	if(GetKeyState('A')&0x80)
		LRCamera(Camera, -Speed);

	if(GetKeyState('D')&0x80)
		LRCamera(Camera, Speed);

	if(GetKeyState('V')&0x80)
		UDCamera(Camera, Speed);

	if(GetKeyState('C')&0x80)
		UDCamera(Camera, -Speed);

	if(GetKeyState('W')&0x80)
		FBCamera(Camera, Speed);

	if(GetKeyState('S')&0x80)
		FBCamera(Camera, -Speed);
}

void CameraCheckCollision(Camera_t *Camera, float *Vertex, unsigned short *Face, int NumFace)
{
	unsigned short i;
	int classification;
	float distance=0.0f;
	float v0[3], v1[3], n[3];

	for(i=0;i<NumFace;i++)
	{
		float Tri[3][3]=
		{
			{ Vertex[3*Face[3*i+0]], Vertex[3*Face[3*i+0]+1], Vertex[3*Face[3*i+0]+2] },
			{ Vertex[3*Face[3*i+1]], Vertex[3*Face[3*i+1]+1], Vertex[3*Face[3*i+1]+2] },
			{ Vertex[3*Face[3*i+2]], Vertex[3*Face[3*i+2]+1], Vertex[3*Face[3*i+2]+2] }
		};

		v0[0]=Tri[1][0]-Tri[0][0];
		v0[1]=Tri[1][1]-Tri[0][1];
		v0[2]=Tri[1][2]-Tri[0][2];

		v1[0]=Tri[2][0]-Tri[0][0];
		v1[1]=Tri[2][1]-Tri[0][1];
		v1[2]=Tri[2][2]-Tri[0][2];

		n[0]=(v0[1]*v1[2]-v0[2]*v1[1]);
		n[1]=(v0[2]*v1[0]-v0[0]*v1[2]);
		n[2]=(v0[0]*v1[1]-v0[1]*v1[0]);

		Normalize(n);

		classification=ClassifySphere(Camera->Position, n, Tri[0], Camera->Radius, &distance);

		if(classification==1)
		{
			float Offset[3]={ n[0]*distance, n[1]*distance, n[2]*distance };
			float Intersection[3]={ Camera->Position[0]-Offset[0], Camera->Position[1]-Offset[1], Camera->Position[2]-Offset[2] };

			if(InsidePolygon(Intersection, Tri)||EdgeSphereCollision(Camera->Position, Tri, Camera->Radius*0.5f))
			{
				GetCollisionOffset(n, Camera->Radius, distance, Offset);

				Camera->Position[0]+=Offset[0];
				Camera->Position[1]+=Offset[1];
				Camera->Position[2]+=Offset[2];

				Camera->View[0]+=Offset[0];
				Camera->View[1]+=Offset[1];
				Camera->View[2]+=Offset[2];
			}
		}
	}
}

int CameraLoadPath(char *filename, CameraPath_t *Path)
{
	FILE *stream;
	int i;

	Path->NumPoints=0;

	if((stream=fopen(filename, "rt"))==NULL)
		return 0;

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

	return 1;
}

void CameraInterpolatePath(CameraPath_t *Path, Camera_t *Camera, float TimeStep)
{
	Path->Time+=TimeStep;

	if(Path->Time>Path->EndTime)
		Path->Time=0.0f;

	CalculatePoint(Path->Knots, Path->NumPoints-1, 3, Path->Time, Path->Position, Camera->Position);
	CalculatePoint(Path->Knots, Path->NumPoints-1, 3, Path->Time, Path->View, Camera->View);
}

void CameraDeletePath(CameraPath_t *Path)
{
	if(Path->Position)
		free(Path->Position);

	if(Path->View)
		free(Path->View);
}