/*!
@file		player.h
@author		Leong Jun Xiang	(junxiang.leong)
@course		CSD1400
@brief		contains all player movement, collision , and collision response
			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/
#ifndef PLAYER_H
#define PLAYER_H
#include "../../CProcessing/inc/cprocessing.h"
#include "../Characters/Character.h"
#include "../Tiles/TileMap.h"
#include "Bullet.h"
#include "../Render/ParticleSystem.h"
typedef struct Player
{
	int isGodMode;
	int isJump;
	int isGround;
	int isDash;
	int isDead;

	int isStunned;

	int health;
	float moveSpeed;

	float dashTimer;
	float stunTimer;
	float respawnTimer;

	float lookAt;
	CP_Vector waypoint;
	CP_Vector direction;
	CP_Vector velocity;
	Character* character;
	
}Player;

void CreatePlayer(Player* object, CP_Vector position, float movespeed, int health);
void PlayerSetDirection(Player* object, CP_Vector dir);
static void PlayerAcceleration_X(Player* object, float acceleration_x, float dt);
static void PlayerAccleration_Y(Player* object, float acceleration_y, float dt);
void PlayerJump(Player* object);
void PlayerDash(Player* object);
void PlayerStopDash(Player* object);
void PlayerStunned(Player* object, float timer);

static void PlayerCheckWorldBounding(Player* object, TileMap* map);
static void PlayerCheckGround(Player* object, TileMap* map);
static void PlayerCheckCeiling(Player* object, TileMap* map);
static void PlayerCheckForward(Player* object, TileMap* map);

static void PlayerMovement(Player* object, TileMap* map, float dt);
void PlayerPhysicsUpdate(Player* object,TileMap* map, float dt);
CP_Vector PlayerGetPosition(Player* object);

//player dead
void PlayerSetDead(void* object);
static void PlayerDead(Player* object,float dt);
//collision callbacks
static void CollideWithEnemy(void* player, void* enemy);
void PlayerCollideWithBullet(Bullet* bullet, Character* player);
#endif//PLAYER_H