/*!
@file		MapEditor.h
@author		Daniel Chua Yee Chen (yeechendaniel.chua)
@course		CSD1400
@brief		Contains implementations for functions used for
			the map editor scene.

			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/

#include "MapEditor.h"
#include "../Render/RenderUtil.h"
#include "../Characters/Character.h"
#include "MainMenu.h"
#include <stdio.h>
#include <string.h>

void MapEditor_Init(void)
{
	Camera_Init(&cam);

	LoadImages();
	background_ = CP_Image_Load("./Images/oldBackground.png");

	InitEmptyMap(&tileMap, 100, 100, 60);

	selectionAlpha = 125;

	currRow = 0;
	currCol = 0;

	legend.image = IMAGE_LEGEND;
	legend.rotation = 0;
	legend.alpha = 255;
	legend.scale = CP_Vector_Set(CP_System_GetDisplayWidth() * 0.25f, CP_System_GetDisplayHeight() * 0.2f);


	currentSelection = SELECT_BLOCK;
	selection.position = Tile_GetWorldPos(&tileMap, currRow, currCol);
	selection.rotation = 0;
	selection.alpha = selectionAlpha;
	selection.image = IMAGE_TILE1;
	selection.scale = CP_Vector_Set(tileMap.tileSize, tileMap.tileSize);

	sbg.position = selection.position;
	sbg.rotation = 0;
	sbg.alpha = 255;
	sbg.image = IMAGE_MAPEDITOR_CURSOR;
	sbg.scale = CP_Vector_Set(tileMap.tileSize *1.0f, tileMap.tileSize*1.0f);

	debounceTimer = 0.0f;

	//transit
	Transit_Create(&transition);
	transition.scale = 1;
	transition.isTransitingToBig = true;
	setSpeed(&transition, -1);
}

static void SelectionChangesInputs(void)
{

	if (CP_Input_KeyTriggered(KEY_1))
	{
		selection.image = IMAGE_TILE1;
		selection.alpha = selectionAlpha;
		currentSelection = SELECT_BLOCK;
	}
	if (CP_Input_KeyTriggered(KEY_2))
	{
		selection.image = IMAGE_TILE2;
		selection.alpha = selectionAlpha;
		currentSelection = SELECT_DIRT;
	}
	if (CP_Input_KeyTriggered(KEY_3))
	{
		selection.image = IMAGE_TRIANGLE;
		selection.alpha = selectionAlpha;
		currentSelection = SELECT_TRIANGLE;
	}
	if (CP_Input_KeyTriggered(KEY_4))
	{
		selection.image = IMAGE_CIRCLE;
		selection.alpha = selectionAlpha;
		currentSelection = SELECT_CIRCLE;
	}
	if (CP_Input_KeyTriggered(KEY_5))
	{
		selection.image = IMAGE_TURRET;
		selection.alpha = selectionAlpha;
		currentSelection = SELECT_TURRET;
	}
	if (CP_Input_KeyTriggered(KEY_6))
	{
		selection.image = IMAGE_SPIKE;
		selection.alpha = selectionAlpha;
		currentSelection = SELECT_SPIKE;
	}
	if (CP_Input_KeyTriggered(KEY_7))
	{
		selection.image = IMAGE_SLIME;
		selection.alpha = selectionAlpha;
		currentSelection = SELECT_BOUNCE;
	}
	if (CP_Input_KeyTriggered(KEY_8))
	{
		selection.image = IMAGE_TILE_START;
		selection.alpha = selectionAlpha;
		currentSelection = SELECT_START;
	}
	if (CP_Input_KeyTriggered(KEY_9))
	{
		selection.image = IMAGE_TILE_END;
		selection.alpha = selectionAlpha;
		currentSelection = SELECT_END;
	}
	if (CP_Input_KeyTriggered(KEY_0))
	{
		selection.image = IMAGE_TILE1;
		selection.alpha = 0;
		currentSelection = SELECT_EMPTY;
	}
}


static void SelectionMovement(void)
{
	if ( (CP_Input_KeyDown(KEY_W) || CP_Input_KeyDown(KEY_UP))&& debounceTimer >= 0.1f)
	{
		debounceTimer = 0.0f;
		++currRow;
		if (currRow >= tileMap.numRow)
		{
			currRow = tileMap.numRow - 1;
		}
	}
	if ((CP_Input_KeyDown(KEY_S) || CP_Input_KeyDown(KEY_DOWN)) && debounceTimer >= 0.1f)
	{
		debounceTimer = 0.0f;
		--currRow;
		if (currRow < 0)
		{
			currRow = 0;
		}
	}
	if ((CP_Input_KeyDown(KEY_A) || CP_Input_KeyDown(KEY_LEFT)) && debounceTimer >= 0.1f)
	{
		debounceTimer = 0.0f;
		--currCol;
		if (currCol < 0)
		{
			currCol = 0;
		}
	}

	if ((CP_Input_KeyDown(KEY_D) || CP_Input_KeyDown(KEY_RIGHT)) && debounceTimer >= 0.1f)
	{
		debounceTimer = 0.0f;
		++currCol;
		if (currCol >= tileMap.numCol)
		{
			currCol = tileMap.numCol - 1;
		}
	}

	if (CP_Input_KeyDown(KEY_SPACE))
	{
		Tile* tile = GetTile_RowCol(&tileMap, currRow, currCol);

		if (tile->tileType != TILE_START && tile->tileType != TILE_END)
		{
			switch (currentSelection)
			{
				case SELECT_EMPTY:
				{
					tile->tileType = TILE_EMPTY;
					tile->sprite.alpha = 0;
					break;
				}
				case SELECT_BLOCK:
				{
					tile->tileType = TILE_BLOCK;
					tile->sprite.image = IMAGE_TILE1;
					tile->sprite.alpha = 255;
					break;
				}
				case SELECT_DIRT:
				{
					tile->tileType = TILE_DIRT;
					tile->sprite.image = IMAGE_TILE2;
					tile->sprite.alpha = 255;
					break;
				}
				case SELECT_TRIANGLE:
				{
					tile->tileType = TILE_TRIANGLE;
					tile->sprite.image = IMAGE_TRIANGLE;
					tile->sprite.alpha = 255;
					break;
				}
				case SELECT_CIRCLE:
				{
					tile->tileType = TILE_CIRCLE;
					tile->sprite.image = IMAGE_CIRCLE;
					tile->sprite.alpha = 255;
					break;
				}
				case SELECT_TURRET:
				{
					tile->tileType = TILE_TURRET;
					tile->sprite.image = IMAGE_TURRET;
					tile->sprite.alpha = 255;
					break;
				}
				case SELECT_SPIKE:
				{
					tile->tileType = TILE_SPIKES;
					tile->sprite.image = IMAGE_SPIKE;
					tile->sprite.alpha = 255;
					break;
				}
				case SELECT_BOUNCE:
				{
					tile->tileType = TILE_BOUNCE;
					tile->sprite.image = IMAGE_SLIME;
					tile->sprite.alpha = 255;
					break;
				}
				case SELECT_START:
				{
					tile->tileType = TILE_START;
					tile->sprite.image = IMAGE_TILE_START;
					tile->sprite.alpha = 255;

					tileMap.startTile->tileType = TILE_EMPTY;
					tileMap.startTile->sprite.image = IMAGE_TILE1;
					tileMap.startTile->sprite.alpha = 0;

					tileMap.startTile = tile;
					break;
				}
				case SELECT_END:
				{
					tile->tileType = TILE_END;
					tile->sprite.image = IMAGE_TILE_END;
					tile->sprite.alpha = 255;

					tileMap.endTile->tileType = TILE_EMPTY;
					tileMap.endTile->sprite.image = IMAGE_TILE1;
					tileMap.endTile->sprite.alpha = 0;
							
					tileMap.endTile = tile;
					break;
				}
			}
		}
		
	}
}


static void PrintTileType(FILE* file, TILE_TYPE type)
{
	switch (type)
	{
		case TILE_EMPTY:
		{
			fprintf(file, " ");
			break;
		}
		case TILE_BLOCK:
		{
			fprintf(file, "1");
			break;
		}
		case TILE_DIRT:
		{
			fprintf(file, "2");
			break;
		}
		case TILE_TRIANGLE:
		{
			fprintf(file, "3");
			break;
		}
		case TILE_CIRCLE:
		{
			fprintf(file, "4");
			break;
		}
		case TILE_TURRET:
		{
			fprintf(file, "5");
			break;
		}
		case TILE_SPIKES:
		{
			fprintf(file, "6");
			break;
		}
		case TILE_BOUNCE:
		{
			fprintf(file, "7");
			break;
		}
		case TILE_START:
		{
			fprintf(file, "8");
			break;
		}
		case TILE_END:
		{
			fprintf(file, "9");
			break;
		}
		default:
		{
			fprintf(file, " ");
			break;
		}
	}
}

static void FinishEdit(void)
{
	FILE* file;
	fopen_s(&file, "./Assets/Custom.csv", "w");

	if (file)
	{
		//char buffer[1000] = {0};
		//writes first row of.
		fseek(file, 0, SEEK_SET);
		fprintf(file,"%d,%d,%d",tileMap.numRow,tileMap.numCol, (int)tileMap.tileSize);
		for (int i = 0; i < tileMap.numCol - 3; ++i)
		{
			fprintf(file,",");
		}
		fprintf(file,"\n");

		for (int r = tileMap.numRow - 1; r >= 0; --r)
		{
			for (int c = 0; c < tileMap.numCol; ++c)
			{
				if (tileMap.map)
				{
					if (c == tileMap.numCol - 1)
					{
						PrintTileType(file, tileMap.map[r][c].tileType);
					}
					else
					{
						PrintTileType(file, tileMap.map[r][c].tileType);
						fprintf(file, ",");
					}
				}
			}
			fprintf(file, "\n");
		}

		fclose(file);
	}

}

void MapEditor_Update(void)
{
	debounceTimer += CP_System_GetDt();
	if (debounceTimer < 0)
	{
		debounceTimer = 0;
	}

	if (CP_Input_KeyTriggered(KEY_ENTER))
	{
		FinishEdit();
		Transit_NextScene(&transition, MainMenu_Init, MainMenu_Update, MainMenu_Exit);
	}

	if (CP_Input_KeyTriggered(KEY_ESCAPE))
	{
		Transit_NextScene(&transition, MainMenu_Init, MainMenu_Update, MainMenu_Exit);
	}

	SelectionMovement();
	SelectionChangesInputs();

	selection.position = Tile_GetWorldPos(&tileMap, currRow, currCol);
	sbg.position = selection.position;
	cam.position = selection.position;
	Camera_Bounds(&cam,
		CP_Vector_Set(CP_System_GetWindowWidth() * 0.5f - tileMap.tileSize * 0.5f,
			CP_System_GetWindowHeight() * 0.5f - tileMap.tileSize * 0.5f),
		CP_Vector_Set(
			tileMap.tileSize * (tileMap.numCol - 1) - (CP_System_GetWindowWidth() * 0.5f),
			tileMap.tileSize * (tileMap.numRow - 1) - (CP_System_GetWindowHeight() * 0.5f)));

	Camera_Update(&cam);

	if (CP_Input_KeyTriggered(KEY_N))
	{
		if (legend.alpha > 0)
		{
			legend.alpha = 0;
		}
		else
		{
			legend.alpha = 255;
		}
	}


	legend.position = CP_Vector_Add(
		cam.position,
		CP_Vector_Set(CP_System_GetDisplayWidth() * 0.22f, 0)
	);

	MapEditor_Render();

	Transit_UpdateToBig(&transition, CP_System_GetDt());
}

void MapEditor_Render(void)
{
	CP_Image_Draw(background_, 800, 450, 1600, 900, 255);

	RenderTileMap(&cam, &tileMap);


	Render_Image(&cam, &legend);

	
	Render_Image(&cam, &selection);
	Render_Image(&cam, &sbg);

	CP_Settings_ResetMatrix();

	CP_Settings_TextSize(40.0f);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	char string[50];

	sprintf_s(string, 50, "Current Row:%d Current Column: %d", currRow,currCol);
	CP_Font_DrawText(string, CP_System_GetWindowWidth() * 0.05f, CP_System_GetWindowHeight() * 0.05f);
}

void MapEditor_Exit(void)
{
	//CharacterListDelete(&g_characterList);
	FreeMap(&tileMap);
	
}