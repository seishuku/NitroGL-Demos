#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "3ds.h"
#include "image.h"
#include "glati.h"

#define FREE(X) { if(X) { free(X); (X)=NULL; } }
#define FABS(X) ((X)<0.0f?-(X):(X))

#define TDS_MAX_MESHES 300

#define TDS_MAGIC 0x4D4D
#define TDS_VERSION 0x0002
#define TDS_DATA 0x3D3D
#define TDS_NAMED_OBJECT 0x4000
#define TDS_TRIANGLE_OBJECT 0x4100
#define	TDS_DIRECT_LIGHT 0x4600
#define TDS_CAMERA 0x4700
#define TDS_VERTEX_ARRAY 0x4110
#define TDS_FACE_ARRAY 0x4120
#define TDS_UV_ARRAY 0x4140

#define TDS_MATERIAL_GROUP 0x4130
#define TDS_MATERIAL_ENTRY 0xAFFF
#define TDS_MATERIAL_NAME 0xA000
#define TDS_MATERIAL_AMBIENT 0xA010
#define TDS_MATERIAL_DIFFUSE 0xA020
#define TDS_MATERIAL_SPECULAR 0xA030
#define TDS_MATERIAL_SHININESS 0xA040
#define TDS_MATERIAL_TRANSPARENCY 0xA050
#define TDS_MATERIAL_TEXTURE_MAP 0xA200

#define TDS_INTEGER_PERCENTAGE 0x0030
#define TDS_FLOAT_PERCENTAGE 0x0031
#define TDS_COLOR_24BIT 0x0011
#define TDS_COLOR_FLOAT 0x0010

const float OneOver255=1.0f/255.0f;

void CalculateNormals(TDS_Mesh_t *Mesh)
{
	float v0[3], v1[3], n[3], mag;
	int i, j;

	Mesh->Normal=(float *)malloc(Mesh->NumVertex*3*sizeof(float));
	memset(Mesh->Normal, 0, Mesh->NumVertex*3*sizeof(float));

	for(i=0;i<Mesh->NumFaces;i++)
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
		mag=1/(float)sqrt(Mesh->Normal[3*i]*Mesh->Normal[3*i]+Mesh->Normal[3*i+1]*Mesh->Normal[3*i+1]+Mesh->Normal[3*i+2]*Mesh->Normal[3*i+2]);
		Mesh->Normal[3*i]*=mag;
		Mesh->Normal[3*i+1]*=mag;
		Mesh->Normal[3*i+2]*=mag;
	}
}

void CalculateTangents(TDS_Mesh_t *Mesh)
{
	float v0[3], v1[3], cross[3], mag;
	float s[3], t[3];
	int i, j;

	if(!Mesh->NumUV)
		return;

	Mesh->Tangent=(float *)malloc(Mesh->NumVertex*3*sizeof(float));
	memset(Mesh->Tangent, 0, Mesh->NumVertex*3*sizeof(float));

	Mesh->Binormal=(float *)malloc(Mesh->NumVertex*3*sizeof(float));
	memset(Mesh->Binormal, 0, Mesh->NumVertex*3*sizeof(float));

	for(i=0;i<Mesh->NumFaces;i++)
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
		mag=1/(float)sqrt(Mesh->Tangent[3*i]*Mesh->Tangent[3*i]+Mesh->Tangent[3*i+1]*Mesh->Tangent[3*i+1]+Mesh->Tangent[3*i+2]*Mesh->Tangent[3*i+2]);
		Mesh->Tangent[3*i]*=mag;
		Mesh->Tangent[3*i+1]*=mag;
		Mesh->Tangent[3*i+2]*=mag;

		mag=1/(float)sqrt(Mesh->Binormal[3*i]*Mesh->Binormal[3*i]+Mesh->Binormal[3*i+1]*Mesh->Binormal[3*i+1]+Mesh->Binormal[3*i+2]*Mesh->Binormal[3*i+2]);
		Mesh->Binormal[3*i]*=mag;
		Mesh->Binormal[3*i+1]*=mag;
		Mesh->Binormal[3*i+2]*=mag;
	}
}

