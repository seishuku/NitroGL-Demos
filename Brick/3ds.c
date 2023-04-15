#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include "3ds.h"

#define MAX_STRIP 1024

#ifndef FREE
#define FREE(p) { if(p) { free(p); p=NULL; } }
#endif

typedef void (*CallBack)(TDS_Chunk_t *, void *);

const float OneOver255=1.0f/255.0f;
const float OneOver100=1.0f/100.0f;

FILE *stream;
char string[64];

void CalculateMeshBoundingBox(TDS_Mesh_t *Mesh)
{
	int i;

	for(i=0;i<Mesh->NumVertex;i++)
	{
		if(i==0)
		{
			Mesh->Max[0]=Mesh->Vertex[3*i];
			Mesh->Max[1]=Mesh->Vertex[3*i+1];
			Mesh->Max[2]=Mesh->Vertex[3*i+2];

			Mesh->Min[0]=Mesh->Vertex[3*i];
			Mesh->Min[1]=Mesh->Vertex[3*i+1];
			Mesh->Min[2]=Mesh->Vertex[3*i+2];
		}
		else
		{
			if(Mesh->Vertex[3*i]>Mesh->Max[0])
				Mesh->Max[0]=Mesh->Vertex[3*i];

			if(Mesh->Vertex[3*i]<Mesh->Min[0])
				Mesh->Min[0]=Mesh->Vertex[3*i];

			if(Mesh->Vertex[3*i+1]>Mesh->Max[1])
				Mesh->Max[1]=Mesh->Vertex[3*i+1];

			if(Mesh->Vertex[3*i+1]<Mesh->Min[1])
				Mesh->Min[1]=Mesh->Vertex[3*i+1];

			if(Mesh->Vertex[3*i+2]>Mesh->Max[2])
				Mesh->Max[2]=Mesh->Vertex[3*i+2];

			if(Mesh->Vertex[3*i+2]<Mesh->Min[2])
				Mesh->Min[2]=Mesh->Vertex[3*i+2];

		}
	}

	Mesh->Center[0]=Mesh->Min[0]+(Mesh->Max[0]-Mesh->Min[0])*0.5f;
	Mesh->Center[1]=Mesh->Min[1]+(Mesh->Max[1]-Mesh->Min[1])*0.5f;
	Mesh->Center[2]=Mesh->Min[2]+(Mesh->Max[2]-Mesh->Min[2])*0.5f;
}

void CalculateMeshNormals(TDS_Mesh_t *Mesh)
{
	float v0[3], v1[3], n[3], mag;
	int i, j;

	Mesh->Normal=(float *)malloc(sizeof(float)*Mesh->NumVertex*3);

	if(!Mesh->Normal)
		return;

	memset(Mesh->Normal, 0, sizeof(float)*Mesh->NumVertex*3);

	for(i=0;i<Mesh->NumFace;i++)
	{
		v0[0]=Mesh->Vertex[3*Mesh->Face[3*i+1]]-Mesh->Vertex[3*Mesh->Face[3*i]];
		v0[1]=Mesh->Vertex[3*Mesh->Face[3*i+1]+1]-Mesh->Vertex[3*Mesh->Face[3*i]+1];
		v0[2]=Mesh->Vertex[3*Mesh->Face[3*i+1]+2]-Mesh->Vertex[3*Mesh->Face[3*i]+2];

		v1[0]=Mesh->Vertex[3*Mesh->Face[3*i+2]]-Mesh->Vertex[3*Mesh->Face[3*i]];
		v1[1]=Mesh->Vertex[3*Mesh->Face[3*i+2]+1]-Mesh->Vertex[3*Mesh->Face[3*i]+1];
		v1[2]=Mesh->Vertex[3*Mesh->Face[3*i+2]+2]-Mesh->Vertex[3*Mesh->Face[3*i]+2];

		n[0]=(v0[1]*v1[2]-v0[2]*v1[1]);
		n[1]=(v0[2]*v1[0]-v0[0]*v1[2]);
		n[2]=(v0[0]*v1[1]-v0[1]*v1[0]);

		for(j=0;j<3;j++)
		{
			Mesh->Normal[3*Mesh->Face[3*i+j]]+=n[0];
			Mesh->Normal[3*Mesh->Face[3*i+j]+1]+=n[1];
			Mesh->Normal[3*Mesh->Face[3*i+j]+2]+=n[2];
		}
	}

	for(i=0;i<Mesh->NumVertex;i++)
	{
		mag=1.0f/(float)sqrt(Mesh->Normal[3*i]*Mesh->Normal[3*i]+Mesh->Normal[3*i+1]*Mesh->Normal[3*i+1]+Mesh->Normal[3*i+2]*Mesh->Normal[3*i+2]);
		Mesh->Normal[3*i]*=mag;
		Mesh->Normal[3*i+1]*=mag;
		Mesh->Normal[3*i+2]*=mag;
	}
}

