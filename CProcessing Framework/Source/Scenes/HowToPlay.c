//---------------------------------------------------------
// file:	HowToPlay.c
// author:	Wu Pei Fang(peifang.w)
// email:	peifang.w@digipen.edu
//
// brief:	These code create the instruction page scene when triggered the mainmenu HowTo button.
//			Instruction page will return to main menu page when space key is triggered.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "MainMenu.h"
#include "cprocessing.h"

void HowToPlay_Init()
{

}

void HowToPlay_Update()
{
	//load image to main menu
	CP_Image Instruction = CP_Image_Load("./Images/HowToPlay.png");
	//draw the main menu background
	float InstructionX, InstructionY;
	InstructionX = WINDOW_WIDTH / 2.0f;
	InstructionY = WINDOW_HEIGHT / 2.0f;
	CP_Image_Draw(Instruction, InstructionX, InstructionY, WINDOW_WIDTH, WINDOW_HEIGHT, 300);

	//when hit space bar, proceed to main menu scene
	if (CP_Input_KeyTriggered(KEY_SPACE))
	{
		CP_Engine_SetNextGameState(MainMenu_Init, MainMenu_Update, MainMenu_Exit);
	}
}

void HowToPlay_Exit()
{

}
