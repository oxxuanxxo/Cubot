/*!
@file		TileMap.h
@author		Daniel Chua Yee Chen (yeechendaniel.chua)
@course		CSD1400
@brief		Contains implementations for functions used in loading
			and interacting with tile map.

			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/

#ifndef _TILE_MAP_H
#define _TILE_MAP_H

#include "Tile.h"
#include "../Camera/Camera.h"
#include "../GlobalDefine.h"
#include <cprocessing.h>
#include <stdbool.h>
#include "../Misc/VariableList.h"

struct Player;

typedef struct TileMap
{
	Tile** map; //2D array to store all the tiles.
	float tileSize;
	int numRow; //total number of rows
	int numCol; //total number of columns

	IMAGE_TYPE tileImage;

	Tile* startTile;
	Tile* endTile;

	bool mapWin;

	//list of enemies
	VariableList triangleList;
	VariableList circleList;
	VariableList turretList;

	VariableList spikeList;
	VariableList bounceList;

} TileMap;

//Gets the tile based on its Row and Column
Tile* GetTile_RowCol(TileMap* tileMap, int row, int col);
//Gets the tile based on its World position (not done) 
Tile* GetTile_WorldPos(TileMap* tileMap, float x, float y);
//Gets the WorldPosition (pivot point) based on tile's row and Column
CP_Vector Tile_GetWorldPos(TileMap* tileMap, int row, int col);

//Checks if the position is within a tile that is a block (should have collider)
bool IsColliding_WorldPos(TileMap* tileMap, float x, float y);
bool IsColliding_Tile(Tile* tile);

bool IsColliding(TileMap* tileMap,Tile* tile, CP_Vector pos);

void SetTileType(TileMap* tileMap, Tile* tile,const char* string);
void RenderTileMap(const Camera* camera,const TileMap* tileMap);

void UpdateTileMap(TileMap* tileMap, struct Player* player, VariableList* bulletList);

void LoadMapFromFile(TileMap* tileMap, const char* filePath);
void FreeMap(TileMap* tileMap);

void InitEmptyMap(TileMap* tileMap, int numRow, int numCol,float tileSize);

#endif