void CalculateMeshTangents(TDS_Mesh_t *Mesh)
{
	float v0[3], v1[3], cross[3], mag;
	float s[3], t[3];
	int i, j;

	if(!Mesh->NumUV)
		return;

	Mesh->Tangent=(float *)malloc(sizeof(float)*Mesh->NumVertex*3);

	if(!Mesh->Tangent)
		return;

	memset(Mesh->Tangent, 0, sizeof(float)*Mesh->NumVertex*3);

	Mesh->Binormal=(float *)malloc(sizeof(float)*Mesh->NumVertex*3);

	if(!Mesh->Binormal)
		return;

	memset(Mesh->Binormal, 0, sizeof(float)*Mesh->NumVertex*3);

	for(i=0;i<Mesh->NumFace;i++)
	{
		v0[0]=Mesh->Vertex[3*Mesh->Face[3*i+1]]-Mesh->Vertex[3*Mesh->Face[3*i]];
		v0[1]=Mesh->UV[2*Mesh->Face[3*i+1]]-Mesh->UV[2*Mesh->Face[3*i]];
		v0[2]=Mesh->UV[2*Mesh->Face[3*i+1]+1]-Mesh->UV[2*Mesh->Face[3*i]+1];

		v1[0]=Mesh->Vertex[3*Mesh->Face[3*i+2]]-Mesh->Vertex[3*Mesh->Face[3*i]];
		v1[1]=Mesh->UV[2*Mesh->Face[3*i+2]]-Mesh->UV[2*Mesh->Face[3*i]];
		v1[2]=Mesh->UV[2*Mesh->Face[3*i+2]+1]-Mesh->UV[2*Mesh->Face[3*i]+1];

		cross[0]=(v0[1]*v1[2]-v0[2]*v1[1]);
		cross[1]=(v0[2]*v1[0]-v0[0]*v1[2]);
		cross[2]=(v0[0]*v1[1]-v0[1]*v1[0]);

		if(fabs(cross[0])>0.000001f)
		{
			s[0]=-cross[1]/cross[0];
			t[0]=cross[2]/cross[0];
		}

		v0[0]=Mesh->Vertex[3*Mesh->Face[3*i+1]+1]-Mesh->Vertex[3*Mesh->Face[3*i]+1];
		v0[1]=Mesh->UV[2*Mesh->Face[3*i+1]]-Mesh->UV[2*Mesh->Face[3*i]];
		v0[2]=Mesh->UV[2*Mesh->Face[3*i+1]+1]-Mesh->UV[2*Mesh->Face[3*i]+1];

		v1[0]=Mesh->Vertex[3*Mesh->Face[3*i+2]+1]-Mesh->Vertex[3*Mesh->Face[3*i]+1];
		v1[1]=Mesh->UV[2*Mesh->Face[3*i+2]]-Mesh->UV[2*Mesh->Face[3*i]];
		v1[2]=Mesh->UV[2*Mesh->Face[3*i+2]+1]-Mesh->UV[2*Mesh->Face[3*i]+1];

		cross[0]=(v0[1]*v1[2]-v0[2]*v1[1]);
		cross[1]=(v0[2]*v1[0]-v0[0]*v1[2]);
		cross[2]=(v0[0]*v1[1]-v0[1]*v1[0]);

		if(fabs(cross[0])>0.000001f)
		{
			s[1]=-cross[1]/cross[0];
			t[1]=cross[2]/cross[0];
		}

		v0[0]=Mesh->Vertex[3*Mesh->Face[3*i+1]+2]-Mesh->Vertex[3*Mesh->Face[3*i]+2];
		v0[1]=Mesh->UV[2*Mesh->Face[3*i+1]]-Mesh->UV[2*Mesh->Face[3*i]];
		v0[2]=Mesh->UV[2*Mesh->Face[3*i+1]+1]-Mesh->UV[2*Mesh->Face[3*i]+1];

		v1[0]=Mesh->Vertex[3*Mesh->Face[3*i+2]+2]-Mesh->Vertex[3*Mesh->Face[3*i]+2];
		v1[1]=Mesh->UV[2*Mesh->Face[3*i+2]]-Mesh->UV[2*Mesh->Face[3*i]];
		v1[2]=Mesh->UV[2*Mesh->Face[3*i+2]+1]-Mesh->UV[2*Mesh->Face[3*i]+1];

		cross[0]=(v0[1]*v1[2]-v0[2]*v1[1]);
		cross[1]=(v0[2]*v1[0]-v0[0]*v1[2]);
		cross[2]=(v0[0]*v1[1]-v0[1]*v1[0]);

		if(fabs(cross[0])>0.000001f)
		{
			s[2]=-cross[1]/cross[0];
			t[2]=cross[2]/cross[0];
		}

		for(j=0;j<3;j++)
		{
			Mesh->Tangent[3*Mesh->Face[3*i+j]]+=s[0];
			Mesh->Tangent[3*Mesh->Face[3*i+j]+1]+=s[1];
			Mesh->Tangent[3*Mesh->Face[3*i+j]+2]+=s[2];

			Mesh->Binormal[3*Mesh->Face[3*i+j]]+=t[0];
			Mesh->Binormal[3*Mesh->Face[3*i+j]+1]+=t[1];
			Mesh->Binormal[3*Mesh->Face[3*i+j]+2]+=t[2];
		}
	}

	for(i=0;i<Mesh->NumVertex;i++)
	{
		mag=1.0f/(float)sqrt(Mesh->Tangent[3*i]*Mesh->Tangent[3*i]+Mesh->Tangent[3*i+1]*Mesh->Tangent[3*i+1]+Mesh->Tangent[3*i+2]*Mesh->Tangent[3*i+2]);
		Mesh->Tangent[3*i]*=mag;
		Mesh->Tangent[3*i+1]*=mag;
		Mesh->Tangent[3*i+2]*=mag;

		mag=1.0f/(float)sqrt(Mesh->Binormal[3*i]*Mesh->Binormal[3*i]+Mesh->Binormal[3*i+1]*Mesh->Binormal[3*i+1]+Mesh->Binormal[3*i+2]*Mesh->Binormal[3*i+2]);
		Mesh->Binormal[3*i]*=mag;
		Mesh->Binormal[3*i+1]*=mag;
		Mesh->Binormal[3*i+2]*=mag;
	}
}

