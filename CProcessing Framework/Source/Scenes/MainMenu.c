//---------------------------------------------------------
// file:	MainMenu.c
// author:	Wu Pei Fang(peifang.w)
// email:	peifang.w@digipen.edu
//
// brief:	These code create the button in main menu page.
//			When hitting the space key for the buttons, they will toggle to the respective scene.
//			
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "MainMenu.h"
#include "cprocessing.h"
#include "TestScene.h"
#include "HowToPlay.h"
#include "MapEditor.h"
#include "TutorialScene.h"
#include "../Sound/Sound.h"
#include "Transition.h"
#include "MapEditor.h"
#include "Credits.h"
#include "WinScene.h"

void MainMenu_Init()
{
	//structure start button
	struct Button StartB =
	{
		.x = WINDOW_WIDTH / 2,
		.y = WINDOW_HEIGHT * 6 / 12,
		.width = 250,
		.height = WINDOW_HEIGHT / 10,
		.btnImage = CP_Image_Load("./Images/Start.png"),
		.btnHoverImage = CP_Image_Load("./Images/SelectedStart.png"),
		.LCursorImage = CP_Image_Load("./Images/LeftCursor.png"),
	};
	Start = StartB;

	//structure HowTo button
	struct Button HowToB =
	{
		.x = WINDOW_WIDTH / 2,
		.y = WINDOW_HEIGHT * 7 / 12,
		.width = 250,
		.height = WINDOW_HEIGHT / 10,
		.btnImage = CP_Image_Load("./Images/HowTo.png"),
		.btnHoverImage = CP_Image_Load("./Images/SelectedHowTo.png"),
		.LCursorImage = CP_Image_Load("./Images/LeftCursor.png"),
	};
	HowTo = HowToB;

	//structure quite button
	struct Button QuitB =
	{
		.x = WINDOW_WIDTH / 2,
		.y = WINDOW_HEIGHT * 11 / 12,
		.width = 250,
		.height = WINDOW_HEIGHT / 10,
		.btnImage = CP_Image_Load("./Images/Quit.png"),
		.btnHoverImage = CP_Image_Load("./Images/SelectedQuit.png"),
		.LCursorImage = CP_Image_Load("./Images/LeftCursor.png"),
	};
	Quit = QuitB;

	//structure mapEditor button
	MapEditor.x = WINDOW_WIDTH / 2;
	MapEditor.y = WINDOW_HEIGHT * 8 / 12;
	MapEditor.width = 300;
	MapEditor.height = WINDOW_HEIGHT / 10;
	MapEditor.btnImage = CP_Image_Load("./Images/MapEditor.png");
	MapEditor.btnHoverImage = CP_Image_Load("./Images/MapEditorSelected.png");
	MapEditor.LCursorImage = CP_Image_Load("./Images/LeftCursor.png");

	//structure LoadLevel button
	LoadLevel.x = WINDOW_WIDTH / 2;
	LoadLevel.y = WINDOW_HEIGHT * 9 / 12;
	LoadLevel.width = 250;
	LoadLevel.height = WINDOW_HEIGHT / 10;
	LoadLevel.btnImage = CP_Image_Load("./Images/Load.png");
	LoadLevel.btnHoverImage = CP_Image_Load("./Images/SelectedLoad.png");
	LoadLevel.LCursorImage = CP_Image_Load("./Images/LeftCursor.png");

	//structure credit button
	Credits.x = WINDOW_WIDTH / 2;
	Credits.y = WINDOW_HEIGHT * 10 / 12;
	Credits.width = 250;
	Credits.height = WINDOW_HEIGHT / 10;
	Credits.btnImage = CP_Image_Load("./Images/Credits.png");
	Credits.btnHoverImage = CP_Image_Load("./Images/SelectedCredits.png");
	Credits.LCursorImage = CP_Image_Load("./Images/LeftCursor.png");


	currentState = BUTTON_STARTSTATE; //Default at Start button

	SoundLoad();

	Transit_Create(&transition);
	transition.scale = 1;
	transition.isTransitingToBig = true;
	setSpeed(&transition, -1);

	mainMenuMusic = CP_Sound_LoadMusic("./Assets/MenuMusic.mp3");

	CP_Sound_PlayAdvanced(mainMenuMusic, 0.4f, 1.2f, 1, CP_SOUND_GROUP_2);

}

