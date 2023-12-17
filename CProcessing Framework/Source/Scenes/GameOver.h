/*!
@file       GameOver.h
@author     Chua Yip Xuan (yipxuan.chua)
@course     CSD1400
@section    PASCAL-AM
@brief		GameOver scene when player's health reaches 0 or loses the game

			Copyright © 2020 DigiPen, All rights reserved.
*/
/*__________________________________________________________________________*/

#ifndef GAMEOVER_H
#define GAMEOVER_H
#include "../../CProcessing/inc/cprocessing.h"
#include "../Sound/Sound.h"
#include "TestScene.h"
#include "MainMenu.h"

CP_Image background;

CP_Sound gameOverMusic;

typedef enum STATES
{
	BTN_RESTART = 0,
	BTN_MAIN,
	BTN_QUIT,
	ENDOF_STATE
}STATES;

STATES current;

typedef struct Button_GO
{
	CP_Image btn, selectedBtn;
	float x, y, width, height;
	STATES states;
}Button_GO;

Button_GO restartBtn;
Button_GO quitBtn;
Button_GO mainBtn;

Transition transit;

void GameOver_init(void);
void GameOver_update(void);
void GameOver_exit(void);
void GameOver_render(void);

#endif