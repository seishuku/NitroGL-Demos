#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "obj.h"
#include "glati.h"

extern PFNGLVARIANTFVEXTPROC glVariantfvEXT;
extern unsigned int Tangent;

void tangent_basis(float v0[3], float v1[3], float v2[3], float t0[2], float t1[2], float t2[2], float *tangent)
{
	float crossproduct[3];
	float edge0[3]={ v1[0]-v0[0], t1[0]-t0[0], t1[1]-t0[1] };
	float edge1[3]={ v2[0]-v0[0], t2[0]-t0[0], t2[1]-t0[1] };

	crossproduct[0]=edge0[1]*edge1[2]-edge0[2]*edge1[1];
	crossproduct[1]=edge0[2]*edge1[0]-edge0[0]*edge1[2];
	crossproduct[2]=edge0[0]*edge1[1]-edge0[1]*edge1[0];

	tangent[0]=-crossproduct[1]*(1/crossproduct[0]);

	edge0[0]=v1[1]-v0[1];
	edge1[0]=v2[1]-v0[1];

	crossproduct[0]=edge0[1]*edge1[2]-edge0[2]*edge1[1];
	crossproduct[1]=edge0[2]*edge1[0]-edge0[0]*edge1[2];
	crossproduct[2]=edge0[0]*edge1[1]-edge0[1]*edge1[0];

	tangent[1]=-crossproduct[1]*(1/crossproduct[0]);

	edge0[0]=v1[2]-v0[2];
	edge1[0]=v2[2]-v0[2];

	crossproduct[0]=edge0[1]*edge1[2]-edge0[2]*edge1[1];
	crossproduct[1]=edge0[2]*edge1[0]-edge0[0]*edge1[2];
	crossproduct[2]=edge0[0]*edge1[1]-edge0[1]*edge1[0];

	tangent[2]=-crossproduct[1]*(1/crossproduct[0]);
}

OBJ_Group_t *OBJ_FindGroup(OBJ_Model_t *Model, char* name)
{
	OBJ_Group_t *group;

	group=Model->groups;

	while(group)
	{
		if(!strcmp(name, group->name))
			break;

		group=group->next;
	}

	return group;
}

OBJ_Group_t *OBJ_AddGroup(OBJ_Model_t *Model, char *name)
{
	OBJ_Group_t *group;

	group=OBJ_FindGroup(Model, name);

	if(!group)
	{
		group=malloc(sizeof(OBJ_Group_t));

		group->name=strdup(name);
		group->numtriangles=0;
		group->triangles=NULL;
		group->next=Model->groups;
		Model->groups=group;
		Model->numgroups++;
	}

	return group;
}

void OBJ_FirstPass(OBJ_Model_t *Model, FILE *stream)
{
	unsigned int numvertices, numnormals, numtexcoords, numtriangles, v, n, t;
	OBJ_Group_t *group;
	char buf[128];

	group=OBJ_AddGroup(Model, "default");

	numvertices=numnormals=numtexcoords=numtriangles=0;

	while(fscanf(stream, "%s", buf)!=EOF)
	{
		switch(buf[0])
		{
			case '#':
				fgets(buf, sizeof(buf), stream);
				break;

			case 'v':
				switch(buf[1])
				{
					case '\0':
						fgets(buf, sizeof(buf), stream);
						numvertices++;
						break;

					case 'n':
						fgets(buf, sizeof(buf), stream);
						numnormals++;
						break;

					case 't':
						fgets(buf, sizeof(buf), stream);
						numtexcoords++;
						break;

					default:
						break;
				}
				break;

				case 'g':
					fgets(buf, sizeof(buf), stream);
					sscanf(buf, "%s", buf);
					group=OBJ_AddGroup(Model, buf);
					break;

				case 'f':
					v=n=t=0;
					fscanf(stream, "%s", buf);

					if(strstr(buf, "//"))
					{
						sscanf(buf, "%d//%d", &v, &n);
						fscanf(stream, "%d//%d", &v, &n);
						fscanf(stream, "%d//%d", &v, &n);
						numtriangles++;
						group->numtriangles++;

						while(fscanf(stream, "%d//%d", &v, &n)>0)
						{
							numtriangles++;
							group->numtriangles++;
						}
					}
					else
						if(sscanf(buf, "%d/%d/%d", &v, &t, &n)==3)
						{
							fscanf(stream, "%d/%d/%d", &v, &t, &n);
							fscanf(stream, "%d/%d/%d", &v, &t, &n);
							numtriangles++;
							group->numtriangles++;

							while(fscanf(stream, "%d/%d/%d", &v, &t, &n)>0)
							{
								numtriangles++;
								group->numtriangles++;
							}
						}
						else
							if(sscanf(buf, "%d/%d", &v, &t)==2)
							{
								fscanf(stream, "%d/%d", &v, &t);
								fscanf(stream, "%d/%d", &v, &t);
								numtriangles++;
								group->numtriangles++;

								while(fscanf(stream, "%d/%d", &v, &t)>0)
								{
									numtriangles++;
									group->numtriangles++;
								}
							}
							else
							{
								fscanf(stream, "%d", &v);
								fscanf(stream, "%d", &v);
								numtriangles++;
								group->numtriangles++;

								while(fscanf(stream, "%d", &v)>0)
								{
									numtriangles++;
									group->numtriangles++;
								}
							}
					break;

				default:
					fgets(buf, sizeof(buf), stream);
					break;
		}
	}

	Model->numvertices=numvertices;
	Model->numnormals=numnormals;
	Model->numtexcoords=numtexcoords;
	Model->numtriangles=numtriangles;

	group=Model->groups;

	while(group)
	{
		group->triangles=malloc(sizeof(unsigned int)*group->numtriangles);
		group->numtriangles=0;
		group=group->next;
	}
}

