#ifndef _3DS_H_
#define _3DS_H_

typedef struct
{
	int e[2];
	int w[2];
} Winged_Edge_t;

typedef struct
{
	char Name[20];

	char Material[20];
	int MaterialNumber;

	float Center[3];
	float Minimum[3], Maximum[3];

	unsigned int VAOID;

	unsigned short NumFace;
	unsigned short *Face;

	unsigned short NumVertex;
	float *Vertex;
	float *Tangent;
	float *Binormal;
	float *Normal;

	unsigned short NumUV;
	float *UV;

	unsigned short NumEdge;
	Winged_Edge_t *Edge;

	float *Plane;
} TDS_Mesh_t;

typedef struct
{
	unsigned int TextureID;
	unsigned int BumpTextureID;
	char MaterialName[256];
	char MaterialFile[256];
	float MaterialAmbient[3];
	float MaterialDiffuse[3];
	float MaterialSpecular[3];
} TDS_Material_t;

typedef struct
{
	float Position[3];
	float Color[3];
} TDS_Light_t;

typedef struct
{
	float Position[3];
	float Target[3];
	float Roll;
	float FocalLength;
} TDS_Camera_t;

typedef struct
{
	int NumMeshes;
	TDS_Mesh_t *Mesh;

	int NumMaterials;
	TDS_Material_t *Material;

	int NumLights;
	TDS_Light_t *Light;

	int NumCameras;
	TDS_Camera_t *Camera;
} TDS_Model_t;

int TDS_LoadModel(TDS_Model_t *Model, char *filename);
void TDS_Delete(TDS_Model_t *Model);
void TDS_Render(TDS_Model_t *Model, BOOL Textured, unsigned int BaseID, unsigned int BumpID);
void TDS_RenderShadow(TDS_Model_t *Model, float light[3]);

#endif
