#ifndef _WIN_SCENE_H
#define _WIN_SCENE_H

#include "../Camera/Camera.h"
#include "../Render/RenderUtil.h"
#include "Transition.h"

CP_Image background_;

Transition transition;

void WinScene_Init(void);
void WinScene_Update(void);
void WinScene_Render(void);
void WinScene_Exit(void);

#endif

