#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include "quat.h"

void QuatAngle(float out[4], float angle, float x, float y, float z)
{
	float halfangle;
	float mag=1.0f/sqrtf(x*x+y*y+z*z);

	x*=mag;
	y*=mag;
	z*=mag;

	angle*=0.017453292f;

	halfangle=sinf(angle/2.0f);

	out[0]=cosf(angle/2.0f);
	out[1]=x*halfangle;
	out[2]=y*halfangle;
	out[3]=z*halfangle;
}

void QuatEuler(float out[4], float roll, float pitch, float yaw)
{
	float cr, cp, cy, sr, sp, sy, cpcy, spsy;

	cr=cosf(roll/2);
	cp=cosf(pitch/2);
	cy=cosf(yaw/2);
	sr=sinf(roll/2);
	sp=sinf(pitch/2);
	sy=sinf(yaw/2);
	cpcy=cp*cy;
	spsy=sp*sy;

	out[0]=cr*cpcy+sr*spsy;
	out[1]=sr*cpcy-cr*spsy;
	out[2]=cr*sp*cy+sr*cp*sy;
	out[3]=cr*cp*sy-sr*sp*cy;
}

void QuatMultiply(float a[4], float b[4], float out[4])
{
	out[0]=b[0]*a[0]-b[1]*a[1]-b[2]*a[2]-b[3]*a[3];
	out[1]=b[0]*a[1]+b[1]*a[0]+b[2]*a[3]-b[3]*a[2];
	out[2]=b[0]*a[2]-b[1]*a[3]+b[2]*a[0]+b[3]*a[1];
	out[3]=b[0]*a[3]+b[1]*a[2]-b[2]*a[1]+b[3]*a[0];
}

void QuatMatrix(float in[4], float out[16])
{
	float xx, yy, zz;
	float mag=1.0f/sqrtf(in[0]*in[0]+in[1]*in[1]+in[2]*in[2]+in[3]*in[3]);

	in[0]*=mag;
	in[1]*=mag;
	in[2]*=mag;
	in[3]*=mag;

	xx=in[1]*in[1];
	yy=in[2]*in[2];
	zz=in[3]*in[3];

	out[0]=1.0f-2.0f*(yy+zz);
	out[1]=2.0f*(in[1]*in[2]+in[0]*in[3]);
	out[2]=2.0f*(in[1]*in[3]-in[0]*in[2]);
	out[3]=0.0f;
	out[4]=2.0f*(in[1]*in[2]-in[0]*in[3]);
	out[5]=1.0f-2.0f*(xx+zz);
	out[6]=2.0f*(in[2]*in[3]+in[0]*in[1]);
	out[7]=0.0f;
	out[8]=2.0f*(in[1]*in[3]+in[0]*in[2]);
	out[9]=2.0f*(in[2]*in[3]-in[0]*in[1]);
	out[10]=1.0f-2.0f*(xx+yy);
	out[11]=0.0f;
	out[12]=0.0f;
	out[13]=0.0f;
	out[14]=0.0f;
	out[15]=1.0f;
}
