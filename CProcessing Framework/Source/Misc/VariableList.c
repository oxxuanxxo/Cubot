/*!
@file		VariableList.c
@author		Leong Jun Xiang	(junxiang.leong)
@course		CSD1400
@brief		a dynamic array which will expand when it reaches the maximium size
Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/

#include "VariableList.h"
#include <stdlib.h>
#define DEFAULT_LIST_SIZE 5
void VariableListCreate(VariableList* vl , int defaultSize)
{
	vl->length = 0;
	vl->arraySize = defaultSize;
	vl->list = malloc(defaultSize * sizeof(void*));
}
void VariableListAdd(VariableList* vl, void* object)
{
	if (vl->arraySize == 0)//creates the list if its not created
		VariableListCreate(vl, DEFAULT_LIST_SIZE);

	if (vl->length == vl->arraySize)
	{

		VariableListResize(vl);
	}
	//add object
	vl->list[vl->length++] = object;
	return;
}
void VariableListResize(VariableList* vl)
{
	vl->arraySize *= 2;
	size_t newsize = (size_t)vl->arraySize * sizeof(void*);
	void** list = realloc(vl->list, newsize);
	if (list)
	{
		vl->list = list;
	}
}
void VariableListFree(VariableList* vl)
{
	if (vl->list)
	{
		for(int i =0;i<vl->length;++i)
		{
			if(vl->list[i] == NULL)
				free(vl->list[i]);
			vl->list[i] = NULL;
		}
		free(vl->list);
	}
}
void* VariableListGetItemAt(VariableList* vl, int index)
{
	if (index < 0 && index >= vl->length)
		return NULL;
	else
		return vl->list[index];
}