void OBJ_SecondPass(OBJ_Model_t *Model, FILE *stream)
{
	unsigned int numvertices, numnormals, numtexcoords, numtriangles, v, n, t;
	float *vertices, *normals, *texcoords;
	OBJ_Group_t *group;
	char buf[128];

	vertices=Model->vertices;
	normals=Model->normals;
	texcoords=Model->texcoords;
	group=Model->groups;

	numvertices=numnormals=numtexcoords=1;
	numtriangles=0;

	while(fscanf(stream, "%s", buf)!=EOF)
	{
		switch(buf[0])
		{
			case '#':
				fgets(buf, sizeof(buf), stream);
				break;

			case 'v':
				switch(buf[1])
				{
					case '\0':
						fscanf(stream, "%f %f %f", &vertices[3*numvertices+0], &vertices[3*numvertices+1], &vertices[3*numvertices+2]);
						numvertices++;
						break;

					case 'n':
						fscanf(stream, "%f %f %f", &normals[3*numnormals+0], &normals[3*numnormals+1], &normals[3*numnormals+2]);
						numnormals++;
						break;

					case 't':
						fscanf(stream, "%f %f", &texcoords[2*numtexcoords+0], &texcoords[2*numtexcoords+1]);
						numtexcoords++;
						break;
				}
				break;

				case 'g':
					fgets(buf, sizeof(buf), stream);
					sscanf(buf, "%s", buf);
					group=OBJ_FindGroup(Model, buf);
					break;

				case 'f':
					v=n=t=0;
					fscanf(stream, "%s", buf);

					if(strstr(buf, "//"))
					{
						sscanf(buf, "%d//%d", &v, &n);
						Model->triangles[numtriangles].vindices[0]=v;
						fscanf(stream, "%d//%d", &v, &n);
						Model->triangles[numtriangles].vindices[1]=v;
						fscanf(stream, "%d//%d", &v, &n);
						Model->triangles[numtriangles].vindices[2]=v;
						group->triangles[group->numtriangles++]=numtriangles;
						numtriangles++;

						while(fscanf(stream, "%d//%d", &v, &n)>0)
						{
							Model->triangles[numtriangles].vindices[0]=Model->triangles[numtriangles-1].vindices[0];
							Model->triangles[numtriangles].vindices[1]=Model->triangles[numtriangles-1].vindices[2];
							Model->triangles[numtriangles].vindices[2]=v;
							group->triangles[group->numtriangles++]=numtriangles;
							numtriangles++;
						}
					}
					else
						if(sscanf(buf, "%d/%d/%d", &v, &t, &n)==3)
						{
							Model->triangles[numtriangles].vindices[0]=v;
							Model->triangles[numtriangles].tindices[0]=t;
							fscanf(stream, "%d/%d/%d", &v, &t, &n);
							Model->triangles[numtriangles].vindices[1]=v;
							Model->triangles[numtriangles].tindices[1]=t;
							fscanf(stream, "%d/%d/%d", &v, &t, &n);
							Model->triangles[numtriangles].vindices[2]=v;
							Model->triangles[numtriangles].tindices[2]=t;
							group->triangles[group->numtriangles++]=numtriangles;
							numtriangles++;

							while(fscanf(stream, "%d/%d/%d", &v, &t, &n)>0)
							{
								Model->triangles[numtriangles].vindices[0]=Model->triangles[numtriangles-1].vindices[0];
								Model->triangles[numtriangles].tindices[0]=Model->triangles[numtriangles-1].tindices[0];
								Model->triangles[numtriangles].vindices[1]=Model->triangles[numtriangles-1].vindices[2];
								Model->triangles[numtriangles].tindices[1]=Model->triangles[numtriangles-1].tindices[2];
								Model->triangles[numtriangles].vindices[2]=v;
								Model->triangles[numtriangles].tindices[2]=t;
								group->triangles[group->numtriangles++]=numtriangles;
								numtriangles++;
							}
						}
						else
							if(sscanf(buf, "%d/%d", &v, &t)==2)
							{
								Model->triangles[numtriangles].vindices[0]=v;
								Model->triangles[numtriangles].tindices[0]=t;
								fscanf(stream, "%d/%d", &v, &t);
								Model->triangles[numtriangles].vindices[1]=v;
								Model->triangles[numtriangles].tindices[1]=t;
								fscanf(stream, "%d/%d", &v, &t);
								Model->triangles[numtriangles].vindices[2]=v;
								Model->triangles[numtriangles].tindices[2]=t;
								group->triangles[group->numtriangles++]=numtriangles;
								numtriangles++;

								while(fscanf(stream, "%d/%d", &v, &t)>0)
								{
									Model->triangles[numtriangles].vindices[0]=Model->triangles[numtriangles-1].vindices[0];
									Model->triangles[numtriangles].tindices[0]=Model->triangles[numtriangles-1].tindices[0];
									Model->triangles[numtriangles].vindices[1]=Model->triangles[numtriangles-1].vindices[2];
									Model->triangles[numtriangles].tindices[1]=Model->triangles[numtriangles-1].tindices[2];
									Model->triangles[numtriangles].vindices[2]=v;
									Model->triangles[numtriangles].tindices[2]=t;
									group->triangles[group->numtriangles++]=numtriangles;
									numtriangles++;
								}
							}
							else
							{
								sscanf(buf, "%d", &v);
								Model->triangles[numtriangles].vindices[0]=v;
								fscanf(stream, "%d", &v);
								Model->triangles[numtriangles].vindices[1]=v;
								fscanf(stream, "%d", &v);
								Model->triangles[numtriangles].vindices[2]=v;
								group->triangles[group->numtriangles++]=numtriangles;
								numtriangles++;

								while(fscanf(stream, "%d", &v)>0)
								{
									Model->triangles[numtriangles].vindices[0]=Model->triangles[numtriangles-1].vindices[0];
									Model->triangles[numtriangles].vindices[1]=Model->triangles[numtriangles-1].vindices[2];
									Model->triangles[numtriangles].vindices[2]=v;
									group->triangles[group->numtriangles++]=numtriangles;
									numtriangles++;
								}
							}
					break;

				default:
					fgets(buf, sizeof(buf), stream);
					break;
		}
	}
}

