/*!
@file       Transition.h
@author     Chua Yip Xuan (yipxuan.chua)
@course     CSD1400
@section    PASCAL-AM
@brief		This file consists of functions that will animate a circle transition before moving onto the next scene
*/
/*__________________________________________________________________________*/

#ifndef TRANS_H
#define TRANS_H

#include <stdbool.h>
#include "../../CProcessing/inc/cprocessing.h"

typedef struct Transition
{
	float timer;
	float scale; //speed * deltatime
	float mSpeed; 
	bool isTransitingToBig;
	bool isTransitingToSmall;
	FunctionPtr Init;
	FunctionPtr Update; 
	FunctionPtr Exit;
}Transition;

void Transit_Create(Transition* transition);
void Transit_UpdateToBig(Transition* transition, float deltaTime);
void Transit_UpdateToSmall(Transition* transition, float deltaTime);

void Transit_CircleSize(Transition* transition);
void setSpeed(Transition* transition, float speed);
void Transit_NextScene(Transition* transition, FunctionPtr Init, FunctionPtr Update, FunctionPtr Exit);
//void Transit_NoScene(Transition* transition);

//void Transit_CircleToBig(float timer);
//void Transit_CircleToSmall(float timer);
//
//void Transit_RectToRight(void);
//void Transit_RectToLeft(void);
//void Transit_RectToUp(void);
//void Transit_RectToDown(void);



#endif