void Weld(TDS_Mesh_t *Mesh)
{
	float *NewVertex, *NewUV;
	unsigned short i, j, k;

	NewVertex=(float *)malloc(sizeof(float)*Mesh->NumVertex*3);
	memcpy(NewVertex, Mesh->Vertex, sizeof(float)*Mesh->NumVertex*3);

	NewUV=(float *)malloc(sizeof(float)*Mesh->NumVertex*2);
	memcpy(NewUV, Mesh->UV, sizeof(float)*Mesh->NumVertex*2);

	k=0;

	for(i=0;i<Mesh->NumVertex;i++)
	{
		unsigned char Found=0;

		for(j=0;j<k;j++)
		{
			if(NewVertex[3*j]==Mesh->Vertex[3*i]&&NewVertex[3*j+1]==Mesh->Vertex[3*i+1]&&NewVertex[3*j+2]==Mesh->Vertex[3*i+2])
			{
				if(NewUV[2*j]==Mesh->UV[2*i]&&NewUV[2*j+1]==Mesh->UV[2*i+1])
				{
					Found=1;
					break;
				}
			}
		}

		if(!Found)
		{
			NewVertex[3*k]=Mesh->Vertex[3*i];
			NewVertex[3*k+1]=Mesh->Vertex[3*i+1];
			NewVertex[3*k+2]=Mesh->Vertex[3*i+2];

			NewUV[2*k]=Mesh->UV[2*i];
			NewUV[2*k+1]=Mesh->UV[2*i+1];

			j=k;
			k++;
		}

		Mesh->Vertex[3*i]=(float)j;
	}

	Mesh->NumVertex=k;

	for(i=0;i<Mesh->NumFace;i++)
	{
		Mesh->Face[3*i]=(unsigned short)Mesh->Vertex[3*Mesh->Face[3*i]];
		Mesh->Face[3*i+1]=(unsigned short)Mesh->Vertex[3*Mesh->Face[3*i+1]];
		Mesh->Face[3*i+2]=(unsigned short)Mesh->Vertex[3*Mesh->Face[3*i+2]];
	}

	FREE(Mesh->Vertex);
	Mesh->Vertex=NewVertex;

	FREE(Mesh->UV);
	Mesh->UV=NewUV;
}

