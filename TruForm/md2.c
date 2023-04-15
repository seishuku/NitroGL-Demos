#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "glati.h"
#include "md2.h"

extern PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;

BOOL MD2_LoadModel(char *filename, MD2_Model_t *MD2)
{
	FILE *stream;
	int i, j, k;
	unsigned char buffer[MD2_MAX_FRAMESIZE];

	if((stream=fopen(filename, "rb"))==NULL)
		return FALSE;

	fread(&MD2->header, sizeof(unsigned char), sizeof(MD2_Header_t), stream);

	if(MD2->header.magic!=MD2_MAGIC||MD2->header.version!=MD2_VERSION)
		return FALSE;

	fseek(stream, MD2->header.offsetSkins, SEEK_SET);
	MD2->skins=malloc(sizeof(MD2_Skin_t)*MD2->header.numSkins);
	for(i=0;i<MD2->header.numSkins;i++)
		fread(&MD2->skins[i], sizeof(MD2_Skin_t), 1, stream);

	fseek(stream, MD2->header.offsetTexCoords, SEEK_SET);
	MD2->texcoords=malloc(sizeof(MD2_TextureCoordinate_t)*MD2->header.numTexCoords);
	for(i=0;i<MD2->header.numTexCoords;i++)
		fread(&MD2->texcoords[i], sizeof(MD2_TextureCoordinate_t), 1, stream);

	fseek(stream, MD2->header.offsetTriangles, SEEK_SET);
	MD2->triangles=malloc(sizeof(MD2_Triangle_t)*MD2->header.numTriangles);
	for(i=0;i<MD2->header.numTriangles;i++)
		fread(&MD2->triangles[i], sizeof(MD2_Triangle_t), 1, stream);

	fseek(stream, MD2->header.offsetFrames, SEEK_SET);
	MD2->frames=malloc(sizeof(MD2_Frame_t)*MD2->header.numFrames);
	for(i=0;i<MD2->header.numFrames;i++)
	{
		MD2_Alias_Frame_t *frame=(MD2_Alias_Frame_t *)buffer;

		MD2->frames[i].vertices=malloc(sizeof(MD2_TriangleVertex_t)*MD2->header.numVertices);

		fread(frame, 1, MD2->header.frameSize, stream);

		strcpy(MD2->frames[i].name, frame->name);

		for(j=0;j<MD2->header.numVertices;j++)
		{
			MD2->frames[i].vertices[j].vertex[0]=(float)(frame->alias_vertices[j].vertex[0]*frame->scale[0]+frame->translate[0]);
			MD2->frames[i].vertices[j].vertex[1]=(float)(frame->alias_vertices[j].vertex[1]*frame->scale[1]+frame->translate[1]);
			MD2->frames[i].vertices[j].vertex[2]=(float)(frame->alias_vertices[j].vertex[2]*frame->scale[2]+frame->translate[2]);
		}
	}

	fseek(stream, MD2->header.offsetGlCommands, SEEK_SET);
	MD2->glcommandbuffer=malloc(sizeof(int)*MD2->header.numGlCommands);
	fread(MD2->glcommandbuffer, sizeof(int), MD2->header.numGlCommands, stream);

	fclose(stream);

	for(i=0;i<MD2->header.numFrames;i++)
	{
		for(j=0;j<MD2->header.numVertices;j++)
		{
			MD2->frames[i].vertices[j].normal[0]=0.0f;
			MD2->frames[i].vertices[j].normal[1]=0.0f;
			MD2->frames[i].vertices[j].normal[2]=0.0f;
		}

		for(j=0;j<MD2->header.numTriangles;j++)
		{
			float facetnormal[3];
			float v1[3], v2[3];

			v1[0]=MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[1]].vertex[0]-MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[0]].vertex[0];
			v1[1]=MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[1]].vertex[1]-MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[0]].vertex[1];
			v1[2]=MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[1]].vertex[2]-MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[0]].vertex[2];
			v2[0]=MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[2]].vertex[0]-MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[0]].vertex[0];
			v2[1]=MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[2]].vertex[1]-MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[0]].vertex[1];
			v2[2]=MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[2]].vertex[2]-MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[0]].vertex[2];

			facetnormal[0]=(v1[1]*v2[2]-v1[2]*v2[1]);
			facetnormal[1]=(v1[2]*v2[0]-v1[0]*v2[2]);
			facetnormal[2]=(v1[0]*v2[1]-v1[1]*v2[0]);

			for(k=0;k<3;k++)
			{
				MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[k]].normal[0]-=facetnormal[0];
				MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[k]].normal[2]-=facetnormal[2];
				MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[k]].normal[1]-=facetnormal[1];
			}
		}

		for(j=0;j<MD2->header.numVertices;j++)
		{
			float *n=MD2->frames[i].vertices[j].normal;
			float l=(float)sqrt((n[0]*n[0])+(n[1]*n[1])+(n[2]*n[2]));

			if(l!=0.0f)
			{
				n[0]/=l;
				n[1]/=l;
				n[2]/=l;
			}
			else
			{
				n[0]=0.0f;
				n[0]=0.0f;
				n[0]=1.0f;
			}
		}
	}

	return TRUE;
}

