/*!
@file		TutorialScene.c
@author		Daniel Chua Yee Chen (yeechendaniel.chua)
@course		CSD1400
@brief		Contains implementations for functions used for
			the Tutorial scene.

			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/


#include "TutorialScene.h"
#include "Stage1.h"
#include <stdio.h>
#include "../Characters/EnemyCircle.h"
#include "../Characters/Bullet.h"
#include "../Characters/EnemyTriangle.h"
#include "../Sound/Sound.h"
#include "WinScene.h"
#include "GameOver.h"
#include "Parallax.h"



void TutorialScene_Init(void)
{
	currentStage = STAGE_TUTORIAL;

	Camera_Init(&cam);

	LoadImages();

	background_ = CP_Image_Load("./Images/Parallax/Background.png");

	VariableListCreate(&g_characterList, 5);


	LoadMapFromFile(&tileMap, "./Assets/Tutorial.csv");
	CreatePlayer(&player, CP_Vector_Set(60.f, 60.f), 300, 5);
	player.character->renderer.position = Tile_GetWorldPos(&tileMap, tileMap.startTile->row, tileMap.startTile->column);


	//load npcs
	npc1 = NPC_Create(Tile_GetWorldPos(&tileMap, 4, 2), CP_Vector_Set(60.0f, 60.0f), IMAGE_INFO, "./Assets/instruction1.txt", 60.0f * 4.0f);
	npc2 = NPC_Create(Tile_GetWorldPos(&tileMap, 4, 20), CP_Vector_Set(60.0f, 60.0f), IMAGE_INFO, "./Assets/instruction2.txt", 60.0f * 4.0f);
	npc3 = NPC_Create(Tile_GetWorldPos(&tileMap, 4, 37), CP_Vector_Set(60.0f, 60.0f), IMAGE_INFO, "./Assets/instruction3.txt", 60.0f * 4.0f);
	npc4 = NPC_Create(Tile_GetWorldPos(&tileMap, 20, 95), CP_Vector_Set(60.0f, 60.0f), IMAGE_INFO, "./Assets/instruction4.txt", 60.0f * 4.0f);

	//bullet
	VariableListCreate(&g_bulletList, 10);

	//sound
	SoundLoad();

	//transit
	Transit_Create(&transition);
	transition.scale = 1;
	transition.isTransitingToBig = true;
	setSpeed(&transition, -1);

	//Parallax
	Parallax_Load(&paraBackMount, IMAGE_PARA_BACK_MOUNT, 0, 410, 1600, 900, 255, 0.8f, &cam);
	Parallax_Load(&paraFrontMount, IMAGE_PARA_FRONT_MOUNT, 300, 410, 1600, 900, 255, 0.55f, &cam);
	Parallax_Load(&paraMist, IMAGE_PARA_MIST, 300, 410, 1600, 900, 255, 0.55f, &cam);
	Parallax_Load(&paraClouds, IMAGE_PARA_CLOUDS, 300, 410, 1600, 900, 255, 0.55f, &cam);
	Parallax_Load(&paraBackRocks, IMAGE_PARA_BACK_ROCKS, 0, 410, 1600, 900, 255, 0.5f, &cam);
	Parallax_Load(&paraBackTrees, IMAGE_PARA_FRONT_TREES, 0, 410, 1600, 900, 255, 0.4f, &cam);
	Parallax_Load(&paraFrontRocks, IMAGE_PARA_FRONT_ROCKS, 100, 410, 1600, 900, 255, 0.2f, &cam);
	Parallax_Load(&paraFrontTrees, IMAGE_PARA_BACK_TREES, -100, 410, 1600, 900, 255, 0.0f, &cam);

	bgm = CP_Sound_LoadMusic("./Assets/InGameMusic.mp3");
	CP_Sound_PlayAdvanced(bgm, 0.2f, 1.2f, 1, CP_SOUND_GROUP_1);

}

void TutorialScene_Update(void)
{
	CP_System_SetWindowTitle("CUBOT");

	float dt = CP_System_GetDt();

	if (transition.isTransitingToBig == false)
	{
		BulletListCheckCollisionAgainst(&g_bulletList, player.character, dt, PlayerCollideWithBullet);
		CharacterListCollision(&g_characterList);

		BulletListUpdate(&g_bulletList, dt);

		UpdateTileMap(&tileMap, &player, &g_bulletList);


		PlayerPhysicsUpdate(&player, &tileMap, dt);
		//NPC
		NPC_Update(npc1, &player);
		NPC_Update(npc2, &player);
		NPC_Update(npc3, &player);
		NPC_Update(npc4, &player);

		//win condition
		if (tileMap.mapWin == true)
		{
			Transit_NextScene(&transition, Stage1_Init, Stage1_Update, Stage1_Exit);
		}

		if (CP_Input_KeyTriggered(KEY_ESCAPE))
		{
			Transit_NextScene(&transition, MainMenu_Init, MainMenu_Update, MainMenu_Exit);
		}



	}

	//Camera movement
	cam.position = player.character->renderer.position;
	Camera_Bounds(&cam,
		CP_Vector_Set(CP_System_GetWindowWidth() * 0.5f - tileMap.tileSize * 0.5f,
			CP_System_GetWindowHeight() * 0.5f - tileMap.tileSize * 0.5f),
		CP_Vector_Set(
			tileMap.tileSize * (tileMap.numCol - 1) - (CP_System_GetWindowWidth() * 0.5f),
			tileMap.tileSize * (tileMap.numRow - 1) - (CP_System_GetWindowHeight() * 0.5f)));

	//Parallax update
	Parallax_Update(&paraBackMount);
	Parallax_Update(&paraFrontMount);
	ParallaxNotMovingY_Update(&paraMist);
	Parallax_Update(&paraClouds);
	Parallax_Update(&paraBackRocks);
	ParallaxNotMovingY_Update(&paraBackTrees);
	ParallaxNotMovingY_Update(&paraFrontRocks);
	ParallaxNotMovingY_Update(&paraFrontTrees);


	Camera_Update(&cam);

	TutorialScene_Render();
	Transit_UpdateToBig(&transition, dt);

}

void TutorialScene_Render(void)
{
	CP_Image_Draw(background_, 800, 450, 1600, 900, 255);

	//para
	Parallax_Render(&paraBackMount, &cam);
	Parallax_Render(&paraFrontMount, &cam);
	Parallax_Render(&paraMist, &cam);
	Parallax_Render(&paraClouds, &cam);
	Parallax_Render(&paraBackRocks, &cam);
	Parallax_Render(&paraBackTrees, &cam);
	Parallax_Render(&paraFrontRocks, &cam);
	Parallax_Render(&paraFrontTrees, &cam);

	RenderTileMap(&cam, &tileMap);
	CharacterListRender(&g_characterList, &cam);
	BulletListRender(&g_bulletList, &cam);

	//NPC
	NPC_Render(npc1, &cam);
	NPC_Render(npc2, &cam);
	NPC_Render(npc3, &cam);
	NPC_Render(npc4, &cam);

	for (int i = 0; i < player.health; ++i)
	{
		Renderer Heart;
		Heart.alpha = 255;
		Heart.rotation = 0;
		Heart.scale = CP_Vector_Set(60, 60);
		Heart.image = IMAGE_HEART;
		Heart.position = CP_Vector_Subtract(cam.position, CP_Vector_Set((CP_System_GetWindowWidth() * 0.45f) - i * Heart.scale.x, CP_System_GetWindowHeight() * 0.47f));
		Render_Image(&cam, &Heart);
	}

	//Render UI
	CP_Settings_ResetMatrix();


}

void TutorialScene_Exit(void)
{

	CharacterListDelete(&g_characterList);
	VariableListFree(&g_bulletList);
	FreeMap(&tileMap);
	FreeSound();

	NPC_Destroy(npc1);
	NPC_Destroy(npc2);
	NPC_Destroy(npc3);
	NPC_Destroy(npc4);

	CP_Sound_Free(bgm);
}
