#include <windows.h>
#include "opengl.h"
#include "math.h"

#define CPU_HASFPU 0x00000001
#define CPU_HASMMX 0x00800000
#define CPU_HASSSE 0x02000000
#define CPU_HASSSE2 0x04000000

unsigned int _NextPower2(unsigned int n)
{
	n--;
	n|=n>>1;
	n|=n>>2;
	n|=n>>4;
	n|=n>>8;
	n|=n>>16;
	n++;

	return n;
}

#ifndef __GNUC__
float sinf(float x)
{
	__asm
	{
		fld x
		fsin
		fstp x
	}

	return x;
}

float cosf(float x)
{
	__asm
	{
		fld x
		fcos
		fstp x
	}

	return x;
}

float sqrtf(float x)
{
	__asm
	{
		fld x
		fsqrt
		fstp x
	}

	return x;
}
#endif

void QuatAngle(float angle, float x, float y, float z, float *out)
{
	float s, mag;

	mag=1.0f/sqrtf(x*x+y*y+z*z);
	x*=mag;
	y*=mag;
	z*=mag;

	angle*=0.017453292f;
	angle*=0.5f;

	s=sinf(angle);

	out[0]=cosf(angle);
	out[1]=x*s;
	out[2]=y*s;
	out[3]=z*s;
}

void QuatEuler(float roll, float pitch, float yaw, float *out)
{
	float cr, cp, cy, sr, sp, sy, cpcy, spsy;

	sr=sinf(roll*0.5f);
	cr=cosf(roll*0.5f);

	sp=sinf(pitch*0.5f);
	cp=cosf(pitch*0.5f);

	sy=sinf(yaw*0.5f);
	cy=cosf(yaw*0.5f);

	cpcy=cp*cy;
	spsy=sp*sy;

	out[0]=cr*cpcy+sr*spsy;
	out[1]=sr*cpcy-cr*spsy;
	out[2]=cr*sp*cy+sr*cp*sy;
	out[3]=cr*cp*sy-sr*sp*cy;
}

void QuatMultiply(float a[4], float b[4], float *out)
{
	out[0]=b[0]*a[0]-b[1]*a[1]-b[2]*a[2]-b[3]*a[3];
	out[1]=b[0]*a[1]+b[1]*a[0]+b[2]*a[3]-b[3]*a[2];
	out[2]=b[0]*a[2]-b[1]*a[3]+b[2]*a[0]+b[3]*a[1];
	out[3]=b[0]*a[3]+b[1]*a[2]-b[2]*a[1]+b[3]*a[0];
}

void QuatMatrix(float in[4], float *out)
{
	float m[16];
	float xx, yy, zz, mag;

	mag=1.0f/sqrtf(in[0]*in[0]+in[1]*in[1]+in[2]*in[2]+in[3]*in[3]);
	in[0]*=mag;
	in[1]*=mag;
	in[2]*=mag;
	in[3]*=mag;

	xx=in[1]*in[1];
	yy=in[2]*in[2];
	zz=in[3]*in[3];

	m[0]=1.0f-2.0f*(yy+zz);
	m[1]=2.0f*(in[1]*in[2]+in[0]*in[3]);
	m[2]=2.0f*(in[1]*in[3]-in[0]*in[2]);
	m[3]=0.0f;
	m[4]=2.0f*(in[1]*in[2]-in[0]*in[3]);
	m[5]=1.0f-2.0f*(xx+zz);
	m[6]=2.0f*(in[2]*in[3]+in[0]*in[1]);
	m[7]=0.0f;
	m[8]=2.0f*(in[1]*in[3]+in[0]*in[2]);
	m[9]=2.0f*(in[2]*in[3]-in[0]*in[1]);
	m[10]=1.0f-2.0f*(xx+yy);
	m[11]=0.0f;
	m[12]=0.0f;
	m[13]=0.0f;
	m[14]=0.0f;
	m[15]=1.0f;

	if(out!=NULL)
		memcpy(out, m, sizeof(float)*16);
	else
		glMultMatrixf(m);
}
