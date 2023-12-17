/*!
@file		DialogueBox.h
@author		Daniel Chua Yee Chen (yeechendaniel.chua)
@course		CSD1400
@brief		Contains implementations for functions used to initialise
			a dialogue box and update the conversation accordingly.

			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/

#ifndef _DIALOGUE_BOX_H
#define _DIALOGUE_BOX_H

#include "../Misc/VariableList.h"
#include "../GlobalDefine.h"
#include "../Camera/Camera.h"
#include "../Render/Renderer.h"
#include <stdbool.h>

typedef struct Dialogue
{
	char* text;
	char* name;
	
}Dialogue;


typedef struct DialogueBox
{
	VariableList dialogueList;
	int index;
	//how many character of the current text to render.
	float textIndex;
	float speed;
	bool enabled;
	Renderer background;
	Renderer speaker;

} DialogueBox;


DialogueBox* DialogueBox_LoadFromFile(const char* filePath, float speed);
void DialogueBox_Update(DialogueBox* dbox);

void DialogueBox_Render(Camera* cam,DialogueBox* dbox);

void Dialogue_Destroy(Dialogue* dialogue);
void DialogueBox_Destroy(DialogueBox* dBox);


#endif