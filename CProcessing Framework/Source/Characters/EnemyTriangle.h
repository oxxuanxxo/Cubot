//---------------------------------------------------------
// file:	EnemyTriangle.h
// author:	Wu Pei Fang(peifang.w)
// email:	peifang.w@digipen.edu
//
// brief:	This file include functions for EnemyTriangle.c

// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#ifndef ENEMYTRIANGLE_H
#define ENEMYTRIANGLE_H
#include "../Characters/Character.h"
#include "../Render/ParticleSystem.h"
#include "cprocessing.h"
#include "../Tiles/TileMap.h"
#include "Player.h"

//to store the current state of the state machine for Enemy Triangle
typedef enum TRIANGLE_STATE 
{
	PATROLSTATE,
	CHASESTATE,
	IDLESTATE,
}TRIANGLE_STATE;

typedef struct EnemyTriangle
{
	TRIANGLE_STATE triangle_state;
	Character* character; //character pointer
	BoxCollider collider;
	TileMap* tileMap;
	CP_Vector velocity;

	float mspeed, time, timer_D;
	int isDead;

}EnemyTriangle;

//Decalare  function
void CreateTriangle(EnemyTriangle* triangle, CP_Vector position, TileMap* tileMap);
CP_Vector Triangle_GetPosition(EnemyTriangle* triangle);

void Triangle_patrol(EnemyTriangle* triangle, Player* player, TileMap* tileMap);
void Triangle_chase(EnemyTriangle* triangle, Player* player, TileMap* tileMap);
void Triangle_update(EnemyTriangle* triangle, Player* player, TileMap* tileMap);


void Triangle_collision(void* triangle, void* other_character);
void Triangle_dead(void* triangle);
static void Triangle_respawn(EnemyTriangle* triangle, Player* player);

#endif