void ReadASCIIZ(char *string)
{
	int i=0, j=1;

	while(j)
	{
		fread(&string[i], sizeof(char), 1, stream);

		if(string[i]=='\0')
			j=0;

		i++;
	}
}

void ReadChunkArray(long length, void (*callback)(TDS_Chunk_t *, void *), void *data)
{
	TDS_Chunk_t Chunk;
	long Start, Position=0;

	while(Position<length)
	{
		Start=ftell(stream);
		fread(&Chunk, sizeof(TDS_Chunk_t), 1, stream);
		callback(&Chunk, data);
		fseek(stream, Start+Chunk.Length, SEEK_SET);
		Position+=Chunk.Length;
	}
}

void InspectChunkArray(long length, void (*callback)(TDS_Chunk_t *, void *), void *data)
{
	TDS_Chunk_t Chunk;
	long Start, ChunkStart, Position=0;

	ChunkStart=ftell(stream);

	while(Position<length)
	{
		Start=ftell(stream);
		fread(&Chunk, sizeof(TDS_Chunk_t), 1, stream);
		callback(&Chunk, data);
		fseek(stream, Start+Chunk.Length, SEEK_SET);
		Position+=Chunk.Length;
	}

	fseek(stream, ChunkStart, SEEK_SET);
}

void InspectEditObject(TDS_Chunk_t *Chunk, TDS_Model_t *Model)
{
	switch(Chunk->ID)
	{
		case 0x4100:
			Model->NumMesh++;
			break;

		default:
			break;
	}
}

void InspectEdit3DS(TDS_Chunk_t *Chunk, TDS_Model_t *Model)
{
	switch(Chunk->ID)
	{
		case 0xAFFF:
			Model->NumMaterial++;
			break;

		case 0x4000:
			ReadASCIIZ(string);
			InspectChunkArray(Chunk->Length-6+strlen(string), (CallBack)InspectEditObject, Model);
			break;

		default:
			break;
	}
}

void ReadVertexList(TDS_Mesh_t *Model)
{
	fread(&Model->NumVertex, sizeof(unsigned short), 1, stream);
	Model->Vertex=(float *)malloc(sizeof(float)*Model->NumVertex*3);
	fread(Model->Vertex, sizeof(float), Model->NumVertex*3, stream);
}

void ReadMaterialName(TDS_Chunk_t *Chunk, TDS_Mesh_t *Model)
{
	switch(Chunk->ID)
	{
		case 0x4130:
			ReadASCIIZ(Model->MaterialName);
			break;

		default:
			break;
	}
}

void ReadFaceList(TDS_Mesh_t *Model, long length)
{
	long pos=6;
	unsigned short i, temp;

	fread(&Model->NumFace, sizeof(unsigned short), 1, stream);

	Model->Face=(unsigned short *)malloc(sizeof(unsigned short)*Model->NumFace*3);

	pos+=sizeof(unsigned short);

	for(i=0;i<Model->NumFace;i++)
	{
		fread(&Model->Face[3*i], sizeof(unsigned short), 3, stream);
		fread(&temp, sizeof(unsigned short), 1, stream);
	}

	pos+=sizeof(unsigned short)*4*Model->NumFace;

	if(pos<length)
		ReadChunkArray(pos-6, (CallBack)ReadMaterialName, Model);
}

void ReadTextureCoordinates(TDS_Mesh_t *Model)
{
	fread(&Model->NumUV, sizeof(unsigned short), 1, stream);
	Model->UV=(float *)malloc(sizeof(float)*Model->NumUV*2);
	fread(Model->UV, sizeof(float), Model->NumUV*2, stream);
}

