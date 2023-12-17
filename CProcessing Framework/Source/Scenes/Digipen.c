/*!
@file       Digipen.c
@author     Chua Yip Xuan (yipxuan.chua)
@course     CSD1400
@section    PASCAL-AM
@brief		Digipen logo will appear for 2.5 seconds before transiting to main menu or other scenes

			Copyright © 2020 DigiPen, All rights reserved.
*/
/*__________________________________________________________________________*/

#include "Digipen.h"
#include "MainMenu.h"
#include "Transition.h"
#include "MapEditor.h"
#include "WinScene.h"
#include "Credits.h"

#define LOGOHEIGHT 249
#define LOGOWIDTH 1026

//initialise the background and load digipen logo
void Digipen_Init(void)
{
	CP_Settings_Background(CP_Color_Create(255, 255, 255, 255));

	WINWIDTH = (float)CP_System_GetWindowWidth();
	WINHEIGHT = (float)CP_System_GetWindowHeight();

	logo = CP_Image_Load("././Images/DigiPen_BLACK.png");
	Transit_Create(&transit);
}

//draw logo and transit to main menu
void Digipen_Update(void)
{
	CP_System_SetWindowTitle("CUBOT");

	logoTimer += CP_System_GetDt();

	float temp = CP_System_GetDt();

	CP_Image_Draw(logo, WINWIDTH * 0.5f, WINHEIGHT * 0.5f, LOGOWIDTH, LOGOHEIGHT, 255);

	Transit_UpdateToBig(&transit,temp);

	if (logoTimer >= 2.5f)
	{
		Transit_NextScene(&transit, MainMenu_Init, MainMenu_Update, MainMenu_Exit);
	}
	if (logoTimer >= 3.5f)
	{
		//change scene here
		CP_Engine_SetNextGameState(MainMenu_Init, MainMenu_Update, MainMenu_Exit);
		//CP_Engine_SetNextGameState(Credits_Init, Credits_Update, Credits_Exit);
		//CP_Engine_SetNextGameState(MapEditor_Init, MapEditor_Update, MapEditor_Exit);
		//CP_Engine_SetNextGameState(WinScene_Init, WinScene_Update, WinScene_Exit);
		//CP_Engine_SetNextGameState(HowToPlay_Init, HowToPlay_Update, HowToPlay_Exit);
		//CP_Engine_SetNextGameState(TestScene_Init, TestScene_Update, TestScene_Exit);
	}
}

//free images
void Digipen_Exit(void)
{
	CP_Image_Free(&logo);
}