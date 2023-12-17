/*!
@file		Renderer.c
@author		Daniel Chua Yee Chen (yeechendaniel.chua)
@course		CSD1400
@brief		Contains implementations for functions used as a object
			as a base unit for rendering. Sprite animations is done
			within renderer.

			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/


#include "Renderer.h"

void Renderer_InitSpriteSheet(Renderer* renderer, IMAGE_TYPE image, float sizeX, float sizeY, int numRow, int numCol)
{
	renderer->image = image;
	renderer->spriteSize = sizeY / (float)numRow;

	renderer->numRow = numRow;
	renderer->numCol = numCol;

	renderer->currRow = 0;
	renderer->currCol = 0;

	renderer->timer = 0.0f;
	renderer->animSpeed = 1.0f;

	renderer->isLooping = false;

}

void Renderer_PlayAnimation(Renderer* renderer, int row, int numCol, float animSpeed, bool loop)
{
	renderer->currRow = row;
	renderer->currCol = 0;
	renderer->numCol = numCol;
	renderer->animSpeed = animSpeed;
	renderer->isLooping = loop;
	renderer->timer = 0.0f;
}