void OBJ_CalculateNormals(OBJ_Model_t *Model)
{
	unsigned int i, j;
	OBJ_Group_t *group;

	group=Model->groups;

	for(i=0;i<=Model->numvertices;i++)
	{
		Model->normals[3*i]=0.0f;
		Model->normals[3*i+1]=0.0f;
		Model->normals[3*i+2]=0.0f;
	}

	while(group)
	{
		for(i=0;i<group->numtriangles;i++)
		{
			float n[3], v0[3], v1[3];

			v0[0]=Model->vertices[3*Model->triangles[group->triangles[i]].vindices[1]]-Model->vertices[3*Model->triangles[group->triangles[i]].vindices[0]];
			v0[1]=Model->vertices[3*Model->triangles[group->triangles[i]].vindices[1]+1]-Model->vertices[3*Model->triangles[group->triangles[i]].vindices[0]+1];
			v0[2]=Model->vertices[3*Model->triangles[group->triangles[i]].vindices[1]+2]-Model->vertices[3*Model->triangles[group->triangles[i]].vindices[0]+2];

			v1[0]=Model->vertices[3*Model->triangles[group->triangles[i]].vindices[2]]-Model->vertices[3*Model->triangles[group->triangles[i]].vindices[0]];
			v1[1]=Model->vertices[3*Model->triangles[group->triangles[i]].vindices[2]+1]-Model->vertices[3*Model->triangles[group->triangles[i]].vindices[0]+1];
			v1[2]=Model->vertices[3*Model->triangles[group->triangles[i]].vindices[2]+2]-Model->vertices[3*Model->triangles[group->triangles[i]].vindices[0]+2];

			n[0]=(v0[1]*v1[2]-v0[2]*v1[1]);
			n[1]=(v0[2]*v1[0]-v0[0]*v1[2]);
			n[2]=(v0[0]*v1[1]-v0[1]*v1[0]);

			for(j=0;j<3;j++)
			{
				Model->normals[3*Model->triangles[group->triangles[i]].vindices[j]]+=n[0];
				Model->normals[3*Model->triangles[group->triangles[i]].vindices[j]+1]+=n[1];
				Model->normals[3*Model->triangles[group->triangles[i]].vindices[j]+2]+=n[2];
			}
		}

		group=group->next;
	}

	for(i=0;i<=Model->numvertices;i++)
	{
		float mag;

		mag=1/(float)sqrt((Model->normals[3*i]*Model->normals[3*i])+(Model->normals[3*i+1]*Model->normals[3*i+1])+(Model->normals[3*i+2]*Model->normals[3*i+2]));
		Model->normals[3*i]*=mag;
		Model->normals[3*i+1]*=mag;
		Model->normals[3*i+2]*=mag;
	}
}

