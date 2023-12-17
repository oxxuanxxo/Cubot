/*!
@file		TileMap.c
@author		Daniel Chua Yee Chen (yeechendaniel.chua)
@course		CSD1400
@brief		Contains implementations for functions used in loading 
			and interacting with tile map.

			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/

#include "TileMap.h"
#include "../Render/RenderUtil.h"
#include "../Characters/EnemyTriangle.h"
#include "../Characters/EnemyCircle.h"
#include "../Characters/Player.h"
#include "../Characters/EnemyTurret.h"
#include "../Characters/EnemySpikes.h"
#include "../Characters/EnemyBounce.h"
#include "../GlobalDefine.h"
#include <cprocessing.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Tile* GetTile_RowCol(TileMap* tileMap, int row, int col)
{
	if (tileMap->map) // check that map is initialized
	{
		if (tileMap->numRow > row && tileMap->numCol > col && row>=0 && col >= 0) //out of bound check
		{
			return &(tileMap->map[row][col]);
		}
	}
	return NULL;
}

Tile* GetTile_WorldPos(TileMap* tileMap, float x, float y)
{
	if (tileMap->map)
	{
		x += tileMap->tileSize * 0.5f;
		y += tileMap->tileSize * 0.5f;

		return GetTile_RowCol(tileMap,  (int)(y / tileMap->tileSize) , (int)(x / tileMap->tileSize));

	}

	return NULL;
}

CP_Vector Tile_GetWorldPos(TileMap* tileMap, int row, int col)
{
	if (tileMap->map) // check that map is initialized
	{
		if (tileMap->numRow > row && tileMap->numCol > col) //out of bound check
		{
			return CP_Vector_Set(col * tileMap->tileSize,  row * tileMap->tileSize);
		}
	}
	return CP_Vector_Set(-1, -1);
}

bool IsColliding_WorldPos(TileMap* tileMap, float x, float y)
{
	Tile* temp = GetTile_WorldPos(tileMap,x, y);

	return IsColliding_Tile(temp);
}

bool IsColliding_Tile(Tile* tile)
{
	if (tile == NULL)
	{
		return false;
	}

	return (tile->tileType == TILE_BLOCK || tile->tileType == TILE_BOUNCE);
}

bool IsColliding(TileMap* tileMap, Tile* tile, CP_Vector pos)
{
	CP_Vector tilePos = Tile_GetWorldPos(tileMap, tile->row, tile->column);

	float diffx = pos.x - tilePos.x;
	if (diffx < 0)
	{
		diffx *= -1;
	}
	float diffy = pos.y - tilePos.y;
	if (diffy < 0)
	{
		diffy *= -1;
	}

	return (diffx <= tileMap->tileSize && diffy <= tileMap->tileSize);

}

void SetTileType(TileMap* tileMap, Tile* tile, const char* string)
{
	
	switch (string[0])
	{
	case '1':
		tile->tileType = TILE_BLOCK;
		tile->sprite.image = IMAGE_TILE1;
		tile->sprite.alpha = 255;
		break;
	case ' ':
		tile->tileType = TILE_EMPTY;
		tile->sprite.image = IMAGE_TILE1;
		tile->sprite.alpha = 0;
		break;
	case '2':
	{
		tile->tileType = TILE_BLOCK;
		tile->sprite.image = IMAGE_TILE2;
		tile->sprite.alpha = 255;
		break;
	}
	case '3':
	{
		tile->tileType = TILE_EMPTY;
		EnemyTriangle* triangle  = (EnemyTriangle*)malloc(sizeof(EnemyTriangle));
		CreateTriangle(triangle, Tile_GetWorldPos(tileMap,tile->row,tile->column), tileMap);
		VariableListAdd(&(tileMap->triangleList) , triangle);

		break;
	}
	case '4':
	{
		tile->tileType = TILE_EMPTY;
		EnemyCircle* enemy = (EnemyCircle*)malloc(sizeof(EnemyCircle));
		CircleEnemy_Create(enemy, Tile_GetWorldPos(tileMap, tile->row, tile->column), CP_Vector_Set(0,0));
		VariableListAdd(&(tileMap->circleList), enemy);

		break;
	}
	case '5':
	{
		tile->tileType = TILE_EMPTY;
		EnemyTurret* enemy = (EnemyTurret*)malloc(sizeof(EnemyTurret));
		CreateEnemyTurret(enemy, Tile_GetWorldPos(tileMap, tile->row, tile->column));
		VariableListAdd(&(tileMap->turretList), enemy);

		break;
	}
	case '6':
	{
		tile->tileType = TILE_EMPTY;
		EnemySpikes* enemy = (EnemySpikes*)malloc(sizeof(EnemySpikes));
		CreateEnemySpikes(enemy, Tile_GetWorldPos(tileMap, tile->row, tile->column));
		VariableListAdd(&(tileMap->spikeList), enemy);

		break;
	}
	case '7':
	{
		tile->tileType = TILE_BOUNCE;
		EnemyBounce* enemy = (EnemyBounce*)malloc(sizeof(EnemyBounce));
		CreateEnemyBounce(enemy, Tile_GetWorldPos(tileMap, tile->row, tile->column));
		VariableListAdd(&(tileMap->bounceList), enemy);

		break;
	}
	case '8':
	{
		tile->tileType = TILE_START;
		tileMap->startTile = tile;
		tile->sprite.image = IMAGE_TILE_START;
		tile->sprite.alpha = 255;
		break;
	}
	case '9':
	{
		tile->tileType = TILE_END;
		tileMap->endTile = tile;
		tile->sprite.image = IMAGE_TILE_END;
		tile->sprite.alpha = 255;
		break;
	}
	default:
		tile->tileType = TILE_EMPTY;
		break;
	}
}

void RenderTileMap(const Camera* cam, const TileMap* tileMap)
{
	if (tileMap->map) // check that map is initialized
	{
		for (int r = 0; r < tileMap->numRow; ++r)
		{
			for (int c = 0; c < tileMap->numCol; ++c)
			{

				if (GetTile_RowCol(tileMap, r, c)->tileType != TILE_EMPTY)
				{
					if (GetTile_RowCol(tileMap, r, c)->sprite.image >0 && GetTile_RowCol(tileMap, r, c)->sprite.image < MAX_IMAGE)
					{
						Render_Image(cam, &(GetTile_RowCol(tileMap, r, c)->sprite));
					}
					
				}
			}
		}
	}
}


void LoadMapFromFile(TileMap* tileMap, const char* filePath)
{
	FILE* file;
	fopen_s(&file, filePath, "r");
	if (file)
	{
		fseek(file, 0, SEEK_SET);
		printf("success file\n");
		char buffer[1000] = ""; // buffer to read each line.
		if (fgets(buffer, 1000, file) == NULL) // ensure file is not empty
		{
			return;
		}
		char* context = NULL;
		char* ptr = NULL;
		ptr = strtok_s(buffer,",",&context);
		//ptr = strtok_s(NULL, ",", &context);
		tileMap->numRow = atoi(ptr);
		ptr = strtok_s(NULL, ",", &context);
		tileMap->numCol = atoi(ptr);
		ptr = strtok_s(NULL, ",", &context);
		tileMap->tileSize = (float)atof(ptr);

		VariableListCreate(&(tileMap->triangleList), 5);
		VariableListCreate(&(tileMap->circleList), 5);
		VariableListCreate(&(tileMap->turretList), 5);

		tileMap->mapWin = false;
		tileMap->startTile = NULL;
		tileMap->endTile = NULL;

		// creating the dynamic array
		if (tileMap->numRow > 0 && tileMap->numCol > 0) // ensure that malloc will be a positive size
		{
			tileMap->map = (Tile**)malloc(tileMap->numRow * sizeof(Tile*));
			if (tileMap->map) //null pointer guard to prevent warnings
			{
				for (int i = 0; i < tileMap->numRow; ++i)
				{
					tileMap->map[i] = (Tile*)malloc(tileMap->numCol * sizeof(Tile));

				}// end of dynamic memory allocation

				Tile* temp;
				//this loop handles the assigning of values.
				for (int r = tileMap->numRow -1; r >=0; --r) // set the first row as the bottom row in the csv file. hence as row increases, y increases.
				{
					if (fgets(buffer, 1000, file) == NULL) // guards EOF
					{
						break;
					}
					ptr = strtok_s(buffer, ",", &context);
					if (ptr != NULL)
					{
						temp = GetTile_RowCol(tileMap, r, 0);
						
						temp->row = r;
						temp->column = 0;
						SetTileType(tileMap, temp, ptr);

						temp->sprite.position =	Tile_GetWorldPos(tileMap, temp->row, temp->column);
						temp->sprite.scale = CP_Vector_Set(tileMap->tileSize, tileMap->tileSize);
						temp->sprite.rotation = 0.0f;

					}
					for (int c = 1; c < tileMap->numCol; ++c)
					{
						ptr = strtok_s(NULL, ",", &context);
						if (ptr != NULL)
						{
							temp = GetTile_RowCol(tileMap, r, c);

							
							temp->row = r;
							temp->column = c;
							SetTileType(tileMap, temp, ptr);

							temp->sprite.position = Tile_GetWorldPos(tileMap, temp->row, temp->column);
							temp->sprite.scale = CP_Vector_Set(tileMap->tileSize, tileMap->tileSize);
							temp->sprite.rotation = 0.0f;

						}//end if
					}//end column loop
				}//end row loop
			}//end if (tileMap->map)
		}
		
		fclose(file);
	}
}

void UpdateTileMap(TileMap* tileMap, Player* player, VariableList* bulletList)
{
	for (int i = 0; i < tileMap->triangleList.length; ++i)
	{
		Triangle_update((VariableListGetItemAt(&tileMap->triangleList, i)) , player, tileMap);
	}

	for (int i = 0; i < tileMap->circleList.length; ++i)
	{
		CircleEnemy_Update((VariableListGetItemAt(&tileMap->circleList, i)), player, tileMap,CP_System_GetDt());
	}
	for (int i = 0; i < tileMap->turretList.length; ++i)
	{
		EnemyTurretUpdate(bulletList,(VariableListGetItemAt(&tileMap->turretList, i)), player, CP_System_GetDt());
	}
	if (IsColliding(tileMap, tileMap->endTile, *CharacterGetPosition(player->character)))
	{
		tileMap->mapWin = true;
	}

	if (CP_Input_KeyTriggered(KEY_R))
	{
		tileMap->mapWin = true;
	}
}

void FreeMap(TileMap* tileMap)
{
	//clearing npc variable lists
	VariableListFree(&tileMap->triangleList);
	VariableListFree(&tileMap->circleList);
	VariableListFree(&tileMap->turretList);

	if (tileMap->map)
	{
		for (int i = 0; i < tileMap->numRow; ++i)
		{
			if (tileMap->map[i])
			{
				free(tileMap->map[i]);
			}
		}
		free(tileMap->map);
	}
}

void InitEmptyMap(TileMap* tileMap, int numRow, int numCol, float tileSize)
{
	tileMap->numRow = numRow;
	tileMap->numCol = numCol;
	tileMap->tileSize = tileSize;
	tileMap->mapWin = false;
	tileMap->startTile = NULL;
	tileMap->endTile = NULL;

	VariableListCreate(&(tileMap->triangleList), 5);
	VariableListCreate(&(tileMap->circleList), 5);
	VariableListCreate(&(tileMap->turretList), 5);

	if (tileMap->numRow > 0 && tileMap->numCol > 0) // ensure that malloc will be a positive size
	{
		tileMap->map = (Tile**)malloc(tileMap->numRow * sizeof(Tile*));
		if (tileMap->map) //null pointer guard to prevent warnings
		{
			for (int i = 0; i < tileMap->numRow; ++i)
			{
				tileMap->map[i] = (Tile*)malloc(tileMap->numCol * sizeof(Tile));

			}// end of dynamic memory allocation

			Tile* temp;
			//this loop handles the assigning of values.
			for (int r = tileMap->numRow - 1; r >= 0; --r) // set the first row as the bottom row in the csv file. hence as row increases, y increases.
			{

				temp = GetTile_RowCol(tileMap, r, 0);

				temp->row = r;
				temp->column = 0;
				SetTileType(tileMap, temp, " ");

				temp->sprite.position = Tile_GetWorldPos(tileMap, temp->row, temp->column);
				temp->sprite.scale = CP_Vector_Set(tileMap->tileSize, tileMap->tileSize);
				temp->sprite.rotation = 0.0f;
				if (r == 0)
				{
					SetTileType(tileMap, temp, "1");
				}
				for (int c = 1; c < tileMap->numCol; ++c)
				{
					temp = GetTile_RowCol(tileMap, r, c);

					temp->row = r;
					temp->column = c;
					SetTileType(tileMap, temp, " ");

					temp->sprite.position = Tile_GetWorldPos(tileMap, temp->row, temp->column);
					temp->sprite.scale = CP_Vector_Set(tileMap->tileSize, tileMap->tileSize);
					temp->sprite.rotation = 0.0f;
					if (r == 0)
					{
						SetTileType(tileMap, temp, "1");
					}
				}//end column loop

				
				
			}//end row loop
		}//end if (tileMap->map)
	}

	Tile* tile = GetTile_RowCol(tileMap, 1, 1);
	tileMap->startTile = tile;
	tile->tileType = TILE_START;
	tile->sprite.image = IMAGE_TILE_START;
	tile->sprite.alpha = 255;

	tile = GetTile_RowCol(tileMap, 1, 3);
	tileMap->endTile = tile;
	tile->tileType = TILE_END;
	tile->sprite.image = IMAGE_TILE_END;
	tile->sprite.alpha = 255;
}