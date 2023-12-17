/*!
@file		Tile.h
@author		Daniel Chua Yee Chen (yeechendaniel.chua)
@course		CSD1400
@brief		Contains enums and structs for base tile.

			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/

#ifndef _TILE_H
#define _TILE_H

#include "../Render/Renderer.h"

typedef enum TILE_TYPE
{
	TILE_EMPTY = 0,
	TILE_BLOCK ,
	TILE_DIRT, // these are used for map editor.
	TILE_TRIANGLE, //these characters arent stored in tile map normally
	TILE_CIRCLE,
	TILE_TURRET,
	TILE_SPIKES,
	TILE_BOUNCE,
	TILE_START,
	TILE_END,
	MAX_TILE
} TILE_TYPE;

typedef struct Tile
{
	TILE_TYPE tileType;
	int row;
	int column;
	
	Renderer sprite;

} Tile;
#endif