/*!
@file		MapEditor.h
@author		Daniel Chua Yee Chen (yeechendaniel.chua)
@course		CSD1400
@brief		Contains implementations for functions used for
			the map editor scene.

			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/


#ifndef _MAP_EDITOR_H
#define _MAP_EDITOR_H

#include "../Camera/Camera.h"
#include "../Tiles/TileMap.h"
#include "Transition.h"

typedef enum SELECT_STATE
{
	SELECT_EMPTY =0,
	SELECT_BLOCK,
	SELECT_DIRT,
	SELECT_TRIANGLE,
	SELECT_CIRCLE,
	SELECT_TURRET,
	SELECT_SPIKE,
	SELECT_BOUNCE,
	SELECT_START,
	SELECT_END,
	MAX_SELECT
} SELECT_STATE;

Camera cam;
TileMap tileMap;

CP_Image background_;

int selectionAlpha;
int currRow;
int currCol;

float debounceTimer;

Renderer selection;
Renderer sbg; //selection back ground

SELECT_STATE currentSelection;

Renderer legend;
Transition transition;


void MapEditor_Init(void);
void MapEditor_Update(void);
void MapEditor_Render(void);
void MapEditor_Exit(void);


#endif