void ReadLocalAxis(TDS_Mesh_t *Model)
{
	fread(Model->Axis, sizeof(float), 9, stream);
	fread(Model->Position, sizeof(float), 3, stream);
}

void ReadTriangleObject(TDS_Chunk_t *Chunk, TDS_Mesh_t *Model)
{
	switch(Chunk->ID)
	{
		case 0x4110:
			ReadVertexList(Model);
			break;

		case 0x4120:
			ReadFaceList(Model, Chunk->Length-6);
			break;

		case 0x4140:
			ReadTextureCoordinates(Model);
			break;

		case 0x4160:
			ReadLocalAxis(Model);
			break;

		default:
			break;
	}
}

void ReadEditObject(TDS_Chunk_t *Chunk, TDS_Model_t *Model)
{
	switch(Chunk->ID)
	{
		case 0x4100:
			strcpy(Model->Mesh[Model->NumMesh].Name, string);
			ReadChunkArray(Chunk->Length-6, (CallBack)ReadTriangleObject, &Model->Mesh[Model->NumMesh++]);
			break;

		default:
			break;
	}
}

void ReadTexture(TDS_Chunk_t *Chunk, TDS_Material_t *Material)
{
	switch(Chunk->ID)
	{
		case 0xa300:
			ReadASCIIZ(Material->Texture);
			break;

		default:
			break;
	}
}

void ReadColorChunk(TDS_Chunk_t *Chunk, float *fColor)
{
	unsigned char rgb[3];

	switch(Chunk->ID)
	{
		case 0x0010:
			fread(fColor, sizeof(float), 3, stream);
			break;

		case 0x0011:
			fread(rgb, sizeof(unsigned char), 3, stream);

			fColor[0]=(float)rgb[0]*OneOver255;
			fColor[1]=(float)rgb[1]*OneOver255;
			fColor[2]=(float)rgb[2]*OneOver255;
			break;

		default:
			break;
	}
}

void ReadPercentageChunk(TDS_Chunk_t *Chunk, float *value)
{
	unsigned short temp;

	switch(Chunk->ID)
	{
		case 0x0030:
			fread(&temp, sizeof(unsigned short), 1, stream);

			*value=(float)temp;
			break;

		case 0x0031:
			fread(value, sizeof(float), 1, stream);
			break;

		default:
			break;
	};

	*value*=OneOver100;
}

void ReadMaterial(TDS_Chunk_t *Chunk, TDS_Material_t *Material)
{
	switch(Chunk->ID)
	{
		case 0xa000:
			ReadASCIIZ(Material->Name);
			break;

		case 0xa010:
			ReadChunkArray(Chunk->Length-6, (CallBack)ReadColorChunk, Material->Ambient);
			break;

		case 0xa020:
			ReadChunkArray(Chunk->Length-6, (CallBack)ReadColorChunk, Material->Diffuse);
			break;

		case 0xa030:
			ReadChunkArray(Chunk->Length-6, (CallBack)ReadColorChunk, Material->Specular);
			break;

		case 0xa040:
			ReadChunkArray(Chunk->Length-6, (CallBack)ReadPercentageChunk, &Material->Shininess);
			break;

		case 0xa080:
			ReadChunkArray(Chunk->Length-6, ReadColorChunk, Material->Emission);
			break;

		case 0xa200:
			ReadChunkArray(Chunk->Length-6, ReadTexture, Material);
			break;

		default:
			break;
	}
}

void ReadEdit3DS(TDS_Chunk_t *Chunk, TDS_Model_t *Model)
{
	switch(Chunk->ID)
	{
		case 0x4000:
			ReadASCIIZ(string);
			ReadChunkArray(Chunk->Length-6+strlen(string), (CallBack)ReadEditObject, Model);
			break;

		case 0xAFFF:
			ReadChunkArray(Chunk->Length-6, (CallBack)ReadMaterial, &Model->Material[Model->NumMaterial++]);
			break;

		default:
			break;
	}
}