//draw image of button
void MainMenu_Render(void)
{

	if (currentState == BUTTON_STARTSTATE)
	{
		CP_Image_Draw(Start.btnHoverImage, Start.x, Start.y, Start.width, Start.height, 300);
		CP_Image_Draw(Start.LCursorImage, Start.x - Start.x * 0.22f, Start.y, CURSOR_WIDTH, CURSOR_HEIGHT, 300);

	}
	else
	{
		CP_Image_Draw(Start.btnImage, Start.x, Start.y, Start.width, Start.height, 300);
	}

	if (currentState == BUTTON_HOWTOSTATE)
	{
		CP_Image_Draw(HowTo.btnHoverImage, HowTo.x, HowTo.y, HowTo.width, HowTo.height, 300);
		CP_Image_Draw(HowTo.LCursorImage, HowTo.x - HowTo.x * 0.22f, HowTo.y, CURSOR_WIDTH, CURSOR_HEIGHT, 300);
	}
	else
	{
		CP_Image_Draw(HowTo.btnImage, HowTo.x, HowTo.y, HowTo.width, HowTo.height, 300);
	}

	if (currentState == BUTTON_MAPEDITOR)
	{
		CP_Image_Draw(MapEditor.btnHoverImage, MapEditor.x, MapEditor.y, MapEditor.width, MapEditor.height, 300);
		CP_Image_Draw(MapEditor.LCursorImage, MapEditor.x - MapEditor.x * 0.22f, MapEditor.y, CURSOR_WIDTH, CURSOR_HEIGHT, 300);
	}
	else
	{
		CP_Image_Draw(MapEditor.btnImage, MapEditor.x, MapEditor.y, MapEditor.width, MapEditor.height, 300);
	}

	if (currentState == BUTTON_LOAD)
	{
		CP_Image_Draw(LoadLevel.btnHoverImage, LoadLevel.x, LoadLevel.y, LoadLevel.width, LoadLevel.height, 300);
		CP_Image_Draw(LoadLevel.LCursorImage, LoadLevel.x - LoadLevel.x * 0.22f, LoadLevel.y, CURSOR_WIDTH, CURSOR_HEIGHT, 300);
	}
	else
	{
		CP_Image_Draw(LoadLevel.btnImage, LoadLevel.x, LoadLevel.y, LoadLevel.width, LoadLevel.height, 300);
	}

	if (currentState == BUTTON_CREDITS)
	{
		CP_Image_Draw(Credits.btnHoverImage, Credits.x, Credits.y, Credits.width, Credits.height, 300);
		CP_Image_Draw(Credits.LCursorImage, Credits.x - Credits.x * 0.22f, Credits.y, CURSOR_WIDTH, CURSOR_HEIGHT, 300);
	}
	else
	{
		CP_Image_Draw(Credits.btnImage, Credits.x, Credits.y, Credits.width, Credits.height, 300);
	}

	if (currentState == BUTTON_QUITSTATE)
	{
		CP_Image_Draw(Quit.btnHoverImage, Quit.x, Quit.y, Quit.width, Quit.height, 300);
		CP_Image_Draw(Quit.LCursorImage, Quit.x - Quit.x * 0.22f, Quit.y, CURSOR_WIDTH, CURSOR_HEIGHT, 300);
	}
	else
	{
		CP_Image_Draw(Quit.btnImage, Quit.x, Quit.y, Quit.width, Quit.height, 300);
	}
}


void MainMenu_Update()
{
	//load image to main menu
	CP_Image Menubackground = CP_Image_Load("./Images/Menu.png");
	//draw the main menu background
	float MenuX, MenuY;
	MenuX = WINDOW_WIDTH / 2.0f;
	MenuY = WINDOW_HEIGHT / 2.0f;
	CP_Image_Draw(Menubackground, MenuX, MenuY, WINDOW_WIDTH, WINDOW_HEIGHT, 300);

	//call the MainMenu_Render function
	MainMenu_Render();

	//able to toggle the button with key S, W, down and up
	if (CP_Input_KeyTriggered(KEY_S) || CP_Input_KeyTriggered(KEY_DOWN))
	{
		PlaySoundEffect(SOUND_SELECTBTN);
		currentState++;
		if (currentState == MAX_BUTTON_STATE)
		{
			currentState = BUTTON_STARTSTATE;
		}
	}
	if (CP_Input_KeyTriggered(KEY_W) || CP_Input_KeyTriggered(KEY_UP))
	{
		PlaySoundEffect(SOUND_SELECTBTN);
		if (currentState == BUTTON_STARTSTATE)
		{
			currentState = MAX_BUTTON_STATE;
		}

		currentState--;

	}

	float Dt = CP_System_GetDt();
	Transit_UpdateToBig(&transition, Dt);


	if (CP_Input_KeyTriggered(KEY_SPACE))
	{
		PlaySoundEffect(SOUND_SELECTBTN);

		//access enum button state
		switch (currentState)
		{
		case BUTTON_STARTSTATE:
			Transit_NextScene(&transition, TutorialScene_Init, TutorialScene_Update, TutorialScene_Exit);
			break;

		case BUTTON_HOWTOSTATE:
			Transit_NextScene(&transition, HowToPlay_Init, HowToPlay_Update, HowToPlay_Exit);
			break;

		case BUTTON_QUITSTATE:
			CP_Engine_Terminate();
			break;

		case BUTTON_LOAD:
			Transit_NextScene(&transition, TestScene_Init, TestScene_Update, TestScene_Exit);
			break;

		case BUTTON_MAPEDITOR:
			Transit_NextScene(&transition, MapEditor_Init, MapEditor_Update, MapEditor_Exit);
			break;
		
		case BUTTON_CREDITS:
			Transit_NextScene(&transition, Credits_Init, Credits_Update, Credits_Exit);
			break;

		default:
			break;
		}
	}
}


void MainMenu_Exit(void)
{
	FreeSound();
	CP_Image_Free(&Start.btnImage);
	CP_Image_Free(&Start.btnHoverImage);
	CP_Image_Free(&Start.LCursorImage);

	CP_Image_Free(&HowTo.btnImage);
	CP_Image_Free(&HowTo.btnHoverImage);
	CP_Image_Free(&HowTo.LCursorImage);

	CP_Image_Free(&Quit.btnImage);
	CP_Image_Free(&Quit.btnHoverImage);
	CP_Image_Free(&Quit.LCursorImage);

	CP_Image_Free(&MapEditor.btnImage);
	CP_Image_Free(&MapEditor.btnHoverImage);
	CP_Image_Free(&MapEditor.LCursorImage);

	CP_Image_Free(&LoadLevel.btnImage);
	CP_Image_Free(&LoadLevel.btnHoverImage);
	CP_Image_Free(&LoadLevel.LCursorImage);
	
	CP_Image_Free(&Credits.btnImage);
	CP_Image_Free(&Credits.btnHoverImage);
	CP_Image_Free(&Credits.LCursorImage);

	CP_Sound_Free(mainMenuMusic);
}
