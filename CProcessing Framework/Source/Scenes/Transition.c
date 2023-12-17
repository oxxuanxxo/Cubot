/*!
@file       Transition.c
@author     Chua Yip Xuan (yipxuan.chua)
@course     CSD1400
@section    PASCAL-AM
@brief		This file consists of functions that will animate a circle transition before moving onto the next scene
*/
/*__________________________________________________________________________*/

#include "Transition.h"
#define WIN_WIDTH 1600
#define WIN_HEIGHT 900

//initialise the transition
void Transit_Create(Transition* transition)
{
	transition->timer = 0.0f;
	transition->scale = 0.3f;
	transition->Init = NULL;
	transition->Update = NULL;
	transition->Exit = NULL;
	transition->isTransitingToBig = false;
	transition->isTransitingToSmall = false;
}

//set next scene
void Transit_NextScene(Transition* transition, FunctionPtr Init, FunctionPtr Update, FunctionPtr Exit)
{
	transition->timer = 0.0f;
	//transition->scale = 0.0f;
	transition->Init = Init;
	transition->Update = Update;
	transition->Exit = Exit;
	transition->mSpeed = 1.5f;
	transition->isTransitingToSmall = true;
	transition->isTransitingToBig = true;
}

//set the speed of transition
void setSpeed(Transition* transition, float speed)
{
	transition->mSpeed = speed;
}

//set the circle size 
void Transit_CircleSize(Transition* transition)
{
	float circleSize = (WIN_WIDTH * transition->scale);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Graphics_DrawCircle((WIN_WIDTH * 0.5f), (WIN_HEIGHT * 0.5f), circleSize);
}

//transit from small circle to big
void Transit_UpdateToBig(Transition* transition, float deltaTime)
{
	if (transition->isTransitingToBig == true && deltaTime < 0.3f)
	{
		transition->scale += deltaTime * transition->mSpeed;
		transition->timer += deltaTime;
		Transit_CircleSize(transition);

		if (transition->timer >= 1.0f)
		{
			CP_Engine_SetNextGameState(transition->Init, transition->Update, transition->Exit);
			transition->isTransitingToBig = false;
		}
	}
}

//transit from big circle to small
void Transit_UpdateToSmall(Transition* transition, float deltaTime)
{
	transition->isTransitingToBig = false;
	if (transition->isTransitingToSmall == true)
	{
		setSpeed(transition, -0.8f);
		transition->scale += deltaTime * transition->mSpeed;
		transition->timer += deltaTime;
		Transit_CircleSize(transition);

		float circleSize = (WIN_WIDTH - (WIN_WIDTH * transition->scale));
		//float circleSize = (WIN_WIDTH * transition->scale);
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
		CP_Graphics_DrawCircle((WIN_WIDTH * 0.5f), (WIN_HEIGHT * 0.5f), circleSize);

		if (transition->timer >= 2.0f || circleSize <= 0.0f)
		{
			transition->isTransitingToSmall = false;
		}
	}
}


