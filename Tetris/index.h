#ifndef __INDEX_H__
#define __INDEX_H__

typedef int (*sortFunc_t)(const void *, const void *);

typedef struct
{
	int count;
	int *indices;
	char *data;
	int dataSize;
	sortFunc_t sortFunc;
} index_t;

void indexArray(index_t *index, char *array, int size, int count, sortFunc_t sortFunc);
int indexFind(index_t *index, void *element, int *result);
void indexFree(index_t *index);

#endif