void MD2_DrawFrame(MD2_Model_t *MD2, int Frame)
{
	int i=0, val=MD2->glcommandbuffer[i++];

	while(val!=0)
	{
		int count;

		if(val>0)
		{
			glBegin(GL_TRIANGLE_STRIP);

			count=val;
		}
		else
		{
			glBegin(GL_TRIANGLE_FAN);

			count=-val;
		}

		while(count--)
		{
			float s=*(float *)&MD2->glcommandbuffer[i++];
			float t=*(float *)&MD2->glcommandbuffer[i++];
			int index=MD2->glcommandbuffer[i++];

//			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, s, -t);
//			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, s, -t);
//			glMultiTexCoord2fARB(GL_TEXTURE2_ARB, s, -t);
			glTexCoord2f(s, -t);
			glNormal3f(MD2->frames[Frame].vertices[index].normal[0], MD2->frames[Frame].vertices[index].normal[2], -MD2->frames[Frame].vertices[index].normal[1]);
			glVertex3f(MD2->frames[Frame].vertices[index].vertex[0], MD2->frames[Frame].vertices[index].vertex[1], MD2->frames[Frame].vertices[index].vertex[2]);
		}

		glEnd();

		val=MD2->glcommandbuffer[i++];
	}
}

void MD2_DrawFramei(MD2_Model_t *MD2, int Frame1, int Frame2, float Pol)
{
	int i=0, val=MD2->glcommandbuffer[i++];

	float x1, y1, z1, x2, y2, z2, nx1, ny1, nz1, nx2, ny2, nz2;

	while(val!=0)
	{
		int count;

		if(val>0)
		{
			glBegin(GL_TRIANGLE_STRIP);

			count=val;
		}
		else
		{
			glBegin(GL_TRIANGLE_FAN);

			count=-val;
		}

		while(count--)
		{
			float s=*(float *)&MD2->glcommandbuffer[i++];
			float t=*(float *)&MD2->glcommandbuffer[i++];
			int index=MD2->glcommandbuffer[i++];

//			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, s, -t);
//			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, s, -t);
//			glMultiTexCoord2fARB(GL_TEXTURE2_ARB, s, -t);
			glTexCoord2f(s, -t);

			x1=MD2->frames[Frame1].vertices[index].vertex[0];
			y1=MD2->frames[Frame1].vertices[index].vertex[1];
			z1=MD2->frames[Frame1].vertices[index].vertex[2];

			x2=MD2->frames[Frame2].vertices[index].vertex[0];
			y2=MD2->frames[Frame2].vertices[index].vertex[1];
			z2=MD2->frames[Frame2].vertices[index].vertex[2];

			nx1=MD2->frames[Frame1].vertices[index].normal[0];
			ny1=MD2->frames[Frame1].vertices[index].normal[1];
			nz1=MD2->frames[Frame1].vertices[index].normal[2];

			nx2=MD2->frames[Frame2].vertices[index].normal[0];
			ny2=MD2->frames[Frame2].vertices[index].normal[1];
			nz2=MD2->frames[Frame2].vertices[index].normal[2];

			glNormal3f(nx1+Pol*(nx2-nx1), ny1+Pol*(ny2-ny1), nz1+Pol*(nz2-nz1));
			glVertex3f(x1+Pol*(x2-x1), y1+Pol*(y2-y1), z1+Pol*(z2-z1));
		}

		glEnd();

		val=MD2->glcommandbuffer[i++];
	}
}
