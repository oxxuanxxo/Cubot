/*!
@file		Stage3.h
@author		Daniel Chua Yee Chen (yeechendaniel.chua)
@course		CSD1400
@brief		Contains implementations for functions used for
			the Stage 3 scene.

			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/


#ifndef _STAGE_3_H
#define _STAGE_3_H
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

void Stage3_Init(void);
void Stage3_Update(void);
void Stage3_Render(void);
void Stage3_Exit(void);


#endif