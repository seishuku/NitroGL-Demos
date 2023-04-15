typedef struct
{
	unsigned int vindices[3], tindices[3];
} OBJ_Triangle_t;

typedef struct OBJ_Group_s
{
	char *name;
	unsigned int numtriangles;
	unsigned int *triangles;
	struct OBJ_Group_s *next;
} OBJ_Group_t;

typedef struct
{
	unsigned int numvertices;
	float *vertices;
	float *tangents;
	float *binormals;
	unsigned int numnormals;
	float *normals;
	unsigned int numtexcoords;
	float *texcoords;
	unsigned int numtriangles;
	OBJ_Triangle_t *triangles;
	unsigned int numgroups;
	OBJ_Group_t *groups;
} OBJ_Model_t;

BOOL OBJ_LoadModel(OBJ_Model_t *Model, char *filename);
void OBJ_Draw(OBJ_Model_t *Model);
void OBJ_DrawVectors(OBJ_Model_t *Model);
