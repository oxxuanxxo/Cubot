/*!
@file		NPC.c
@author		Daniel Chua Yee Chen (yeechendaniel.chua)
@course		CSD1400
@brief		Contains implementations for functions used initialising
			each NPC. NPC contains a dialogue box that stores the conversation
			and handles checking with the player to know when to display the
			conversation.

			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/


#ifndef _NPC_H
#define _NPC_H

#include "../Render/Renderer.h"
#include "../Dialogue/DialogueBox.h"
#include "Player.h"
#include "../Camera/Camera.h"

typedef struct NPC
{
	Renderer sprite;
	Renderer speechBubble;
	DialogueBox* dialogue;
	float range;

}NPC;

NPC* NPC_Create(CP_Vector pos, CP_Vector scale, IMAGE_TYPE image, const char* filePath, float range);
void NPC_Destroy(NPC* npc);
void NPC_Update(NPC* npc,Player* player);
void NPC_Render(NPC* npc, Camera* cam);


#endif