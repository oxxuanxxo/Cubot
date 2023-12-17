//---------------------------------------------------------
// file:	MainMenu.h
// author:	Wu Pei Fang(peifang.w)
// email:	peifang.w@digipen.edu
//
// brief:	This file include functions for MainMenu.c
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#ifndef MAINMENU_H
#define MAINMENU_H
#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define CURSOR_WIDTH 50
#define CURSOR_HEIGHT 50

#include "cprocessing.h"
#include "../Render/Renderer.h"
#include "Transition.h"
#include <stdbool.h>

////to store the current state of the state machine for main menu button
typedef enum BUTTON_STATE
{
	BUTTON_STARTSTATE = 0,
	BUTTON_HOWTOSTATE,
	BUTTON_MAPEDITOR,
	BUTTON_LOAD,
	BUTTON_CREDITS,
	BUTTON_QUITSTATE,
	MAX_BUTTON_STATE
}BUTTON_STATE;

struct Button
{
	float x, y, width, height;
	CP_Image btnImage;
	CP_Image btnHoverImage;
	CP_Image LCursorImage;
	BUTTON_STATE button_state;
};


struct Button Start;
struct Button HowTo;
struct Button MapEditor;
struct Button LoadLevel;
struct Button Quit;
struct Button Credits;

Transition transition;

BUTTON_STATE currentState;

CP_Sound mainMenuMusic;

//declare main menu function
void MainMenu_Init(void);
void MainMenu_Update(void);
void MainMenu_Render(void);
void MainMenu_Exit(void);


#endif
