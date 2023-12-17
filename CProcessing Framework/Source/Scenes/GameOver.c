/*!
@file       GameOver.c
@author     Chua Yip Xuan (yipxuan.chua)
@course     CSD1400
@section    PASCAL-AM
@brief		GameOver scene when player's health reaches 0 or loses the game

			Copyright © 2020 DigiPen, All rights reserved.
*/
/*__________________________________________________________________________*/

#include "GameOver.h"
#include"../Sound/Sound.h"
#include "Transition.h"
#include "../GlobalDefine.h"

#include "TutorialScene.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "Stage4.h"

#define WIN_HEIGHT 900
#define WIN_WIDTH 1600

//initialise buttons
void GameOver_init(void)
{
	background = CP_Image_Load("././Images/GameOver.png");
	//restart
	restartBtn.btn = CP_Image_Load("././Images/Restart.png");
	restartBtn.selectedBtn = CP_Image_Load("././Images/SelectedRestart.png");
	restartBtn.x = (WIN_WIDTH * 0.5f);
	restartBtn.y = (WIN_HEIGHT * 0.1f) * 5.0f;
	restartBtn.width = 350.0f;
	restartBtn.height = 130.0f;
	restartBtn.states = BTN_RESTART;

	//quit
	quitBtn.btn = CP_Image_Load("././Images/Quit.png");
	quitBtn.selectedBtn = CP_Image_Load("././Images/SelectedQuit.png");
	quitBtn.x = (WIN_WIDTH * 0.5f);
	quitBtn.y = (WIN_HEIGHT * 0.1f) * 9.0f;
	quitBtn.width = 350.0f;
	quitBtn.height = 130.0f;
	quitBtn.states = BTN_QUIT;

	//main
	mainBtn.btn = CP_Image_Load("././Images/Main.png");
	mainBtn.selectedBtn = CP_Image_Load("././Images/SelectedMain.png");
	mainBtn.x = (WIN_WIDTH * 0.5f);
	mainBtn.y = (WIN_HEIGHT * 0.1f) * 7.0f;
	mainBtn.width = 350.0f;
	mainBtn.height = 130.0f;
	mainBtn.states = BTN_MAIN;

	current = BTN_RESTART;
	
	SoundLoad();
	Transit_Create(&transit);

	gameOverMusic = CP_Sound_LoadMusic("./Assets/MenuMusic.mp3");
	CP_Sound_PlayAdvanced(gameOverMusic, 0.4f, 1.2f, 1, CP_SOUND_GROUP_9);
}

//update every frame
void GameOver_update(void)
{
	//render images
	GameOver_render();

	//if key triggered w or up, and current state is restart, current state will go from restart state to endofstate, decrement it after the if function, 
	//it will go to the last button which is Quit button
	if (CP_Input_KeyTriggered(KEY_W) || CP_Input_KeyTriggered(KEY_UP))
	{
		PlaySoundEffect(SOUND_SELECTBTN);
		if (current == BTN_RESTART)
		{
			current = ENDOF_STATE;
		}
		current--;
	}
	//if key triggered s or down, increment the state, which will go to end of state, 
	//if current state is end of state, current state = restart state
	if (CP_Input_KeyTriggered(KEY_S) || CP_Input_KeyTriggered(KEY_DOWN))
	{
		PlaySoundEffect(SOUND_SELECTBTN);
		current++;
		if (current == ENDOF_STATE)
		{
			current = BTN_RESTART;
		}
	} //loop 

	float dt = CP_System_GetDt();
	Transit_UpdateToBig(&transit, dt);

	//transiting into next scene when press key space
	if (CP_Input_KeyTriggered(KEY_SPACE))
	{
		PlaySoundEffect(SOUND_SELECTBTN);
		switch (current)
		{
		case BTN_RESTART:
		{
			switch (currentStage)
			{
				case STAGE_TUTORIAL:
					{
						Transit_NextScene(&transit, TutorialScene_Init, TutorialScene_Update, TutorialScene_Exit);
						break;
					}
				case STAGE_1:
					{
						Transit_NextScene(&transit, Stage1_Init, Stage1_Update, Stage1_Exit);
						break;
					}
				case STAGE_2:
					{
						Transit_NextScene(&transit, Stage2_Init, Stage2_Update, Stage2_Exit);
						break;
					}
				case STAGE_3:
					{
						Transit_NextScene(&transit, Stage3_Init, Stage3_Update, Stage3_Exit);
						break;
					}
				case STAGE_4:
					{
						Transit_NextScene(&transit, Stage4_Init, Stage4_Update, Stage4_Exit);
						break;
					}

				default:
					{
						Transit_NextScene(&transit, MainMenu_Init, MainMenu_Update, MainMenu_Exit);
						break;
					}	
			}
			break;
		}
		
		case BTN_QUIT:
			CP_Engine_Terminate();
			break;
		case BTN_MAIN:
			Transit_NextScene(&transit, MainMenu_Init, MainMenu_Update, MainMenu_Exit);
			//CP_Engine_SetNextGameState(MainMenu_Init,MainMenu_Update, MainMenu_Exit);
			break;
		default:
			break;
		}
	}
}

//render images 
void GameOver_render(void)
{
	background = CP_Image_Load("././Images/GameOver.png");
	CP_Image_Draw(background, WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f, WIN_WIDTH, WIN_HEIGHT, 255);

	if (current == BTN_RESTART)
	{
		CP_Image_Draw(restartBtn.selectedBtn, restartBtn.x, restartBtn.y, restartBtn.width, restartBtn.height, 255);
	}
	else
	{
		CP_Image_Draw(restartBtn.btn, restartBtn.x, restartBtn.y, restartBtn.width, restartBtn.height, 255);
	}
	if (current == BTN_MAIN)
	{
		CP_Image_Draw(mainBtn.selectedBtn, mainBtn.x, mainBtn.y, mainBtn.width, mainBtn.height, 255);
	}
	else
	{
		CP_Image_Draw(mainBtn.btn, mainBtn.x, mainBtn.y, mainBtn.width, mainBtn.height, 255);
	}
	if (current == BTN_QUIT)
	{
		CP_Image_Draw(quitBtn.selectedBtn, quitBtn.x, quitBtn.y, quitBtn.width, quitBtn.height, 255);
	}
	else
	{
		CP_Image_Draw(quitBtn.btn, quitBtn.x, quitBtn.y, quitBtn.width, quitBtn.height, 255);
	}
}

//free sound and images
void GameOver_exit(void)
{
	CP_Image_Free(&restartBtn.btn);
	CP_Image_Free(&quitBtn.btn);
	CP_Image_Free(&mainBtn.btn);
	CP_Image_Free(&restartBtn.selectedBtn);
	CP_Image_Free(&quitBtn.selectedBtn);
	CP_Image_Free(&mainBtn.selectedBtn);
	CP_Image_Free(&background);
	FreeSound();
	CP_Sound_Free(gameOverMusic);
}