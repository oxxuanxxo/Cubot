/*!
@file		Stage2.h
@author		Daniel Chua Yee Chen (yeechendaniel.chua)
@course		CSD1400
@brief		Contains implementations for functions used for
			the Stage 2 scene.

			Copyright � 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/

#ifndef _STAGE_2_H
#define _STAGE_2_H
#include "../GlobalDefine.h"
#include "../Camera/Camera.h"
#include "../Tiles/TileMap.h"
#include "../Characters/Player.h"
#include "../Render/RenderUtil.h"
#include "../Render/ParticleSystem.h"
#include "../Characters/EnemyTurret.h"
#include "../Characters/NPC.h"
#include "../Dialogue/DialogueBox.h"
#include "../Characters/EnemySpikes.h"
#include "Transition.h"
#include "../Characters/EnemyCircle.h"
#include "../Characters/Bullet.h"
#include "../Characters/EnemyTriangle.h"
#include "../Characters/EnemyBounce.h"
#include "Parallax.h"

Camera cam;
Player player;

TileMap tileMap;

ParallaxImage paraBackMount;
ParallaxImage paraFrontMount;
ParallaxImage paraMist;
ParallaxImage paraClouds;
ParallaxImage paraBackTrees;
ParallaxImage paraFrontTrees;
ParallaxImage paraBackRocks;
ParallaxImage paraFrontRocks;

CP_Sound bgm;

NPC* npc1;
NPC* npc2;

//transition
Transition transition;

CP_Image background_;

static VariableList g_bulletList;

void Stage2_Init(void);
void Stage2_Update(void);
void Stage2_Render(void);
void Stage2_Exit(void);


#endif