/*!
@file		Renderer.h
@author		Daniel Chua Yee Chen (yeechendaniel.chua)
@course		CSD1400
@brief		Contains implementations for functions used as a object
			as a base unit for rendering. Sprite animations is done
			within renderer.

			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/

#ifndef _RENDERER_H
#define _RENDERER_H

#include <cprocessing.h>
#include "../GlobalDefine.h"
#include <stdbool.h>

typedef struct Renderer
{
	CP_Vector position;
	CP_Vector scale;
	
	int alpha;
	float rotation;
	IMAGE_TYPE image;
	CP_Color color;


	//variables for sprite animation

	//size of each sprite in the sprite sheet
	float spriteSize;
	//number of row in the sprite sheet
	int numRow;
	//number of columns in current animation.
	int numCol;

	//current row to be displayed
	int currRow;
	//current column to be displayed.
	int currCol;

	// private variable to help iterate to next animation.
	float timer;
	//animation speed. number of animation frames per second.
	float animSpeed;
	//is this animation looping. if the animation is not looping, it will play till the end and freeze on the last frame.
	bool isLooping;


} Renderer;

void Renderer_InitSpriteSheet(Renderer* renderer,IMAGE_TYPE image,float sizeX, float sizeY, int numRow, int numCol);
void Renderer_PlayAnimation(Renderer* renderer, int row, int numCol, float animSpeed, bool loop);


#endif
