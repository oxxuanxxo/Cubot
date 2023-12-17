/*!
@file		RendererUtils.h
@author		Daniel Chua Yee Chen (yeechendaniel.chua)
@course		CSD1400
@brief		Contains implementations for wrapper functions used for rendering

			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/

#ifndef _RENDER_UTIL_H
#define _RENDER_UTIL_H

#include "Renderer.h"
#include "../Camera/Camera.h"
#include "../Misc/VariableList.h"
#include "../GlobalDefine.h"

void LoadImages(void);
void Render_Rect(const Camera* cam, const Renderer* renderer);
void Render_Image(const Camera* cam, const Renderer* renderer);
void Render_Sprite(const Camera* cam, Renderer* renderer);



#endif