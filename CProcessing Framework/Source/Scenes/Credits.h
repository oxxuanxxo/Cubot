/*!
@file       Credits.h
@author     Chua Yip Xuan (yipxuan.chua)
@course     CSD1400
@section    PASCAL-AM
@brief      This will show credits screen when credit button is selected in main menu

			Copyright © 2020 DigiPen, All rights reserved.
*/
/*__________________________________________________________________________*/

#ifndef CREDITS_H
#define CREDITS_H

#include "../../CProcessing/inc/cprocessing.h"
#include "Transition.h"
#include "../Sound/Sound.h"


float deltaT;
float WINWIDTH;
float WINHEIGHT;

Transition trans;

CP_Image credits1;
CP_Image credits2;
CP_Image credits3;
CP_Image credits4;
CP_Image credits5;
CP_Image credits6;

CP_Sound creditsMusic;

void Credits_Init(void);
void Credits_Update(void);
void Credits_Exit(void);

#endif
