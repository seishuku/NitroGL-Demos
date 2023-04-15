#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include "frustum.h"

void CalculateFrustum(float frustum[6][4])
{
	float projection[16], modelview[16], clip[16], mag;

	glGetFloatv(GL_PROJECTION_MATRIX, projection);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	clip[0]=modelview[0]*projection[0]+modelview[1]*projection[4]+modelview[2]*projection[8]+modelview[3]*projection[12];
	clip[1]=modelview[0]*projection[1]+modelview[1]*projection[5]+modelview[2]*projection[9]+modelview[3]*projection[13];
	clip[2]=modelview[0]*projection[2]+modelview[1]*projection[6]+modelview[2]*projection[10]+modelview[3]*projection[14];
	clip[3]=modelview[0]*projection[3]+modelview[1]*projection[7]+modelview[2]*projection[11]+modelview[3]*projection[15];
	clip[4]=modelview[4]*projection[0]+modelview[5]*projection[4]+modelview[6]*projection[8]+modelview[7]*projection[12];
	clip[5]=modelview[4]*projection[1]+modelview[5]*projection[5]+modelview[6]*projection[9]+modelview[7]*projection[13];
	clip[6]=modelview[4]*projection[2]+modelview[5]*projection[6]+modelview[6]*projection[10]+modelview[7]*projection[14];
	clip[7]=modelview[4]*projection[3]+modelview[5]*projection[7]+modelview[6]*projection[11]+modelview[7]*projection[15];
	clip[8]=modelview[8]*projection[0]+modelview[9]*projection[4]+modelview[10]*projection[8]+modelview[11]*projection[12];
	clip[9]=modelview[8]*projection[1]+modelview[9]*projection[5]+modelview[10]*projection[9]+modelview[11]*projection[13];
	clip[10]=modelview[8]*projection[2]+modelview[9]*projection[6]+modelview[10]*projection[10]+modelview[11]*projection[14];
	clip[11]=modelview[8]*projection[3]+modelview[9]*projection[7]+modelview[10]*projection[11]+modelview[11]*projection[15];
	clip[12]=modelview[12]*projection[0]+modelview[13]*projection[4]+modelview[14]*projection[8]+modelview[15]*projection[12];
	clip[13]=modelview[12]*projection[1]+modelview[13]*projection[5]+modelview[14]*projection[9]+modelview[15]*projection[13];
	clip[14]=modelview[12]*projection[2]+modelview[13]*projection[6]+modelview[14]*projection[10]+modelview[15]*projection[14];
	clip[15]=modelview[12]*projection[3]+modelview[13]*projection[7]+modelview[14]*projection[11]+modelview[15]*projection[15];
	
	frustum[0][0]=clip[3]-clip[0];
	frustum[0][1]=clip[7]-clip[4];
	frustum[0][2]=clip[11]-clip[8];
	frustum[0][3]=clip[15]-clip[12];

	mag=1/(float)sqrt(frustum[0][0]*frustum[0][0]+frustum[0][1]*frustum[0][1]+frustum[0][2]*frustum[0][2]);
	frustum[0][0]*=mag;
	frustum[0][1]*=mag;
	frustum[0][2]*=mag;
	frustum[0][3]*=mag;

	frustum[1][0]=clip[3]+clip[0];
	frustum[1][1]=clip[7]+clip[4];
	frustum[1][2]=clip[11]+clip[8];
	frustum[1][3]=clip[15]+clip[12];

	mag=1/(float)sqrt(frustum[1][0]*frustum[1][0]+frustum[1][1]*frustum[1][1]+frustum[1][2]*frustum[1][2]);
	frustum[1][0]*=mag;
	frustum[1][1]*=mag;
	frustum[1][2]*=mag;
	frustum[1][3]*=mag;

	frustum[2][0]=clip[3]+clip[1];
	frustum[2][1]=clip[7]+clip[5];
	frustum[2][2]=clip[11]+clip[9];
	frustum[2][3]=clip[15]+clip[13];

	mag=1/(float)sqrt(frustum[2][0]*frustum[2][0]+frustum[2][1]*frustum[2][1]+frustum[2][2]*frustum[2][2]);
	frustum[2][0]*=mag;
	frustum[2][1]*=mag;
	frustum[2][2]*=mag;
	frustum[2][3]*=mag;

	frustum[3][0]=clip[3]-clip[1];
	frustum[3][1]=clip[7]-clip[5];
	frustum[3][2]=clip[11]-clip[9];
	frustum[3][3]=clip[15]-clip[13];

	mag=1/(float)sqrt(frustum[3][0]*frustum[3][0]+frustum[3][1]*frustum[3][1]+frustum[3][2]*frustum[3][2]);
	frustum[3][0]*=mag;
	frustum[3][1]*=mag;
	frustum[3][2]*=mag;
	frustum[3][3]*=mag;

	frustum[4][0]=clip[3]-clip[2];
	frustum[4][1]=clip[7]-clip[6];
	frustum[4][2]=clip[11]-clip[10];
	frustum[4][3]=clip[15]-clip[14];

	mag=1/(float)sqrt(frustum[4][0]*frustum[4][0]+frustum[4][1]*frustum[4][1]+frustum[4][2]*frustum[4][2]);
	frustum[4][0]*=mag;
	frustum[4][1]*=mag;
	frustum[4][2]*=mag;
	frustum[4][3]*=mag;

	frustum[5][0]=clip[3]+clip[2];
	frustum[5][1]=clip[7]+clip[6];
	frustum[5][2]=clip[11]+clip[10];
	frustum[5][3]=clip[15]+clip[14];

	mag=1/(float)sqrt(frustum[5][0]*frustum[5][0]+frustum[5][1]*frustum[5][1]+frustum[5][2]*frustum[5][2]);
	frustum[5][0]*=mag;
	frustum[5][1]*=mag;
	frustum[5][2]*=mag;
	frustum[5][3]*=mag;
}

