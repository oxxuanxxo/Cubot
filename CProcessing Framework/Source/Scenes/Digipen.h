/*!
@file       Digipen.h
@author     Chua Yip Xuan (yipxuan.chua)
@course     CSD1400
@section    PASCAL-AM
@brief		Digipen logo will appear for 2.5 seconds before transiting to main menu or other scenes

			Copyright © 2020 DigiPen, All rights reserved.
*/
/*__________________________________________________________________________*/

#ifndef LOGO_H
#define LOGO_H

#include "../../CProcessing/inc/cprocessing.h"
#include "Transition.h"

float WINWIDTH;
float WINHEIGHT;
float logoTimer;
CP_Image logo;

Transition transit;

void Digipen_Init(void);
void Digipen_Update(void);
void Digipen_Exit(void);

#endif