BOOL TDS_LoadModel(TDS_Model_t *Model, char *filename)
{
	FILE *stream;
	int MeshPos[TDS_MAX_MESHES], MatPos[TDS_MAX_MESHES], LightPos[TDS_MAX_MESHES], CameraPos[TDS_MAX_MESHES];
	char MeshName[TDS_MAX_MESHES][20];
	int MeshNumber, MaterialNumber, LightNumber, CameraNumber;
	unsigned long FilePos, FileSize, ChunkSize;
	int DataOffset=-1, i, j;
	unsigned short Chunk, TmpShort;
	unsigned long Size, TempSize, SecCont=0;
	unsigned char TempByte[3];

	ZeroMemory(Model, sizeof(TDS_Model_t));

	if((stream=fopen(filename, "rb"))==NULL)
		return FALSE;

	fread(&Chunk, sizeof(unsigned short), 1, stream);
	fread(&FileSize, sizeof(unsigned long), 1, stream);

	if(Chunk!=TDS_MAGIC) 
	{
		fclose(stream);
		return FALSE;
	}

	FilePos=6;

	while(FilePos<FileSize&&++SecCont<200)
	{
		fseek(stream, FilePos, SEEK_SET);

		fread(&Chunk, sizeof(unsigned short), 1, stream);
		fread(&Size, sizeof(unsigned long), 1, stream);

		switch(Chunk)
		{
			case TDS_DATA:
				DataOffset=FilePos;
				break;

			default:
				break;
		}

		if(DataOffset!=-1)
			break;

		FilePos+=Size;
	}

	if(DataOffset==-1)
	{
		fclose(stream);
		return FALSE;
	}

	SecCont=0;
	FilePos=DataOffset+6;
	fseek(stream, FilePos, SEEK_SET);

	MeshNumber=0;
	MaterialNumber=0;
	LightNumber=0;
	CameraNumber=0;

	while(SecCont++<TDS_MAX_MESHES)
	{
		fread(&Chunk, sizeof(unsigned short), 1, stream);
		fread(&Size, sizeof(unsigned long), 1, stream);

		switch(Chunk)
		{
			case TDS_NAMED_OBJECT:
				if(MeshNumber>=TDS_MAX_MESHES)
					break;

				for(j=0;j<20;j++)
				{
					fread(&MeshName[MeshNumber][j], sizeof(char), 1, stream);

					if(MeshName[MeshNumber][j]==0)
						break;

					if(MeshName[MeshNumber][j]<'0'||MeshName[MeshNumber][j]>'z')
						MeshName[MeshNumber][j]='_';
				}

				if(MeshName[MeshNumber][0]>='0'&&MeshName[MeshNumber][0]<='9')
					MeshName[MeshNumber][0]='N';

				fread(&Chunk, sizeof(unsigned short), 1, stream);

				switch(Chunk)
				{
					case TDS_TRIANGLE_OBJECT:
						MeshPos[MeshNumber]=FilePos+7+j;
						MeshNumber++;
						break;

					case TDS_DIRECT_LIGHT:
						LightPos[LightNumber]=FilePos+7+j;
						LightNumber++;
						break;

					case TDS_CAMERA:
						CameraPos[CameraNumber]=FilePos+7+j;
						CameraNumber++;
						break;

					default:
						break;
				}
				break;

			case TDS_MATERIAL_ENTRY:
				MatPos[MaterialNumber]=FilePos;
				MaterialNumber++;
				break;

			default:
				break;
		}

		FilePos+=Size;

		if(FilePos>FileSize)
			break;

		fseek(stream, FilePos, SEEK_SET);
	}

	Model->NumMeshes=MeshNumber;
	Model->NumMaterials=MaterialNumber;
	Model->NumLights=LightNumber;

	Model->Mesh=(TDS_Mesh_t *)malloc(sizeof(TDS_Mesh_t)*Model->NumMeshes);

	if(!Model->Mesh)
	{
		fclose(stream);
		return FALSE;
	}

	for(i=0;i<Model->NumMeshes;i++)
	{
		FilePos=MeshPos[i];
		fseek(stream, FilePos, SEEK_SET);

		fread(&Chunk, sizeof(unsigned short), 1, stream);
		fread(&Size, sizeof(unsigned long), 1, stream);

		ChunkSize=FilePos+Size;
		FilePos+=6;

		while(1)
		{
			fread(&Chunk, sizeof(unsigned short), 1, stream);
			fread(&Size, sizeof(unsigned long), 1, stream);

			switch(Chunk)
			{
				case TDS_VERTEX_ARRAY:
					fread(&Model->Mesh[i].NumVertex, sizeof(unsigned short), 1, stream);
					
					Model->Mesh[i].Vertex=(float *)malloc(sizeof(float)*Model->Mesh[i].NumVertex*3);

					if(!Model->Mesh[i].Vertex)
					{
						TDS_Delete(Model);
						fclose(stream);
						return FALSE;
					}

					fread(Model->Mesh[i].Vertex, sizeof(float)*Model->Mesh[i].NumVertex*3, 1, stream);
					break;

				case TDS_FACE_ARRAY:
					fread(&Model->Mesh[i].NumFaces, sizeof(unsigned short), 1, stream);

					Model->Mesh[i].Face=(unsigned short *)malloc(sizeof(unsigned short)*Model->Mesh[i].NumFaces*3);

					if(!Model->Mesh[i].Face)
					{
						TDS_Delete(Model);
						fclose(stream);
						return FALSE;
					}

					for(j=0;j<Model->Mesh[i].NumFaces;j++)
					{
						fread(&Model->Mesh[i].Face[3*j], sizeof(unsigned short), 1, stream);
						fread(&Model->Mesh[i].Face[3*j+1], sizeof(unsigned short), 1, stream);
						fread(&Model->Mesh[i].Face[3*j+2], sizeof(unsigned short), 1, stream);
						fread(&TmpShort, sizeof(unsigned short), 1, stream);
					}

					fread(&Chunk, sizeof(unsigned short), 1, stream);
					fread(&TempSize, sizeof(unsigned long), 1, stream);

					if(Chunk==TDS_MATERIAL_GROUP)
					{
						for(j=0;j<16;j++)
						{
							fread(&Model->Mesh[i].Material[j], sizeof(char), 1, stream);

							if(Model->Mesh[i].Material[j]==0)
								break;

							if(Model->Mesh[i].Material[j]<'0'||Model->Mesh[i].Material[j]>'z')
								Model->Mesh[i].Material[j]='_';
						}
					}
					break;

				case TDS_UV_ARRAY:
					fread(&Model->Mesh[i].NumUV, sizeof(unsigned short), 1, stream);

					Model->Mesh[i].UV=(float *)malloc(sizeof(float)*Model->Mesh[i].NumUV*2);

					if(!Model->Mesh[i].UV)
					{
						TDS_Delete(Model);
						fclose(stream);
						return FALSE;
					}

					fread(Model->Mesh[i].UV, sizeof(float)*Model->Mesh[i].NumUV*2, 1, stream);
					break;

				default:
					break;
			}

			FilePos+=Size;

			if(FilePos>ChunkSize)
				break;

			fseek(stream, FilePos, SEEK_SET);
		}
	}

	Model->Material=(TDS_Material_t *)malloc(sizeof(TDS_Material_t)*Model->NumMaterials);

	if(!Model->Material)
	{
		TDS_Delete(Model);
		fclose(stream);
		return FALSE;
	}

	for(i=0;i<Model->NumMaterials;i++)
	{
		FilePos=MatPos[i];
		fseek(stream, FilePos, SEEK_SET);

		fread(&Chunk, sizeof(unsigned short), 1, stream);
		fread(&Size, sizeof(unsigned long), 1, stream);

		ChunkSize=FilePos+Size;
		FilePos+=6;

		while(1)
		{
			fread(&Chunk, sizeof(unsigned short), 1, stream);
			fread(&Size, sizeof(unsigned long), 1, stream);

			switch(Chunk)
			{
				case TDS_MATERIAL_NAME:
					for(j=0;j<16;j++)
					{
						fread(&Model->Material[i].MaterialName[j], sizeof(char), 1, stream);

						if(Model->Material[i].MaterialName[j]==0)
							break;

						if(Model->Material[i].MaterialName[j]<'0'||Model->Material[i].MaterialName[j]>'z')
							Model->Material[i].MaterialName[j]='_';
					}
					break;

				case TDS_MATERIAL_TEXTURE_MAP:
					fread(&Chunk, sizeof(unsigned short), 1, stream);

					if(Chunk==TDS_INTEGER_PERCENTAGE)
						fseek(stream, 12, SEEK_CUR);
					else
						fseek(stream, 20, SEEK_CUR);

					for(j=0;j<20;j++)
					{
						fread(&Model->Material[i].MaterialFile[j], sizeof(char), 1, stream);

						if(Model->Material[i].MaterialFile[j]==0)
							break;
					}
					break;

				case TDS_MATERIAL_AMBIENT:
					fread(&Chunk, sizeof(unsigned short), 1, stream);
					fread(&TempSize, sizeof(unsigned long), 1, stream);

					if(Chunk==TDS_COLOR_24BIT) 
					{
						fread(&TempByte[0], sizeof(unsigned char)*3, 1, stream);

						Model->Material[i].MaterialAmbient[0]=(float)TempByte[0]*OneOver255;
						Model->Material[i].MaterialAmbient[1]=(float)TempByte[1]*OneOver255;
						Model->Material[i].MaterialAmbient[2]=(float)TempByte[2]*OneOver255;
					}
					else
						if(Chunk==TDS_COLOR_FLOAT)
							fread(&Model->Material[i].MaterialAmbient[0], sizeof(float)*3, 1, stream);
					break;

				case TDS_MATERIAL_DIFFUSE:
					fread(&Chunk, sizeof(unsigned short), 1, stream);
					fread(&TempSize, sizeof(unsigned long), 1, stream);

					if(Chunk==TDS_COLOR_24BIT)
					{
						fread(&TempByte[0], sizeof(unsigned char)*3, 1, stream);

						Model->Material[i].MaterialDiffuse[0]=(float)TempByte[0]*OneOver255;
						Model->Material[i].MaterialDiffuse[1]=(float)TempByte[1]*OneOver255;
						Model->Material[i].MaterialDiffuse[2]=(float)TempByte[2]*OneOver255;
					}
					else
						if(Chunk==TDS_COLOR_FLOAT)
							fread(&Model->Material[i].MaterialDiffuse[0], sizeof(float)*3, 1, stream);
					break;

				case TDS_MATERIAL_SPECULAR:
					fread(&Chunk, sizeof(unsigned short), 1, stream);
					fread(&TempSize, sizeof(unsigned long), 1, stream);

					if(Chunk==TDS_COLOR_24BIT)
					{
						fread(&TempByte[0], sizeof(unsigned char)*3, 1, stream);

						Model->Material[i].MaterialSpecular[0]=(float)TempByte[0]*OneOver255;
						Model->Material[i].MaterialSpecular[1]=(float)TempByte[1]*OneOver255;
						Model->Material[i].MaterialSpecular[2]=(float)TempByte[2]*OneOver255;
					}
					else
						if(Chunk==TDS_COLOR_FLOAT)
							fread(&Model->Material[i].MaterialSpecular[0], sizeof(float)*3, 1, stream);
					break;

				default:
					break;
			}

			FilePos+=Size;

			if(FilePos>ChunkSize)
				break;

			fseek(stream, FilePos, SEEK_SET);
		}
	}

	if(Model->NumLights)
	{
		Model->Light=(TDS_Light_t *)malloc(sizeof(TDS_Light_t)*Model->NumLights);

		if(!Model->Light)
		{
			TDS_Delete(Model);
			fclose(stream);
			return FALSE;
		}

		for(i=0;i<Model->NumLights;i++)
		{
			FilePos=LightPos[i];
			fseek(stream, FilePos, SEEK_SET);

			fread(&Chunk, sizeof(unsigned short), 1, stream);
			fread(&Size, sizeof(unsigned long), 1, stream);

			fread(&Model->Light[i].Position[0], sizeof(float), 1, stream);
			fread(&Model->Light[i].Position[2], sizeof(float), 1, stream);
			fread(&Model->Light[i].Position[1], sizeof(float), 1, stream);

			fread(&Chunk, sizeof(unsigned short), 1, stream);
			fread(&TempSize, sizeof(unsigned long), 1, stream);

			if(Chunk==TDS_COLOR_24BIT)
			{
				fread(&TempByte[0], sizeof(unsigned char)*3, 1, stream);

				Model->Light[i].Color[0]=(float)TempByte[0]*OneOver255;
				Model->Light[i].Color[1]=(float)TempByte[1]*OneOver255;
				Model->Light[i].Color[2]=(float)TempByte[2]*OneOver255;
			}
			else
				if(Chunk==TDS_COLOR_FLOAT)
					fread(&Model->Light[i].Color[0], sizeof(float)*3, 1, stream);
		}
	}

	for(i=0;i<Model->NumMeshes;i++)
	{
		strcpy(Model->Mesh[i].Name, MeshName[i]);

		if(Model->Mesh[i].Material)
		{
			for(j=0;j<Model->NumMaterials;j++)
			{
				if(strcmp(Model->Mesh[i].Material, Model->Material[j].MaterialName)==0)
					Model->Mesh[i].MaterialNumber=j;
			}
		}
	}

	fclose(stream);

	for(i=0;i<Model->NumMeshes;i++)
	{
		CalculateNormals(&Model->Mesh[i]);
		CalculateTangents(&Model->Mesh[i]);

		Model->Mesh[i].Minimum[0]=Model->Mesh[i].Maximum[0]=Model->Mesh[i].Vertex[0];
		Model->Mesh[i].Minimum[1]=Model->Mesh[i].Maximum[1]=Model->Mesh[i].Vertex[1];
		Model->Mesh[i].Minimum[2]=Model->Mesh[i].Maximum[2]=Model->Mesh[i].Vertex[2];

		for(j=0;j<Model->Mesh[i].NumVertex*3;j+=3)
		{
			if(Model->Mesh[i].Vertex[j]<Model->Mesh[i].Minimum[0])
				Model->Mesh[i].Minimum[0]=Model->Mesh[i].Vertex[j];

			if(Model->Mesh[i].Vertex[j+1]<Model->Mesh[i].Minimum[1])
				Model->Mesh[i].Minimum[1]=Model->Mesh[i].Vertex[j+1];

			if(Model->Mesh[i].Vertex[j+2]<Model->Mesh[i].Minimum[2])
				Model->Mesh[i].Minimum[2]=Model->Mesh[i].Vertex[j+2];

			if(Model->Mesh[i].Vertex[j]>Model->Mesh[i].Maximum[0])
				Model->Mesh[i].Maximum[0]=Model->Mesh[i].Vertex[j];

			if(Model->Mesh[i].Vertex[j+1]>Model->Mesh[i].Maximum[1])
				Model->Mesh[i].Maximum[1]=Model->Mesh[i].Vertex[j+1];

			if(Model->Mesh[i].Vertex[j+2]>Model->Mesh[i].Maximum[2])
				Model->Mesh[i].Maximum[2]=Model->Mesh[i].Vertex[j+2];
		}

		Model->Mesh[i].Center[0]=(Model->Mesh[i].Minimum[0]+Model->Mesh[i].Maximum[0])*0.5f;
		Model->Mesh[i].Center[1]=(Model->Mesh[i].Minimum[1]+Model->Mesh[i].Maximum[1])*0.5f;
		Model->Mesh[i].Center[2]=(Model->Mesh[i].Minimum[2]+Model->Mesh[i].Maximum[2])*0.5f;
	}

	return TRUE;
}