void OBJ_CalculateTangentBasis(OBJ_Model_t *Model)
{
	unsigned int i, j;
	OBJ_Group_t *group;

	group=Model->groups;

	for(i=0;i<=Model->numvertices;i++)
	{
		Model->tangents[3*i]=0.0f;
		Model->tangents[3*i+1]=0.0f;
		Model->tangents[3*i+2]=0.0f;
	}

	while(group)
	{
		for(i=0;i<group->numtriangles;i++)
		{
			float v0[3], v1[3], v2[3], t0[2], t1[2], t2[2], tangent[3];

			v0[0]=Model->vertices[3*Model->triangles[group->triangles[i]].vindices[0]];
			v0[1]=Model->vertices[3*Model->triangles[group->triangles[i]].vindices[0]+1];
			v0[2]=Model->vertices[3*Model->triangles[group->triangles[i]].vindices[0]+2];

			v1[0]=Model->vertices[3*Model->triangles[group->triangles[i]].vindices[1]];
			v1[1]=Model->vertices[3*Model->triangles[group->triangles[i]].vindices[1]+1];
			v1[2]=Model->vertices[3*Model->triangles[group->triangles[i]].vindices[1]+2];

			v2[0]=Model->vertices[3*Model->triangles[group->triangles[i]].vindices[2]];
			v2[1]=Model->vertices[3*Model->triangles[group->triangles[i]].vindices[2]+1];
			v2[2]=Model->vertices[3*Model->triangles[group->triangles[i]].vindices[2]+2];

			t0[0]=Model->texcoords[2*Model->triangles[group->triangles[i]].tindices[0]];
			t0[1]=Model->texcoords[2*Model->triangles[group->triangles[i]].tindices[0]+1];

			t1[0]=Model->texcoords[2*Model->triangles[group->triangles[i]].tindices[1]];
			t1[1]=Model->texcoords[2*Model->triangles[group->triangles[i]].tindices[1]+1];

			t2[0]=Model->texcoords[2*Model->triangles[group->triangles[i]].tindices[2]];
			t2[1]=Model->texcoords[2*Model->triangles[group->triangles[i]].tindices[2]+1];

			tangent_basis(v0, v1, v2, t0, t1, t2, tangent);

			for(j=0;j<3;j++)
			{
				Model->tangents[3*Model->triangles[group->triangles[i]].vindices[j]]+=tangent[0];
				Model->tangents[3*Model->triangles[group->triangles[i]].vindices[j]+1]+=tangent[1];
				Model->tangents[3*Model->triangles[group->triangles[i]].vindices[j]+2]+=tangent[2];
			}
		}

		group=group->next;
	}

	for(i=0;i<=Model->numvertices;i++)
	{
		float mag;

		mag=1/(float)sqrt((Model->tangents[3*i]*Model->tangents[3*i])+(Model->tangents[3*i+1]*Model->tangents[3*i+1])+(Model->tangents[3*i+2]*Model->tangents[3*i+2]));
		Model->tangents[3*i]*=mag;
		Model->tangents[3*i+1]*=mag;
		Model->tangents[3*i+2]*=mag;
	}
}

