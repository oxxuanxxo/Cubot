/*!
@file		VariableList.h
@author		Leong Jun Xiang	(junxiang.leong)
@course		CSD1400
@brief		a dynamic array which will expand when it reaches the maximium size
Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/
#ifndef VARIABLELIST_H
#define VARIABLELIST_H

typedef struct VariableList
{
	//amount of variables
	int length;
	//total size of the array
	int arraySize;
	void** list;
}VariableList;
//object lifetime should last until free is called (malloc created)
void VariableListCreate(VariableList* vl, int defaultSize);
//Appending to the list
void VariableListAdd(VariableList* vl ,void* object);
//resize the list(wont be called outside)
void VariableListResize(VariableList* vl);
//delete memory
void VariableListFree(VariableList* vl);
//0 is the first item vl->length -1 is the max length
void* VariableListGetItemAt(VariableList* vl, int index);
#endif // !VARIABLELIST_H