void TDS_Delete(TDS_Model_t *Model)
{
	int i;

	if(Model->Mesh)
	{
		for(i=0;i<Model->NumMeshes;i++)
		{
			FREE(Model->Mesh[i].Vertex);
			FREE(Model->Mesh[i].Face);
			FREE(Model->Mesh[i].UV);
			FREE(Model->Mesh[i].Tangent);
			FREE(Model->Mesh[i].Binormal);
			FREE(Model->Mesh[i].Normal);

			Model->Mesh[i].NumVertex=0;
			Model->Mesh[i].NumUV=0;
			Model->Mesh[i].NumFaces=0;
		}
	}

	FREE(Model->Mesh);
	FREE(Model->Light);
	FREE(Model->Material);

	Model->NumMeshes=0;
	Model->NumMaterials=0;
	Model->NumLights=0;
}

void TDS_Render(TDS_Model_t *Model)
{
	int i;

	glColor3f(1.0f, 1.0f, 1.0f);

	for(i=0;i<Model->NumMeshes;i++)
	{
		glVertexPointer(3, GL_FLOAT, 0, Model->Mesh[i].Vertex);
		glEnableClientState(GL_VERTEX_ARRAY);

		glNormalPointer(GL_FLOAT, 0, Model->Mesh[i].Normal);
		glEnableClientState(GL_NORMAL_ARRAY);

		if(Model->Mesh[i].NumUV)
		{
			glTexCoordPointer(2, GL_FLOAT, 0, Model->Mesh[i].UV);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		glDrawElements(GL_TRIANGLES, Model->Mesh[i].NumFaces*3, GL_UNSIGNED_SHORT, Model->Mesh[i].Face);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}
