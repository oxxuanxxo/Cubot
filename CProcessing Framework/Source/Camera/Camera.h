/*!
@file Camera.h
@author Daniel Chua (yeechendaniel.chua)
@date 15/10/20
@brief This file defines the camera structure and functions related to camera.

			Copyright © 2020 DigiPen, All rights reserved.

*//*________________________________________________________________________
_*/

#ifndef _CAMERA_H
#define _CAMERA_H

#include "CProcessing.h"

typedef struct Camera
{
	CP_Vector position;
	CP_Vector scale; // the bigger the camera scale, the bigger the "view range". Hence objects will be seen as smaller.
	float rotation;

	CP_Matrix transfrom;
	CP_Matrix viewMatrix; //this matrix handles world to camera space conversion
	CP_Matrix projectionMatrix; //this matrix handles camera to screen space conversion
	CP_Matrix VPMatrix; //matrix containing projection*view
} Camera;

void Camera_UpdateProjection(Camera* camera);
void Camera_Init(Camera* camera);
void Camera_Update(Camera* camera);

CP_Vector Camera_ScreenToWorldSpace(Camera* camera, CP_Vector screenCoords);
CP_Vector Camera_WorldToScreenSpace(Camera* camera, CP_Vector worldCoords);
void Camera_Bounds(Camera* camera, CP_Vector min, CP_Vector max);

void Camera_Input(Camera* camera);

#endif