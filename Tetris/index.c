#include <malloc.h>
#include <string.h>
#include "index.h"

int indexAlloc(index_t *index)
{
	if(index->indices==NULL)
		index->indices=(int *)malloc(sizeof(int));
	else
		index->indices=(int *)realloc(index->indices, sizeof(int)*(index->count+1));

	if(index->indices==NULL)
		return -1;

	return 0;
}

int indexBinaryTraverse(void *value, void *data, int *indices, int stride, int count, int *result, int (*compare)(const void *, const void *))
{
	int high=count, low=0, mid, nextMid;

	mid=low+((high-low)>>1);
	*result=-1;

	while(low!=high)
	{
		*result=compare(value, (void *)((unsigned int)data+indices[mid]*stride));

		if(*result==0)
			break;
		else
		{
			if(*result<0)
				high=mid;
			else
				low=mid;
		}

		nextMid=low+((high-low)>>1);

		if(mid==nextMid)
			break;

		mid=nextMid;
	}

	return mid;
}

int indexSortedInsert(index_t *ind, int value)
{
	int position, result=0;

	position=indexBinaryTraverse((char *)ind->data+(value*ind->dataSize), ind->data, ind->indices, ind->dataSize, ind->count, &result, ind->sortFunc);

	if(indexAlloc(ind)==-1)
		return -1;

	if(result==0)
		return 0;
	else
	{
		if(result<0)
		{
			if(ind->count!=0)
				memmove(&ind->indices[position+1], &ind->indices[position], (ind->count-position)*sizeof(int));

			ind->indices[position]=value;
			ind->count++;

			return 0;
		}
		else
		{
			if((ind->count!=0)&&(position<(ind->count-1)))
				memmove(&ind->indices[position+2], &ind->indices[position+1], (ind->count-position-1)*sizeof(int));

			ind->indices[position+1]=value;
			ind->count++;

			return 0;
		}
	}
}

void indexArray(index_t *index, char *array, int size, int count, sortFunc_t sortFunc)
{
	int i;

	memset(index, 0, sizeof(index_t));
	index->data=array;
	index->dataSize=size;
	index->sortFunc=sortFunc;

	for(i=0;i<count;i++)
		indexSortedInsert(index, i);
}

int indexFind(index_t *index, void *element, int *result)
{
	return indexBinaryTraverse(element, index->data, index->indices, index->dataSize, index->count, result, index->sortFunc);
}

void indexFree(index_t *index)
{
	if(index->indices!=NULL)
		free(index->indices);

	memset(index, 0, sizeof(index_t));
}
