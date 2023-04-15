// A very crude and naive rewrite of the old 3DS loader, which had issues with x64.

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <memory.h>
#include "math.h"
#include "3ds.h"

#ifdef _linux_
#include <inttypes.h>
#else
typedef unsigned int uint32_t;
#endif

#ifndef FREE
#define FREE(p) { if(p) { free(p); p=NULL; } }
#endif

void CalculateTangent(Mesh3DS_t *Mesh)
{
	int i;
	float v0[3], v1[3], n[3];

	Mesh->Normal=(float *)malloc(sizeof(float)*3*Mesh->NumVertex);

	if(Mesh->Normal==NULL)
		return;

	memset(Mesh->Normal, 0, sizeof(float)*3*Mesh->NumVertex);

	for(i=0;i<Mesh->NumFace;i++)
	{
		unsigned short i1=Mesh->Face[3*i+0];
		unsigned short i2=Mesh->Face[3*i+1];
		unsigned short i3=Mesh->Face[3*i+2];

		v0[0]=Mesh->Vertex[3*i2+0]-Mesh->Vertex[3*i1+0];
		v0[1]=Mesh->Vertex[3*i2+1]-Mesh->Vertex[3*i1+1];
		v0[2]=Mesh->Vertex[3*i2+2]-Mesh->Vertex[3*i1+2];

		v1[0]=Mesh->Vertex[3*i3+0]-Mesh->Vertex[3*i1+0];
		v1[1]=Mesh->Vertex[3*i3+1]-Mesh->Vertex[3*i1+1];
		v1[2]=Mesh->Vertex[3*i3+2]-Mesh->Vertex[3*i1+2];

		Cross(v0, v1, n);
		Normalize(n);

		Mesh->Normal[3*i1+0]+=n[0];		Mesh->Normal[3*i1+1]+=n[1];		Mesh->Normal[3*i1+2]+=n[2];
		Mesh->Normal[3*i2+0]+=n[0];		Mesh->Normal[3*i2+1]+=n[1];		Mesh->Normal[3*i2+2]+=n[2];
		Mesh->Normal[3*i3+0]+=n[0];		Mesh->Normal[3*i3+1]+=n[1];		Mesh->Normal[3*i3+2]+=n[2];
	}
}