void ReadMain3DS(TDS_Chunk_t *Chunk, TDS_Model_t *Model)
{
	switch(Chunk->ID)
	{
		case 0x3d3d:
			InspectChunkArray(Chunk->Length-6, (CallBack)InspectEdit3DS, Model);

			if(Model->NumMesh)
			{
				Model->Mesh=(TDS_Mesh_t *)malloc(sizeof(TDS_Mesh_t)*Model->NumMesh);
				memset(Model->Mesh, 0, sizeof(TDS_Mesh_t)*Model->NumMesh);
			}

			if(Model->NumMaterial)
			{
				Model->Material=(TDS_Material_t *)malloc(sizeof(TDS_Material_t)*Model->NumMaterial);
				memset(Model->Material, 0, sizeof(TDS_Material_t)*Model->NumMaterial);
			}

			Model->NumMesh=0;
			Model->NumMaterial=0;

			ReadChunkArray(Chunk->Length-6, (CallBack)ReadEdit3DS, Model);
			break;

		default:
			break;
	}
}

void TDS_Free(TDS_Model_t *Model)
{
	int i;

	if(Model->Mesh)
	{
		for(i=0;i<Model->NumMesh;i++)
		{
			FREE(Model->Mesh[i].Vertex);
			FREE(Model->Mesh[i].Face);
			FREE(Model->Mesh[i].Tangent);
			FREE(Model->Mesh[i].Binormal);
			FREE(Model->Mesh[i].Normal);
			FREE(Model->Mesh[i].UV);

			Model->Mesh[i].NumVertex=0;
			Model->Mesh[i].NumUV=0;
			Model->Mesh[i].NumFace=0;
		}

		FREE(Model->Mesh);
	}

	Model->NumMesh=0;

	FREE(Model->Material);
	Model->NumMaterial=0;
}

int TDS_LoadModel(TDS_Model_t *Model, char *fileName)
{
	int i, j;
	TDS_Chunk_t Chunk;

	memset(Model, 0, sizeof(TDS_Model_t));

	stream=NULL;

	if((stream=fopen(fileName, "rb"))==NULL)
		return 0;

	fread(&Chunk, sizeof(TDS_Chunk_t), 1, stream);

	if(Chunk.ID==0x4d4d)
		ReadChunkArray(Chunk.Length-6, (CallBack)ReadMain3DS, Model);

	fclose(stream);

	Model->TotalFaces=0;

	for(i=0;i<Model->NumMesh;i++)
	{
		if(Model->Mesh[i].MaterialName)
		{
			for(j=0;j<Model->NumMaterial;j++)
			{
				if(strcmp(Model->Mesh[i].MaterialName, Model->Material[j].Name)==0)
					Model->Mesh[i].MaterialIndex=(unsigned short)j;
			}
		}

		Weld(&Model->Mesh[i]);
		CalculateMeshNormals(&Model->Mesh[i]);
		CalculateMeshTangents(&Model->Mesh[i]);
		CalculateMeshBoundingBox(&Model->Mesh[i]);

		Model->TotalFaces+=Model->Mesh[i].NumFace;

		if(i==0)
		{
			Model->Max[0]=Model->Mesh[i].Max[0];
			Model->Max[1]=Model->Mesh[i].Max[1];
			Model->Max[2]=Model->Mesh[i].Max[2];

			Model->Min[0]=Model->Mesh[i].Min[0];
			Model->Min[1]=Model->Mesh[i].Min[1];
			Model->Min[2]=Model->Mesh[i].Min[2];
		}
		else
		{
			if(Model->Mesh[i].Max[0]>Model->Max[0])
				Model->Max[0]=Model->Mesh[i].Max[0];

			if(Model->Mesh[i].Min[0]<Model->Min[0])
				Model->Min[0]=Model->Mesh[i].Min[0];

			if(Model->Mesh[i].Max[1]>Model->Max[1])
				Model->Max[1]=Model->Mesh[i].Max[1];

			if(Model->Mesh[i].Min[1]<Model->Min[1])
				Model->Min[1]=Model->Mesh[i].Min[1];

			if(Model->Mesh[i].Max[2]>Model->Max[2])
				Model->Max[2]=Model->Mesh[i].Max[2];

			if(Model->Mesh[i].Min[2]<Model->Min[2])
				Model->Min[2]=Model->Mesh[i].Min[2];
		}

		Model->Center[0]=Model->Min[0]+(Model->Max[0]-Model->Min[0])*0.5f;
		Model->Center[1]=Model->Min[1]+(Model->Max[1]-Model->Min[1])*0.5f;
		Model->Center[2]=Model->Min[2]+(Model->Max[2]-Model->Min[2])*0.5f;
	}

	return 1;
}