BOOL OBJ_LoadModel(OBJ_Model_t *Model, char *filename)
{
	FILE *stream;

	if((stream=fopen(filename, "rt"))==NULL)
		return FALSE;

    Model->numvertices=0;
    Model->vertices=NULL;
	Model->tangents=NULL;
	Model->binormals=NULL;
    Model->numnormals=0;
    Model->normals=NULL;
    Model->numtexcoords=0;
    Model->texcoords=NULL;
    Model->numtriangles=0;
    Model->triangles=NULL;
    Model->numgroups=0;
    Model->groups=NULL;

	OBJ_FirstPass(Model, stream);

	Model->triangles=malloc(sizeof(OBJ_Triangle_t)*Model->numtriangles);
	Model->vertices=malloc(sizeof(float)*3*(Model->numvertices+1));
	Model->normals=malloc(sizeof(float)*3*(Model->numvertices+1));

	if(Model->numtexcoords)
	{
		Model->texcoords=malloc(sizeof(float)*2*(Model->numtexcoords+1));

		Model->tangents=malloc(sizeof(float)*3*(Model->numvertices+1));
		Model->binormals=malloc(sizeof(float)*3*(Model->numvertices+1));
	}

	rewind(stream);

	OBJ_SecondPass(Model, stream);

	fclose(stream);

	OBJ_CalculateNormals(Model);

	if(Model->numtexcoords>0)
		OBJ_CalculateTangentBasis(Model);

	return TRUE;
}

void OBJ_Draw(OBJ_Model_t *Model)
{
	unsigned int i, j;
	OBJ_Group_t *group;

	group=Model->groups;

	while(group)
	{
		glBegin(GL_TRIANGLES);

		for(i=0;i<group->numtriangles;i++)
		{
			for(j=0;j<3;j++)
			{
				if(Model->numtexcoords>0)
				{
					glTexCoord2fv(&Model->texcoords[2*Model->triangles[group->triangles[i]].tindices[j]]);
					glVariantfvEXT(Tangent, &Model->tangents[3*Model->triangles[group->triangles[i]].vindices[j]]);
				}

				glNormal3fv(&Model->normals[3*Model->triangles[group->triangles[i]].vindices[j]]);
				glVertex3fv(&Model->vertices[3*Model->triangles[group->triangles[i]].vindices[j]]);
			}
		}

		glEnd();

		group=group->next;
	}
}

void OBJ_DrawVectors(OBJ_Model_t *Model)
{
	unsigned int i, j;
	OBJ_Group_t *group;

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_LIGHTING);
	
	group=Model->groups;

	while(group)
	{
		glBegin(GL_LINES);

		for(i=0;i<group->numtriangles;i++)
		{
			for(j=0;j<3;j++)
			{
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(Model->vertices[3*Model->triangles[group->triangles[i]].vindices[j]], Model->vertices[3*Model->triangles[group->triangles[i]].vindices[j]+1], Model->vertices[3*Model->triangles[group->triangles[i]].vindices[j]+2]);
				glVertex3f(Model->vertices[3*Model->triangles[group->triangles[i]].vindices[j]]+Model->normals[3*Model->triangles[group->triangles[i]].vindices[j]]*4, Model->vertices[3*Model->triangles[group->triangles[i]].vindices[j]+1]+Model->normals[3*Model->triangles[group->triangles[i]].vindices[j]+1]*4, Model->vertices[3*Model->triangles[group->triangles[i]].vindices[j]+2]+Model->normals[3*Model->triangles[group->triangles[i]].vindices[j]+2]*4);

				if(Model->numtexcoords>0)
				{
					glColor3f(0.0f, 1.0f, 0.0f);
					glVertex3f(Model->vertices[3*Model->triangles[group->triangles[i]].vindices[j]], Model->vertices[3*Model->triangles[group->triangles[i]].vindices[j]+1], Model->vertices[3*Model->triangles[group->triangles[i]].vindices[j]+2]);
					glVertex3f(Model->vertices[3*Model->triangles[group->triangles[i]].vindices[j]]+Model->tangents[3*Model->triangles[group->triangles[i]].vindices[j]]*4, Model->vertices[3*Model->triangles[group->triangles[i]].vindices[j]+1]+Model->tangents[3*Model->triangles[group->triangles[i]].vindices[j]+1]*4, Model->vertices[3*Model->triangles[group->triangles[i]].vindices[j]+2]+Model->tangents[3*Model->triangles[group->triangles[i]].vindices[j]+2]*4);
				}

				glColor3f(1.0f, 1.0f, 1.0f);
			}
		}

		glEnd();

		group=group->next;
	}

	glPopAttrib();
}
