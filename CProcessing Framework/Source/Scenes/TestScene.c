/*!
@file		TestScene.c
@author		Daniel Chua Yee Chen (yeechendaniel.chua)
@course		CSD1400
@brief		Contains implementations for functions used for
			the Test scene.

			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/

#include "TestScene.h"
#include <stdio.h>
#include "../Characters/EnemyCircle.h"
#include "../Characters/Bullet.h"
#include "../Characters/EnemyTriangle.h"
#include "../Sound/Sound.h"
#include "WinScene.h"
#include "GameOver.h"
#include "Parallax.h"



void TestScene_Init(void)
{
	Camera_Init(&cam);

	LoadImages();

	background_ = CP_Image_Load("./Images/oldBackground.png");
	
	VariableListCreate(&g_characterList, 5);


	LoadMapFromFile(&tileMap, "./Assets/Custom.csv");
	CreatePlayer(&player, Tile_GetWorldPos(&tileMap, tileMap.startTile->row, tileMap.startTile->column), 300, 5);


	//load npcs
	
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
	Parallax_Load(&paraBackMount, IMAGE_PARA_BACK_MOUNT,0, 410, 1600, 900, 255, 0.6f, &cam);
	Parallax_Load(&paraFrontMount, IMAGE_PARA_FRONT_MOUNT,300, 410, 1600, 900, 255, 0.55f, &cam);
	Parallax_Load(&paraMist, IMAGE_PARA_MIST,300, 410, 1600, 900, 255, 0.55f, &cam);
	Parallax_Load(&paraClouds, IMAGE_PARA_CLOUDS,300, 410, 1600, 900, 255, 0.55f, &cam);
	Parallax_Load(&paraBackRocks, IMAGE_PARA_BACK_ROCKS,0, 410, 1600, 900, 255, 0.5f, &cam);
	Parallax_Load(&paraFrontRocks2, IMAGE_PARA_FRONT_ROCKS,0, 410, 1600, 900, 255, 0.5f, &cam);
	Parallax_Load(&paraBackTrees, IMAGE_PARA_FRONT_TREES,0, 410, 1600, 900, 255, 0.4f, &cam);
	Parallax_Load(&paraFrontRocks, IMAGE_PARA_FRONT_ROCKS,100, 410, 1600, 900, 255, 0.2f, &cam);
	Parallax_Load(&paraFrontTrees, IMAGE_PARA_BACK_TREES,-100, 410, 1600, 900, 255, 0.0f, &cam);

	testSceneMusic = CP_Sound_LoadMusic("./Assets/InGameMusic.mp3");
	CP_Sound_PlayAdvanced(testSceneMusic, 0.2f, 1.2f, 1, CP_SOUND_GROUP_9);
}

void TestScene_Update(void)
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


		//win condition
		if (tileMap.mapWin == true)
		{
			Transit_NextScene(&transition, MainMenu_Init, MainMenu_Update, MainMenu_Exit);
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
	Parallax_Update(&paraFrontRocks2);
	ParallaxNotMovingY_Update(&paraBackTrees);
	ParallaxNotMovingY_Update(&paraFrontRocks);
	ParallaxNotMovingY_Update(&paraFrontTrees);
	

	Camera_Update(&cam);
	TestScene_Render();

	Transit_UpdateToBig(&transition, dt);

}

void TestScene_Render(void)
{
	CP_Image_Draw(background_, 800, 450, 1600, 900, 255);

	//para
	Parallax_Render(&paraBackMount, &cam);
	Parallax_Render(&paraFrontMount, &cam);
	Parallax_Render(&paraMist, &cam);
	Parallax_Render(&paraClouds, &cam);
	Parallax_Render(&paraBackRocks, &cam);
	Parallax_Render(&paraFrontRocks2, &cam);
	Parallax_Render(&paraBackTrees, &cam);
	Parallax_Render(&paraFrontRocks, &cam);
	Parallax_Render(&paraFrontTrees, &cam);

	RenderTileMap(&cam, &tileMap);
	CharacterListRender(&g_characterList, &cam);
	BulletListRender(&g_bulletList, &cam);

	//NPC


	for (int i = 0; i < player.health; ++i)
	{
		Renderer Heart;
		Heart.alpha = 255;
		Heart.rotation = 0;
		Heart.scale = CP_Vector_Set(60, 60);
		Heart.image = IMAGE_HEART;
		Heart.position = CP_Vector_Subtract(cam.position, CP_Vector_Set((CP_System_GetWindowWidth() * 0.45f) - i * Heart.scale.x, CP_System_GetWindowHeight() * 0.45f));
		Render_Image(&cam, &Heart);
	}

	//Render UI
	CP_Settings_ResetMatrix();


}

void TestScene_Exit(void)
{
	
	CharacterListDelete(&g_characterList);
	VariableListFree(&g_bulletList);
	FreeMap(&tileMap);
	FreeSound();

	CP_Sound_Free(testSceneMusic);

}
