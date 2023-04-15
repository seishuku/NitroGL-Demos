#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "glati.h"
#include "md2.h"

extern PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
extern PFNGLVARIANTFVEXTPROC glVariantfvEXT;
extern unsigned int Tangent, Binormal;

void tangent_basis(float v0[3], float v1[3], float v2[3], float t0[2], float t1[2], float t2[2], float *tangent, float *binormal)
{
	float crossproduct[3], mag;
	float edge0[3]={ v1[0]-v0[0], t1[0]-t0[0], t1[1]-t0[1] };
	float edge1[3]={ v2[0]-v0[0], t2[0]-t0[0], t2[1]-t0[1] };

	crossproduct[0]=edge0[1]*edge1[2]-edge0[2]*edge1[1];
	crossproduct[1]=edge0[2]*edge1[0]-edge0[0]*edge1[2];
	crossproduct[2]=edge0[0]*edge1[1]-edge0[1]*edge1[0];

	if(fabs(crossproduct[0])>0.00001f)
	{
		tangent[0]=-crossproduct[1]/crossproduct[0];
		binormal[0]=-crossproduct[2]/crossproduct[0];
	}

	edge0[0]=v1[1]-v0[1];
	edge1[0]=v2[1]-v0[1];

	crossproduct[0]=edge0[1]*edge1[2]-edge0[2]*edge1[1];
	crossproduct[1]=edge0[2]*edge1[0]-edge0[0]*edge1[2];
	crossproduct[2]=edge0[0]*edge1[1]-edge0[1]*edge1[0];

	if(fabs(crossproduct[0])>0.00001f)
	{
		tangent[1]=-crossproduct[1]/crossproduct[0];
		binormal[1]=-crossproduct[2]/crossproduct[0];
	}

	edge0[0]=v1[2]-v0[2];
	edge1[0]=v2[2]-v0[2];

	crossproduct[0]=edge0[1]*edge1[2]-edge0[2]*edge1[1];
	crossproduct[1]=edge0[2]*edge1[0]-edge0[0]*edge1[2];
	crossproduct[2]=edge0[0]*edge1[1]-edge0[1]*edge1[0];

	if(fabs(crossproduct[0])>0.00001f)
	{
		tangent[2]=-crossproduct[1]/crossproduct[0];
		binormal[2]=-crossproduct[2]/crossproduct[0];
	}

	mag=1/(float)sqrt((tangent[0]*tangent[0])+(tangent[1]*tangent[1])+(tangent[2]*tangent[2]));
	tangent[0]*=mag;
	tangent[1]*=mag;
	tangent[2]*=mag;

	mag=1/(float)sqrt((binormal[0]*binormal[0])+(binormal[1]*binormal[1])+(binormal[2]*binormal[2]));
	binormal[0]*=mag;
	binormal[1]*=mag;
	binormal[2]*=mag;
}

