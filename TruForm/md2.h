#ifndef __md2_h_
#define __md2_h_
#ifdef __cplusplus
extern "C" {
#endif

#define MD2_MAGIC ('I'+('D'<<8)+('P'<<16)+('2'<<24))
#define MD2_VERSION 8
#define MD2_HEADER_SIZE 68
#define MD2_MAX_TRIANGLES 4096
#define MD2_MAX_VERTICES 2048
#define MD2_MAX_TEXCOORDS 2048
#define MD2_MAX_FRAMES 512
#define MD2_MAX_SKINS 32
#define MD2_MAX_FRAMESIZE 8320
#define NUMVERTEXNORMALS 162

typedef struct
{
	int magic;
	int version;
	int skinWidth;
	int skinHeight;
	int frameSize;
	int numSkins;
	int numVertices;
	int numTexCoords;
	int numTriangles;
	int numGlCommands;
	int numFrames;
	int offsetSkins;
	int offsetTexCoords;
	int offsetTriangles;
	int offsetFrames;
	int offsetGlCommands;
	int offsetEnd;
} MD2_Header_t;

typedef struct
{
	unsigned char vertex[3];
	unsigned char lightNormalIndex;
} MD2_Alias_TriangleVertex_t;

typedef struct
{
	float vertex[3];
	float normal[3];
} MD2_TriangleVertex_t;

typedef struct
{
	short vertexIndices[3];
	short textureIndices[3];
} MD2_Triangle_t;

typedef struct
{
	short s;
	short t;
} MD2_TextureCoordinate_t;

typedef struct
{
	float scale[3];
	float translate[3];
	char name[16];
	MD2_Alias_TriangleVertex_t alias_vertices[1];
} MD2_Alias_Frame_t;

typedef struct
{
	char name[16];
	MD2_TriangleVertex_t *vertices;
} MD2_Frame_t;

typedef char MD2_Skin_t[64];

typedef struct
{
	float s;
	float t;
	int vertexIndex;
} MD2_GLCommandVertex_t;

typedef struct
{
	MD2_Header_t header;
	MD2_Skin_t *skins;
	MD2_TextureCoordinate_t *texcoords;
	MD2_Triangle_t *triangles;
	MD2_Frame_t *frames;
	int *glcommandbuffer;
} MD2_Model_t;

BOOL MD2_LoadModel(char *filename, MD2_Model_t *MD2);
void MD2_DrawFrame(MD2_Model_t *MD2, int Frame);
void MD2_DrawFramei(MD2_Model_t *MD2, int Frame1, int Frame2, float Pol);
void GenerateNormals(MD2_Model_t *MD2);

#ifdef __cplusplus
}
#endif
#endif