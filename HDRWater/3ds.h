#ifndef __3DS_H__
#define __3DS_H__

#pragma pack(push, 1)

typedef struct
{
	unsigned short ID;
	unsigned long Length;
} TDS_Chunk_t;

typedef struct
{
	char Name[32];
	char Texture[32];

	unsigned int BaseTexID;
	unsigned int BumpTexID;

	float Ambient[3];
	float Diffuse[3];
	float Specular[3];
	float Emission[3];
	float Shininess;
} TDS_Material_t;

typedef struct
{
	char Name[32];

	char MaterialName[32];
	unsigned short MaterialIndex;

	unsigned int VAOID;

	unsigned short NumVertex;
	float *Vertex;
	float *Tangent;
	float *Binormal;
	float *Normal;

	unsigned short NumUV;
	float *UV;

	unsigned short NumFace;
	unsigned short *Face;

	float Axis[9];
	float Position[3];

	float Min[3], Max[3];
	float Center[3];
} TDS_Mesh_t;

typedef struct
{
	int NumMaterial;
	TDS_Material_t *Material;

	int NumMesh;
	TDS_Mesh_t *Mesh;

	float Min[3], Max[3];
	float Center[3];
} TDS_Model_t;

#pragma pack(pop)

int TDS_LoadModel(TDS_Model_t *Model, char *filename);
void TDS_Free(TDS_Model_t *Model);

#endif
