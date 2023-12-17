/*!
@file       Parallax.h
@author     Chua Yip Xuan (yipxuan.chua)
@course     CSD1400
@section    PASCAL-AM
@brief		This file consists of functions that load foregrounds and stitch the foregrounds according to the direction of the camera's movement and direction
			and moving foregrounds, like parallax scrolling

			Copyright © 2020 DigiPen, All rights reserved.
*/
/*__________________________________________________________________________*/

#ifndef PARALLAX_H
#define PARALLAX_H

#include <stdbool.h>
#include "../../CProcessing/inc/cprocessing.h"
#include "../Camera/Camera.h"
#include "../Characters/Player.h"
#include "../Render/Renderer.h"
#include "../Render/RenderUtil.h"
#include "../GlobalDefine.h"

typedef struct ParallaxImage
{
	//CP_Vector* camPos;
	float vel;
	CP_Vector prevCam;
	//Renderer* renderImage;
	Camera* camera;
	Renderer render1;
	Renderer render2;
}ParallaxImage;

//ParallaxImage render1;
//ParallaxImage render2;

void Parallax_Load(ParallaxImage* parallax, IMAGE_TYPE image, float image_x, float image_y,
	float image_width, float image_height, int image_alpha, float image_velocity, Camera* camera);

void Parallax_Update(ParallaxImage* parallax);
void ParallaxNotMovingY_Update(ParallaxImage* parallax);

void Parallax_Render(ParallaxImage* parallax, Camera* camera);


#endif //