BOOL CubeInFrustum(float frustum[6][4], float x, float y, float z, float size)
{
	int i;

	for(i=0;i<6;i++)
	{
		if(frustum[i][0]*(x-size)+frustum[i][1]*(y-size)+frustum[i][2]*(z-size)+frustum[i][3]>0.0f)
		   continue;

		if(frustum[i][0]*(x+size)+frustum[i][1]*(y-size)+frustum[i][2]*(z-size)+frustum[i][3]>0.0f)
		   continue;

		if(frustum[i][0]*(x-size)+frustum[i][1]*(y+size)+frustum[i][2]*(z-size)+frustum[i][3]>0.0f)
		   continue;

		if(frustum[i][0]*(x+size)+frustum[i][1]*(y+size)+frustum[i][2]*(z-size)+frustum[i][3]>0.0f)
		   continue;

		if(frustum[i][0]*(x-size)+frustum[i][1]*(y-size)+frustum[i][2]*(z+size)+frustum[i][3]>0.0f)
		   continue;

		if(frustum[i][0]*(x+size)+frustum[i][1]*(y-size)+frustum[i][2]*(z+size)+frustum[i][3]>0.0f)
		   continue;

		if(frustum[i][0]*(x-size)+frustum[i][1]*(y+size)+frustum[i][2]*(z+size)+frustum[i][3]>0.0f)
		   continue;

		if(frustum[i][0]*(x+size)+frustum[i][1]*(y+size)+frustum[i][2]*(z+size)+frustum[i][3]>0.0f)
		   continue;

		return FALSE;
	}

	return TRUE;
}

BOOL SphereInFrustum(float frustum[6][4], float x, float y, float z, float radius)
{
	int i;

	for(i=0;i<6;i++)
	{
		if(frustum[i][0]*x+frustum[i][1]*y+frustum[i][2]*z+frustum[i][3]<=-radius)
			return FALSE;
	}

	return TRUE;
}