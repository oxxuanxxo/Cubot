/*!
@file       Parallax.c
@author     Chua Yip Xuan (yipxuan.chua)
@course     CSD1400
@section    PASCAL-AM
@brief		This file consists of functions that load foregrounds and stitch the foregrounds according to the direction of the camera's movement and direction
			and moving foregrounds, like parallax scrolling

			Copyright © 2020 DigiPen, All rights reserved.
*/			
/*__________________________________________________________________________*/

#include "Parallax.h"
#include "../Camera/Camera.h"

//initialise the variables
void Parallax_Load(ParallaxImage* parallax, IMAGE_TYPE image, float image_x, float image_y, 
	float image_width, float image_height, int image_alpha, float image_velocity, Camera* camera)
{
	//CP_Image image_I = image; 
	parallax->camera = camera;
	parallax->prevCam = camera->position;
	//parallax->camPos = camera;
	parallax->render1.position.x = image_x;
	parallax->render1.position.y = image_y;
	parallax->render1.scale.x = image_width;
	parallax->render1.scale.y = image_height;
	parallax->render1.alpha = image_alpha;
					 
	parallax->render2.position.x = parallax->render1.position.x + parallax->render1.scale.x;
	parallax->render2.position.y = image_y;
	parallax->render2.scale.x = image_width;
	parallax->render2.scale.y = image_height;
	parallax->render2.alpha = image_alpha;

	parallax->vel = image_velocity;

	parallax->render1.image = image;
	parallax->render2.image = image;

}

//foregrounds will stitch according to the camera position and direction
//foregrounds will move according to the input velocity when called and initialised
void Parallax_Update(ParallaxImage* parallax)
{
	//parallax->render1->position = parallax->camera->position;
	//get the direction of where the camera or player will be moving
	CP_Vector dirTemp = CP_Vector_Subtract(parallax->camera->position, parallax->prevCam) ;

	float negOffSet = parallax->camera->position.x - (CP_System_GetWindowWidth() * 0.5f);
	float posOffSet = parallax->camera->position.x + (CP_System_GetWindowWidth() * 0.5f);

	parallax->render1.position.x += dirTemp.x * parallax->vel;
	parallax->render2.position.x += dirTemp.x * parallax->vel;

	//check behind, if it is going behind, stitch the foreground in that direction
	if (dirTemp.x >= 0)
	{
		if (parallax->render1.position.x + parallax->render1.scale.x * 0.5f < negOffSet)
		{
			parallax->render1.position.x = parallax->render2.position.x + parallax->render2.scale.x;

		}
		if (parallax->render2.position.x + parallax->render2.scale.x * 0.5f < negOffSet)
		{
			parallax->render2.position.x = parallax->render1.position.x + parallax->render1.scale.x;
		}
	}
	//check infront, if it is going forward, stitch the foreground in that direction
	else
	{
		if (parallax->render2.position.x - parallax->render2.scale.x * 0.5f > posOffSet)
		{
			parallax->render2.position.x = parallax->render1.position.x - parallax->render1.scale.x;
		}
		if (parallax->render1.position.x - parallax->render1.scale.x * 0.5f > posOffSet)
		{
			parallax->render1.position.x = parallax->render2.position.x - parallax->render2.scale.x;
		}
	}
	parallax->render1.position.y = parallax->camera->position.y;
	parallax->render2.position.y = parallax->camera->position.y;

	parallax->prevCam = parallax->camera->position;
	
}

//foregrounds will stitch according to the camera position and direction
//foregrounds will move according to the input velocity when called and initialised
//foregrounds y pos will not move if this function is called mianly for trees and rocks
void ParallaxNotMovingY_Update(ParallaxImage* parallax)
{
	//parallax->render1->position = parallax->camera->position;
	//get the direction of where the camera or player will be moving
	CP_Vector dirTemp = CP_Vector_Subtract(parallax->camera->position, parallax->prevCam);

	float negOffSet = parallax->camera->position.x - (CP_System_GetWindowWidth() * 0.5f);
	float posOffSet = parallax->camera->position.x + (CP_System_GetWindowWidth() * 0.5f);

	parallax->render1.position.x += dirTemp.x * parallax->vel;
	parallax->render2.position.x += dirTemp.x * parallax->vel;

	//check behind, if it is going behind, stitch the foreground in that direction
	if (dirTemp.x >= 0)
	{
		if (parallax->render1.position.x + parallax->render1.scale.x * 0.5f < negOffSet)
		{
			parallax->render1.position.x = parallax->render2.position.x + parallax->render2.scale.x;

		}
		if (parallax->render2.position.x + parallax->render2.scale.x * 0.5f < negOffSet)
		{
			parallax->render2.position.x = parallax->render1.position.x + parallax->render1.scale.x;
		}
	}
	//check infront, if it is going forward, stitch the foreground in that direction
	else
	{
		if (parallax->render2.position.x - parallax->render2.scale.x * 0.5f > posOffSet)
		{
			parallax->render2.position.x = parallax->render1.position.x - parallax->render1.scale.x;
		}
		if (parallax->render1.position.x - parallax->render1.scale.x * 0.5f > posOffSet)
		{
			parallax->render1.position.x = parallax->render2.position.x - parallax->render2.scale.x;
		}
	}
	parallax->prevCam = parallax->camera->position;

}

//render images
void Parallax_Render(ParallaxImage* parallax, Camera* camera)
{
	Render_Image(camera, &parallax->render1);
	Render_Image(camera, &parallax->render2);
}