BOOL MD2_Load(char *filename, MD2_Model_t *MD2)
{
	FILE *stream;
	int i, j, k;
	unsigned char buffer[MD2_MAX_FRAMESIZE];

	if((stream=fopen(filename, "rb"))==NULL)
		return FALSE;

	fread(&MD2->header, sizeof(MD2_Header_t), 1, stream);

	if(MD2->header.magic!=MD2_MAGIC||MD2->header.version!=MD2_VERSION)
	{
		fclose(stream);
		return FALSE;
	}

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
			MD2->frames[i].vertices[j].vertex[0]=(float)frame->alias_vertices[j].vertex[0]*frame->scale[0]+frame->translate[0];
			MD2->frames[i].vertices[j].vertex[1]=(float)frame->alias_vertices[j].vertex[1]*frame->scale[1]+frame->translate[1];
			MD2->frames[i].vertices[j].vertex[2]=(float)frame->alias_vertices[j].vertex[2]*frame->scale[2]+frame->translate[2];
		}
	}

	fseek(stream, MD2->header.offsetGlCommands, SEEK_SET);
	MD2->glcommandbuffer=malloc(sizeof(int)*MD2->header.numGlCommands);
	fread(MD2->glcommandbuffer, sizeof(int), MD2->header.numGlCommands, stream);

	fclose(stream);

	for(i=0;i<MD2->header.numFrames;i++)
	{
		for(j=0;j<MD2->header.numTriangles;j++)
		{
			float n[3], v0[3], v1[3], mag;

			v0[0]=MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[1]].vertex[0]-MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[0]].vertex[0];
			v0[1]=MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[1]].vertex[1]-MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[0]].vertex[1];
			v0[2]=MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[1]].vertex[2]-MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[0]].vertex[2];
			v1[0]=MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[2]].vertex[0]-MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[0]].vertex[0];
			v1[1]=MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[2]].vertex[1]-MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[0]].vertex[1];
			v1[2]=MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[2]].vertex[2]-MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[0]].vertex[2];

			n[0]=(v0[1]*v1[2]-v0[2]*v1[1]);
			n[1]=(v0[2]*v1[0]-v0[0]*v1[2]);
			n[2]=(v0[0]*v1[1]-v0[1]*v1[0]);

			mag=1/(float)sqrt((n[0]*n[0])+(n[1]*n[1])+(n[2]*n[2]));
			n[0]*=mag;
			n[1]*=mag;
			n[2]*=mag;

			for(k=0;k<3;k++)
			{
				MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[k]].normal[0]=-n[0];
				MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[k]].normal[1]=-n[1];
				MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[k]].normal[2]=-n[2];
			}
		}
	}

	for(i=0;i<MD2->header.numFrames;i++)
	{
		for(j=0;j<MD2->header.numTriangles;j++)
		{
			float v0[3], v1[3], v2[3], t0[2], t1[2], t2[2], tangent[3], binormal[3];

			v0[0]=MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[0]].vertex[0];
			v0[1]=MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[0]].vertex[1];
			v0[2]=MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[0]].vertex[2];

			v1[0]=MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[1]].vertex[0];
			v1[1]=MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[1]].vertex[1];
			v1[2]=MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[1]].vertex[2];

			v2[0]=MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[2]].vertex[0];
			v2[1]=MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[2]].vertex[1];
			v2[2]=MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[2]].vertex[2];

			t0[0]=(float)MD2->texcoords[MD2->triangles[j].textureIndices[0]].s/MD2->header.skinWidth;
			t0[1]=(float)MD2->texcoords[MD2->triangles[j].textureIndices[0]].t/MD2->header.skinHeight;

			t1[0]=(float)MD2->texcoords[MD2->triangles[j].textureIndices[1]].s/MD2->header.skinWidth;
			t1[1]=(float)MD2->texcoords[MD2->triangles[j].textureIndices[1]].t/MD2->header.skinHeight;

			t2[0]=(float)MD2->texcoords[MD2->triangles[j].textureIndices[2]].s/MD2->header.skinWidth;
			t2[1]=(float)MD2->texcoords[MD2->triangles[j].textureIndices[2]].t/MD2->header.skinHeight;

			tangent_basis(v0, v1, v2, t0, t1, t2, tangent, binormal);

			for(k=0;k<3;k++)
			{
				MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[k]].tangent[0]=tangent[0];
				MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[k]].tangent[1]=tangent[1];
				MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[k]].tangent[2]=tangent[2];

				MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[k]].binormal[0]=binormal[0];
				MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[k]].binormal[1]=binormal[1];
				MD2->frames[i].vertices[MD2->triangles[j].vertexIndices[k]].binormal[2]=binormal[2];
			}
		}
	}

	return TRUE;
}

void MD2_DrawFrame(MD2_Model_t *MD2, int Frame)
{
	int i, j;

	glBegin(GL_TRIANGLES);
	for(i=0;i<MD2->header.numTriangles;i++)
	{
		for(j=0;j<3;j++)
		{
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, (float)MD2->texcoords[MD2->triangles[i].textureIndices[j]].s/MD2->header.skinWidth, -(float)MD2->texcoords[MD2->triangles[i].textureIndices[j]].t/MD2->header.skinHeight);
			glVariantfvEXT(Tangent, MD2->frames[Frame].vertices[MD2->triangles[i].vertexIndices[j]].tangent);
			glVariantfvEXT(Binormal, MD2->frames[Frame].vertices[MD2->triangles[i].vertexIndices[j]].binormal);
			glNormal3f(MD2->frames[Frame].vertices[MD2->triangles[i].vertexIndices[j]].normal[0], MD2->frames[Frame].vertices[MD2->triangles[i].vertexIndices[j]].normal[1], MD2->frames[Frame].vertices[MD2->triangles[i].vertexIndices[j]].normal[2]);
			glVertex3f(MD2->frames[Frame].vertices[MD2->triangles[i].vertexIndices[j]].vertex[0], MD2->frames[Frame].vertices[MD2->triangles[i].vertexIndices[j]].vertex[1], MD2->frames[Frame].vertices[MD2->triangles[i].vertexIndices[j]].vertex[2]);
		}
	}
	glEnd();
}
