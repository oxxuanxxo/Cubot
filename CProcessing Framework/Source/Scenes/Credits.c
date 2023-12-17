/*!
@file       Credits.c
@author     Chua Yip Xuan (yipxuan.chua)
@course     CSD1400
@section    PASCAL-AM
@brief      This will show credits screen when credit button is selected in main menu

			Copyright © 2020 DigiPen, All rights reserved.
*/
/*__________________________________________________________________________*/

#include "Credits.h"
#include "MainMenu.h"

//set initials
void Credits_Init(void)
{
	WINWIDTH = (float)CP_System_GetWindowWidth();
	WINHEIGHT = (float)CP_System_GetWindowHeight();

	credits1 = CP_Image_Load("././Images/Credits/Credits1.png");
	credits2 = CP_Image_Load("././Images/Credits/Credits2.png");
	credits3 = CP_Image_Load("././Images/Credits/Credits3.png");
	credits4 = CP_Image_Load("././Images/Credits/Credits4.png");
	credits5 = CP_Image_Load("././Images/Credits/Credits5.png");
	credits6 = CP_Image_Load("././Images/Credits/Credits6.png");

	Transit_Create(&trans);
	creditsMusic = CP_Sound_LoadMusic("./Assets/MenuMusic.mp3");
	CP_Sound_PlayAdvanced(creditsMusic, 0.8f, 1.2f, 1, CP_SOUND_GROUP_9);

}

//draw credits pages after certain time passed
void Credits_Update(void)
{
	deltaT += CP_System_GetDt();

	CP_Image_Draw(credits1, WINWIDTH * 0.5f, WINHEIGHT * 0.5f, WINWIDTH, WINHEIGHT, 255);

	if (deltaT >= 2.5f)
	{
		CP_Image_Draw(credits2, WINWIDTH * 0.5f, WINHEIGHT * 0.5f, WINWIDTH, WINHEIGHT, 255);
	}
	if (deltaT >= 5.5f)
	{
		CP_Image_Draw(credits3, WINWIDTH * 0.5f, WINHEIGHT * 0.5f, WINWIDTH, WINHEIGHT, 255);
	}
	if (deltaT >= 9.0f)
	{
		CP_Sound_SetGroupVolume(CP_SOUND_GROUP_9, 0.6f);
		CP_Image_Draw(credits4, WINWIDTH * 0.5f, WINHEIGHT * 0.5f, WINWIDTH, WINHEIGHT, 255);
	}
	if (deltaT >= 12.5f)
	{
		CP_Sound_SetGroupVolume(CP_SOUND_GROUP_9, 0.5f);
		CP_Image_Draw(credits5, WINWIDTH * 0.5f, WINHEIGHT * 0.5f, WINWIDTH, WINHEIGHT, 255);
	}
	if (deltaT >= 15.0f)
	{
		CP_Sound_SetGroupVolume(CP_SOUND_GROUP_9, 0.3f);
		CP_Image_Draw(credits6, WINWIDTH * 0.5f, WINHEIGHT * 0.5f, WINWIDTH, WINHEIGHT, 255);
	}
	
	float temp = CP_System_GetDt();
	Transit_UpdateToBig(&trans, temp);

	if (deltaT >= 16.5f)
	{
		CP_Sound_SetGroupVolume(CP_SOUND_GROUP_9, 0.2f);
		Transit_NextScene(&trans, MainMenu_Init, MainMenu_Update, MainMenu_Exit);
		if (deltaT >= 17.5f)
		{
			CP_Sound_SetGroupVolume(CP_SOUND_GROUP_9, 0.0f);
			CP_Engine_SetNextGameState(MainMenu_Init, MainMenu_Update, MainMenu_Exit);
			deltaT = 0.0f;
		}
	}
	
}

//free images and sound
void Credits_Exit(void)
{
	CP_Image_Free(&credits1);
	CP_Image_Free(&credits2);
	CP_Image_Free(&credits3);
	CP_Image_Free(&credits4);
	CP_Image_Free(&credits5);
	CP_Image_Free(&credits6);
	CP_Sound_Free(creditsMusic);
}