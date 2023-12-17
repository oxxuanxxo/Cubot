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

#include "NPC.h"
#include "../Render/RenderUtil.h"
#include <stdlib.h>


NPC* NPC_Create(CP_Vector pos, CP_Vector scale, IMAGE_TYPE image, const char* filePath, float range)
{
	NPC* result = (NPC*)malloc(sizeof(NPC));
	if (result)
	{
		result->sprite.position = pos;
		result->sprite.scale = scale;
		result->sprite.image = image;
		result->sprite.alpha = 255;
		result->sprite.rotation = 0.0f;

		result->speechBubble.position = CP_Vector_Add(pos,CP_Vector_Scale(scale,0.7f));
		result->speechBubble.scale = scale;
		result->speechBubble.image = IMAGE_SPEECH_BUBBLE;
		result->speechBubble.alpha = 255;
		result->speechBubble.rotation = 0.0f;


		result->dialogue = DialogueBox_LoadFromFile(filePath, 45.0f);
		result->range = range;
	}
	return result;
}

void NPC_Destroy(NPC* npc)
{
	DialogueBox_Destroy(npc->dialogue);
	free(npc);

}

void NPC_Update(NPC* npc,Player* player)
{
	if (npc->dialogue)
	{
		if (CP_Vector_Distance(npc->sprite.position, player->character->renderer.position) <= npc->range)
		{
			npc->dialogue->enabled = true;
			DialogueBox_Update(npc->dialogue);
		}
		else
		{
			npc->dialogue->enabled = false;
			npc->dialogue->index = 0;
			npc->dialogue->textIndex = 0;
		}
	}
	
}

void NPC_Render(NPC* npc, Camera* cam)
{
	Render_Image(cam, &(npc->sprite));
	if (npc->dialogue && npc->dialogue->enabled == true)
	{
		DialogueBox_Render(cam, npc->dialogue);
		Render_Image(cam, &(npc->speechBubble));
	}
}