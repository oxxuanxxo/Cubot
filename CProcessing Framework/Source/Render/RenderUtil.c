/*!
@file		RendererUtils.c
@author		Daniel Chua Yee Chen (yeechendaniel.chua)
@course		CSD1400
@brief		Contains implementations for wrapper functions used for rendering

			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/

#include "RenderUtil.h"

void LoadImages(void)
{
	VariableListFree(&g_imageList);

	VariableListCreate(&g_imageList, 12);
	CP_Image temp;

	temp = CP_Image_Load("./Images/Apple.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/Circle.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/Player.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/Tile1.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/Tile2.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/Slime.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/Triangle.png");
	VariableListAdd(&g_imageList, temp);
	
	temp = CP_Image_Load("./Images/Turret.png");
	VariableListAdd(&g_imageList, temp);
	
	temp = CP_Image_Load("./Images/Bullet.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/Smoke.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/NPC.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/Spikes.png");
	VariableListAdd(&g_imageList, temp);
	
	temp = CP_Image_Load("./Images/Dialogue.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/testSprite.png");
	VariableListAdd(&g_imageList, temp);
	
	temp = CP_Image_Load("./Images/PlayerDash.png");
	VariableListAdd(&g_imageList, temp);
	
	temp = CP_Image_Load("./Images/Alert.png");
	VariableListAdd(&g_imageList, temp);
	
	temp = CP_Image_Load("./Images/CircleParticle.png");
	VariableListAdd(&g_imageList, temp);
	
	temp = CP_Image_Load("./Images/TriangleParticle.png");
	VariableListAdd(&g_imageList, temp);
	
	temp = CP_Image_Load("./Images/PlayerParticle.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/Heart.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/PlayerSpriteSheet.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/SlimeSpriteSheet.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/FlagStart.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/FlagFinish.png");
	VariableListAdd(&g_imageList, temp);

	//PARA
	temp = CP_Image_Load("./Images/Parallax/ParallaxBackMountain.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/Parallax/ParallaxBackTrees.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/Parallax/ParallaxClouds.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/Parallax/ParallaxFrontMountain.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/Parallax/ParallaxFrontTrees.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/Parallax/ParallaxMist.png");
	VariableListAdd(&g_imageList, temp);
	
	temp = CP_Image_Load("./Images/Parallax/ParallaxBackRocks.png");
	VariableListAdd(&g_imageList, temp);
	
	temp = CP_Image_Load("./Images/Parallax/ParallaxFrontRocks.png");
	VariableListAdd(&g_imageList, temp);
	//END_PARA

	temp = CP_Image_Load("./Images/MapEditorCursor.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/SlimeParticles.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/Legend.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/NPCDialogue.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/NPCChick.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/NPCFox.png");
	VariableListAdd(&g_imageList, temp);

	temp = CP_Image_Load("./Images/NPCPig.png");
	VariableListAdd(&g_imageList, temp);
}


void Render_Rect(const Camera* cam, const Renderer* renderer)
{
	CP_Vector pos = CP_Vector_MatrixMultiply(cam->VPMatrix, renderer->position);

	CP_Settings_Fill(renderer->color);

	CP_Graphics_DrawRect(pos.x, pos.y, renderer->scale.x, renderer->scale.y);
}

void Render_Image(const Camera* cam, const Renderer* renderer)
{
	CP_Vector pos = CP_Vector_MatrixMultiply(cam->VPMatrix, renderer->position);

	CP_Image_DrawAdvanced(VariableListGetItemAt(&g_imageList,renderer->image), pos.x, pos.y, renderer->scale.x, renderer->scale.y, renderer->alpha, renderer->rotation);
}

void Render_Sprite(const Camera* cam, Renderer* renderer)
{
	renderer->timer += CP_System_GetDt();
	if (renderer->timer >= 1.0f / renderer->animSpeed)
	{
		renderer->timer = 0.0f;
		++(renderer->currCol);
		if (renderer->currCol >= renderer->numCol)
		{
			if (renderer->isLooping == true)
			{
				renderer->currCol = 0;
			}
			else
			{
				renderer->currCol = renderer->numCol - 1;
			}
		}
	}

	//guards out of bounds rows
	if (renderer->currRow >= renderer->numRow)
	{
		renderer->currRow = renderer->numRow - 1; // row starts counting from 0
	}
	if (renderer->currCol >= renderer->numCol)
	{
		renderer->currRow = renderer->numCol - 1;// col starts counting from 0
	}

	CP_Vector pos = CP_Vector_MatrixMultiply(cam->VPMatrix, renderer->position);

	/*CP_Image_DrawSubImage(VariableListGetItemAt(&g_imageList, renderer->image), pos.x, pos.y, renderer->scale.x, renderer->scale.y, 
		renderer->currCol * renderer->spriteSize, renderer->currRow * renderer->spriteSize, 
		(renderer->currCol+1) * renderer->spriteSize, (renderer->currRow +1) * renderer->spriteSize,
		renderer->alpha);*/

	CP_Image_DrawSubImage(VariableListGetItemAt(&g_imageList, renderer->image), pos.x, pos.y, renderer->scale.x, renderer->scale.y,
		renderer->currCol * renderer->spriteSize, renderer->currRow * renderer->spriteSize,
		(renderer->currCol + 1) * renderer->spriteSize, (renderer->currRow + 1) * renderer->spriteSize,
		renderer->alpha);
}