/*!
@file		DialogueBox.c
@author		Daniel Chua Yee Chen (yeechendaniel.chua)
@course		CSD1400
@brief		Contains implementations for functions used to initialise
			a dialogue box and update the conversation accordingly.

			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/

#include "DialogueBox.h"
#include "cprocessing.h"
#include "../Render/RenderUtil.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

DialogueBox* DialogueBox_LoadFromFile(const char* filePath, float speed)
{
	DialogueBox* result = (DialogueBox*)malloc(sizeof(DialogueBox));

	if (result)
	{
		VariableListCreate( &(result->dialogueList), 0);

		int width = CP_System_GetWindowWidth();
		int height = CP_System_GetWindowHeight();

		result->index = 0;
		result->textIndex = 0;
		result->speed = speed;
		result->enabled = false;
		result->background.alpha = 255;
		result->background.image = IMAGE_DIALOGUE;
		result->background.rotation = 0.0f;
		result->background.position = CP_Vector_Set(width * 0.45f, 0);
		result->background.scale = CP_Vector_Set((float)width, height * 0.35f);

		result->speaker.alpha = 255;
		result->speaker.image = IMAGE_INFO;
		result->speaker.rotation = 0.0f;
		result->speaker.position = CP_Vector_Set(width * 0.1f, height * 0.025f);
		result->speaker.scale = CP_Vector_Set((float)width * 0.1f, height * 0.15f);

		bool b_isNewDialogue = true;
		Dialogue* ptr;
		size_t currentIndex = 0;

		FILE* file_in;
		fopen_s(&file_in,filePath, "r");
		if (file_in)
		{
			char buffer[5000];


			while (fgets(buffer, 5000, file_in) != NULL)
			{
				if (b_isNewDialogue)
				{
					ptr = (Dialogue*)malloc(sizeof(Dialogue));

					b_isNewDialogue = false;
					if (ptr)
					{
						size_t nameIndex = 0;
						for (nameIndex = 0; nameIndex < strlen(buffer); ++nameIndex)
						{
							if (buffer[nameIndex] == ':')
							{
								break;
							}
						}
						//NPC: , nameIndex = 3
						// nameIndex = length of my name.


						ptr->name = (char*)malloc(sizeof(char) * (nameIndex + 1));


						if (ptr->name)
						{
							//copy name from buffer
							for (size_t i = 0; i < nameIndex; ++i)
							{
								ptr->name[i] = buffer[i];
							}
							//adding null terminator at the back.
							ptr->name[nameIndex] = '\0';
							
						}

						ptr->text = (char*)malloc(sizeof(char) * (strlen(buffer) - nameIndex));
						if (ptr->text)
						{
							// buffer contains NPC: hi how is your day@
							// loop to copy into text
							for (size_t i = 0; i < (strlen(buffer) - nameIndex - 1); ++i)
							{
								ptr->text[i] = buffer[i + nameIndex + 1];
							}
							// add the null terminator to the end of the text.
							ptr->text[(strlen(buffer) - nameIndex - 1)] = '\0';

							size_t length = strlen(ptr->text);
							
							if (ptr->text[length - 2] == '@')
							{
								ptr->text[length - 2] = '\0';
								b_isNewDialogue = true;
								++currentIndex;

							}

						}

						VariableListAdd(&(result->dialogueList), ptr);
					}
				}
				else
				{
					ptr = VariableListGetItemAt(&(result->dialogueList), (int)currentIndex);
					//ptr = result->dialogueList[currentIndex];

					//hi how 
					//need to append is your day@
					size_t oldSize = strlen(ptr->text);
					size_t bufferSize = strlen(buffer);

					ptr->text = (char*)realloc(ptr->text,oldSize + bufferSize + 1);
					if (ptr->text)
					{
						for (size_t i = 0; i < bufferSize; ++i)
						{
							ptr->text[i + oldSize] = buffer[i];
						}
						//ptr->text[oldSize - 1] = '\n';
						ptr->text[oldSize + bufferSize] = '\0';

						size_t length = strlen(ptr->text);

						if (ptr->text[length - 2] == '@')
						{
							ptr->text[length - 2] = '\0';
							b_isNewDialogue = true;
							++currentIndex;

						}
					}
				}
			}

			fclose(file_in);
		}
	}
	


	return result;
}


static void DialogueBox_SwitchSpeaker(DialogueBox* dbox, const char* speakerName)
{
	if (strcmp(speakerName, "INFO") == 0)
	{
		dbox->speaker.image = IMAGE_INFO;
	}
	if (strcmp(speakerName, "CUBOT") == 0)
	{
		dbox->speaker.image = IMAGE_PLAYER;
	}
	if (strcmp(speakerName, "CHICK") == 0)
	{
		dbox->speaker.image = IMAGE_CHICK;
	}
	if (strcmp(speakerName, "FOX") == 0)
	{
		dbox->speaker.image = IMAGE_FOX;
	}
	if (strcmp(speakerName, "PIG") == 0)
	{
		dbox->speaker.image = IMAGE_PIG;
	}
}

void DialogueBox_Update(DialogueBox* dbox)
{
	if (dbox->enabled == true)
	{
		dbox->textIndex += CP_System_GetDt() * dbox->speed;
		Dialogue* dialogue = VariableListGetItemAt(&(dbox->dialogueList), dbox->index);
		DialogueBox_SwitchSpeaker(dbox, dialogue->name);
		if (dbox->textIndex >= strlen(dialogue->text) + dbox->speed * 0.75f)
		{
			if (dbox->index < dbox->dialogueList.length - 1)
			{
				dbox->index++;
				dbox->textIndex = 0;
			}
		}

	}

}


void DialogueBox_Render(Camera* cam, DialogueBox* dbox)
{
	if (dbox->enabled == true)
	{
		int width = CP_System_GetWindowWidth();
		int height = CP_System_GetWindowHeight();

		dbox->background.position = CP_Vector_Subtract(cam->position, CP_Vector_Set(0, -height * 0.37f));
		dbox->speaker.position = CP_Vector_Subtract(cam->position, CP_Vector_Set(width * 0.4f, -height * 0.37f));


		Render_Image(cam, &(dbox->background));
		Render_Image(cam, &(dbox->speaker));


		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_BOTTOM);

		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
		CP_Settings_TextSize(40.0f);

		Dialogue* dialogue = VariableListGetItemAt(&(dbox->dialogueList), dbox->index);
		CP_Font_DrawText(dialogue->name, width * 0.2f, height * 0.08f);

		char buffer[5000];

		for (size_t i = 0; i < (size_t)dbox->textIndex; ++i)
		{
			buffer[i] = dialogue->text[i];
		}
		buffer[(size_t)dbox->textIndex] = '\0';

		CP_Font_DrawText(buffer, width * 0.2f, height * 0.15f);
	}
}

void Dialogue_Destroy(Dialogue* dialogue)
{
	free(dialogue->name);
	free(dialogue->text);
}

void DialogueBox_Destroy(DialogueBox* dBox)
{
	for (int i = 0; i < dBox->dialogueList.length; ++i)
	{
		Dialogue_Destroy(VariableListGetItemAt(&(dBox->dialogueList), i));
	}

	free(dBox);
}

