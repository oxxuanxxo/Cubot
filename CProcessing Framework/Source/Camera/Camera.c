/*!
@file Camera.c
@author Daniel Chua (yeechendaniel.chua)
@date 15/10/20
@brief This file contains functions that deal with the Camera.

			Copyright © 2020 DigiPen, All rights reserved.
*//*________________________________________________________________________
_*/



#include "Camera.h"



/*!
@brief Initilize default values of camera. This should be called in each scene's Init
@param camera - pointer that points to the camera to initialize.
@return void - none
*//*________________________________________________________________________
_*/
void Camera_Init(Camera* camera)
{
	camera->position = CP_Vector_Set(0.0f, 0.0f);
	camera->scale = CP_Vector_Set(1.0f, 1.0f);
	//camera->scale = CP_Vector_Set(50.0f, 50.0f);


	//camera->scale.y *= -1.0f; // flips the y axis (screen space y is downwards)
	camera->viewMatrix = CP_Matrix_Identity();
	camera->rotation = 0.0f;

	
	Camera_UpdateProjection(camera);


}

/*!
@brief Updates the projection matrix of the camera
@param camera - pointer that points to the camera to update.
@return void - none
*//*________________________________________________________________________
_*/
void Camera_UpdateProjection(Camera* camera)
{
	//this matrix handles the conversion from camera space to screen space.
	CP_Matrix translate = CP_Matrix_Translate(CP_Vector_Set(CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() / 2.0f));
	//this matrix will flip the y-axis.
	CP_Matrix yReverse = CP_Matrix_Scale(CP_Vector_Set(1.0f, -1.0f));

	//camera->projectionMatrix = translate;
	camera->projectionMatrix = CP_Matrix_Multiply(translate, yReverse);
}


/*!
@brief Update matrices of the camera and use CP_Setting_ApplyMatrix to apply the VPMatrix to the world.
@param camera - pointer that points to the camera to initialize.
@return void - none
*//*________________________________________________________________________
_*/
void Camera_Update(Camera* camera)
{
	CP_Matrix scale = CP_Matrix_Scale((camera->scale));

	// move to camera position, offset based on left of screen.
	CP_Matrix translate = CP_Matrix_Translate(CP_Vector_Set(camera->position.x,camera->position.y)); //y-axis is inverted here to prevent scaling from being inverted.
	
	CP_Matrix rotate = CP_Matrix_Rotate(camera->rotation);

	//camera->transfrom = CP_Matrix_Multiply(CP_Matrix_Multiply(translate, yReverse) ,CP_Matrix_Multiply(rotate,scale));
	camera->transfrom = CP_Matrix_Multiply(translate, CP_Matrix_Multiply(rotate,scale));
	
	camera->viewMatrix = CP_Matrix_Inverse(camera->transfrom); //this matrix converts from world space to camera space

	camera->VPMatrix = CP_Matrix_Multiply(camera->projectionMatrix, camera->viewMatrix); //projection matrix converts from camera space to screen space

	//CP_Settings_ApplyMatrix(camera->VPMatrix);
}

/*!
@brief Helper function to convert from screen space to world space coordinates
@param camera - pointer that points to the camera to initialize.
@param screenCoords - vector2 containing the screen space coodinates to be converted
@return void - none
*//*________________________________________________________________________
_*/
CP_Vector Camera_ScreenToWorldSpace(Camera* camera, CP_Vector screenCoords)
{
	return CP_Vector_MatrixMultiply(CP_Matrix_Inverse(camera->VPMatrix), screenCoords);
}


/*!
@brief Helper function to convert from world space to screen space coordinates
@param camera - pointer that points to the camera to initialize.
@param worldCoords - vector2 containing the world space coodinates to be converted
@return void - none
*//*________________________________________________________________________
_*/
CP_Vector Camera_WorldToScreenSpace(Camera* camera, CP_Vector worldCoords)
{
	return CP_Vector_MatrixMultiply(camera->VPMatrix,worldCoords);
}

/*!
@brief default camera controls using arrow keys for movements and I and O for zoom
@param camera - pointer that points to the camera to initialize.
@return void - none
*//*________________________________________________________________________
_*/
void Camera_Input(Camera* camera)
{
	float cameraSpeed = 200.0f;
	float cameraScaleSpeed = 1.0f;
	float dt = CP_System_GetDt();
	if (CP_Input_KeyDown(KEY_UP))
	{
		camera->position.y += cameraSpeed * dt;
	}
	if (CP_Input_KeyDown(KEY_DOWN))
	{
		camera->position.y -= cameraSpeed * dt;
	}
	if (CP_Input_KeyDown(KEY_RIGHT))
	{
		camera->position.x += cameraSpeed * dt;
	}
	if (CP_Input_KeyDown(KEY_LEFT))
	{
		camera->position.x -= cameraSpeed * dt;
	}

	if (CP_Input_KeyDown(KEY_I))
	{
		camera->scale = CP_Vector_Add(camera->scale, CP_Vector_Set(cameraScaleSpeed * dt, cameraScaleSpeed * dt));
	}
	if (CP_Input_KeyDown(KEY_O))
	{
		camera->scale = CP_Vector_Add(camera->scale, CP_Vector_Set(-cameraScaleSpeed * dt, -cameraScaleSpeed * dt));
	}
}


void Camera_Bounds(Camera* camera, CP_Vector min, CP_Vector max)
{
	if (camera->position.x < min.x)
	{
		camera->position.x = min.x;
	}
	if (camera->position.y < min.y)
	{
		camera->position.y = min.y;
	}

	if (camera->position.x > max.x)
	{
		camera->position.x = max.x;
	}
	if (camera->position.y > max.y)
	{
		camera->position.y = max.y;
	}

}