int Load3DS(Model3DS_t *Model, char *Filename)
{
	FILE *Stream;
	long i, Length;
	unsigned short ChunkID;
	uint32_t ChunkLength;
	uint32_t Temp;
	unsigned short Temp16;
	char *Ptr;
	unsigned char RGB[3];
	float *ColorPtr;

	if((Stream=fopen(Filename, "rb"))==NULL)
		return 0;

	fseek(Stream, 0, SEEK_END);
	Length=ftell(Stream);
	fseek(Stream, 0, SEEK_SET);

	while(ftell(Stream)<Length)
	{
		fread(&ChunkID, sizeof(unsigned short), 1, Stream);
		fread(&ChunkLength, sizeof(uint32_t), 1, Stream);

		switch(ChunkID)
		{
			case 0x4D4D:
				break;

			case 0x3D3D:
				break;

			case 0x4000:
				Model->NumMesh++;

				Model->Mesh=(Mesh3DS_t *)realloc(Model->Mesh, sizeof(Mesh3DS_t)*Model->NumMesh);

				if(Model->Mesh==NULL)
				{
					fclose(Stream);
					return 0;
				}

				memset(&Model->Mesh[Model->NumMesh-1], 0, sizeof(Mesh3DS_t));

				Ptr=Model->Mesh[Model->NumMesh-1].Name;

				for(;;Ptr++)
				{
					fread(Ptr, sizeof(char), 1, Stream);

					if(*Ptr=='\0')
						break;
				}
				break;

			case 0x4100:
				break;

			case 0x4110:
				fread(&Model->Mesh[Model->NumMesh-1].NumVertex, sizeof(unsigned short), 1, Stream);

				if(!Model->Mesh[Model->NumMesh-1].NumVertex)
					break;

				Model->Mesh[Model->NumMesh-1].Vertex=(float *)malloc(3*sizeof(float)*Model->Mesh[Model->NumMesh-1].NumVertex);

				if(Model->Mesh[Model->NumMesh-1].Vertex==NULL)
				{
					if(Model->Mesh)
					{
						for(i=0;i<Model->NumMesh;i++)
						{
							FREE(Model->Mesh[i].Vertex);
							FREE(Model->Mesh[i].UV);
							FREE(Model->Mesh[i].Normal);
							FREE(Model->Mesh[i].Face);
						}

						FREE(Model->Mesh);
					}

					fclose(Stream);

					return 0;
				}

				fread(Model->Mesh[Model->NumMesh-1].Vertex, sizeof(float), 3*Model->Mesh[Model->NumMesh-1].NumVertex, Stream);

				for(i=0;i<Model->Mesh[Model->NumMesh-1].NumVertex;i++)
				{
					float Temp=Model->Mesh[Model->NumMesh-1].Vertex[3*i+1];
					Model->Mesh[Model->NumMesh-1].Vertex[3*i+1]=Model->Mesh[Model->NumMesh-1].Vertex[3*i+2];
					Model->Mesh[Model->NumMesh-1].Vertex[3*i+2]=-Temp;
				}
				break;

			case 0x4120:
				fread(&Model->Mesh[Model->NumMesh-1].NumFace, sizeof(unsigned short), 1, Stream);

				if(!Model->Mesh[Model->NumMesh-1].NumFace)
					break;

				Model->Mesh[Model->NumMesh-1].Face=(unsigned short *)malloc(3*sizeof(unsigned short)*Model->Mesh[Model->NumMesh-1].NumFace);

				if(Model->Mesh[Model->NumMesh-1].Face==NULL)
				{
					if(Model->Mesh)
					{
						for(i=0;i<Model->NumMesh;i++)
						{
							FREE(Model->Mesh[i].Vertex);
							FREE(Model->Mesh[i].UV);
							FREE(Model->Mesh[i].Normal);
							FREE(Model->Mesh[i].Face);
						}

						FREE(Model->Mesh);
					}

					fclose(Stream);

					return 0;
				}

				for(i=0;i<Model->Mesh[Model->NumMesh-1].NumFace;i++)
				{
					fread(&Model->Mesh[Model->NumMesh-1].Face[3*i], sizeof(unsigned short), 3, Stream);
					fread(&Temp, sizeof(unsigned short), 1, Stream);
				}
				break;

			case 0x4130:
				Ptr=Model->Mesh[Model->NumMesh-1].MaterialName;

				for(;;Ptr++)
				{
					fread(Ptr, sizeof(char), 1, Stream);

					if(*Ptr=='\0')
						break;
				}

				// Skip face groups, probably should read these though.
				fread(&Temp16, sizeof(unsigned short), 1, Stream);
				fseek(Stream, sizeof(unsigned short)*Temp16, SEEK_CUR);
				break;

			case 0x4140:
				fread(&Temp, sizeof(unsigned short), 1, Stream);

				if(!Temp||Temp!=Model->Mesh[Model->NumMesh-1].NumVertex)
					break;

				Model->Mesh[Model->NumMesh-1].UV=(float *)malloc(2*sizeof(float)*Model->Mesh[Model->NumMesh-1].NumVertex);

				if(Model->Mesh[Model->NumMesh-1].UV==NULL)
				{
					if(Model->Mesh)
					{
						for(i=0;i<Model->NumMesh;i++)
						{
							FREE(Model->Mesh[i].Vertex);
							FREE(Model->Mesh[i].UV);
							FREE(Model->Mesh[i].Normal);
							FREE(Model->Mesh[i].Face);
						}

						FREE(Model->Mesh);
					}

					fclose(Stream);

					return 0;
				}

				fread(Model->Mesh[Model->NumMesh-1].UV, sizeof(float), 2*Model->Mesh[Model->NumMesh-1].NumVertex, Stream);
				break;

			case 0xAFFF:
				break;

			case 0xA000:
				Model->NumMaterial++;

				Model->Material=(Material3DS_t *)realloc(Model->Material, sizeof(Material3DS_t)*Model->NumMaterial);

				if(Model->Material==NULL)
				{
					fclose(Stream);
					return 0;
				}

				memset(&Model->Material[Model->NumMaterial-1], 0, sizeof(Material3DS_t));

				Ptr=Model->Material[Model->NumMaterial-1].Name;

				for(;;Ptr++)
				{
					fread(Ptr, sizeof(char), 1, Stream);

					if(*Ptr=='\0')
						break;
				}
				break;

			case 0xA020:
				ColorPtr=Model->Material[Model->NumMaterial-1].Diffuse;
				break;

			case 0x0011:
				fread(RGB, sizeof(unsigned char), 3, Stream);

				*ColorPtr++=(float)RGB[0]/255.0f;
				*ColorPtr++=(float)RGB[1]/255.0f;
				*ColorPtr++=(float)RGB[2]/255.0f;
				*ColorPtr++=1.0f;
				break;

			default:
				fseek(Stream, ChunkLength-6, SEEK_CUR);
		}
	}

	fclose(Stream);

	return 1;
}

void Free3DS(Model3DS_t *Model)
{
	int i;

	if(Model->Mesh)
	{
		for(i=0;i<Model->NumMesh;i++)
		{
			FREE(Model->Mesh[i].Vertex);
			FREE(Model->Mesh[i].UV);
			FREE(Model->Mesh[i].Face);
			FREE(Model->Mesh[i].Normal);
		}

		FREE(Model->Mesh);
	}

	FREE(Model->Material);
}
