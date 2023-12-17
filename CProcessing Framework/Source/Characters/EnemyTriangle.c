//---------------------------------------------------------
// file:	EnemyTriangle.c
// author:	Wu Pei Fang(peifang.w)
// email:	peifang.w@digipen.edu
//
// brief:	These code create the enemy triangle. 
//			Triangle will patrol around certain distance in patrol stage.
//			When player enter certain distance, triangle turn to chase stage, it will chase the player, 
//			and turn back to patrol stage when player is out of the range.
//			Triangle will die when player dash and collide with it.
//			Player will die when collided with triangle.
//			Eliminated triangle will respawn when player die.

// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#define TRI_WIDTH 60
#define TRI_HEIGHT 60
#include "cprocessing.h"
#include "EnemyTriangle.h"
#include "../Sound/Sound.h"
#include "../Render/ParticleSystem.h"
#include "EnemyCircle.h"

//initialize enemy triangle
void CreateTriangle(EnemyTriangle* triangle, CP_Vector position, TileMap* tileMap)
{
	CP_Vector collider_offset = CP_Vector_Set(0, 0);
	triangle->character = CreateCharacter(triangle, position, collider_offset, TRI_WIDTH, TRI_HEIGHT, CHARACTER_ENEMY_TRIANGLE, IMAGE_TRIANGLE);  //access character to create triangle
	triangle->mspeed = 1.0f;
	triangle->velocity = CP_Vector_Set(100.0f, 0);
	triangle->time = 0.0f;
	triangle->isDead = 0;
	triangle->timer_D = 0.5f;

	triangle->triangle_state = PATROLSTATE; //default patrol state

	AddCallBack(triangle->character, Triangle_collision);

	ParticleSystem* ps = CreateParticleSystem(30, 360, 0.2f, 900, 20, position, CP_Vector_Zero(), IMAGE_TRIANGLEPARTICLE, 200);
	ps->numParticles = 0;
	VariableListAdd(&triangle->character->particleSystemList, ps);
	AddDeadCallBack(triangle->character, Triangle_dead); //triangle death function
}

//Function fo enemy triangle position
CP_Vector Triangle_GetPosition(EnemyTriangle* triangle)
{
	return triangle->character->renderer.position;
}


//Process in PATROLSTATE
void Triangle_patrol(EnemyTriangle* triangle, Player* player, TileMap* tileMap)
{
	
	//To move character
	CP_Vector newPos = CP_Vector_Add(triangle->character->renderer.position, CP_Vector_Scale(triangle->velocity, CP_System_GetDt() * triangle->mspeed)); //position = position + (velocity * dt * mspeed);

	//To check if enemy hit wall, if yes, reverse direction
	if (IsColliding_WorldPos(tileMap, newPos.x + TRI_WIDTH * 0.5f, newPos.y) == false && IsColliding_WorldPos(tileMap, newPos.x - TRI_WIDTH * 0.5f, newPos.y) == false)//check left and right collision between triangle and wall/hole
	{
		if (IsColliding_WorldPos(tileMap, newPos.x, newPos.y - TRI_HEIGHT * 0.5f - 9.8f) == true) //check bottom(To check against the tile, -9.8 to make sure its checked) 
		{
			triangle->character->renderer.position = newPos;
		}
		else
		{
			triangle->velocity.x *= -1;
		}
	}
	else
	{
		triangle->velocity.x *= -1;
	}

	//Check distance between player and triangle
	float chardistance = CP_Vector_Distance(PlayerGetPosition(player), Triangle_GetPosition(triangle));
	
	//If player is within the triangle patrol range, triangle change to CHASESTATE
	if (chardistance < 3*tileMap->tileSize) //check against size of 3 tiles
	{
		triangle->triangle_state = CHASESTATE;
	}
}


//Process in CHASESTATE
void Triangle_chase(EnemyTriangle* triangle, Player* player, TileMap* tileMap)
{
	//Get the direction
	CP_Vector dir = CP_Vector_Subtract(PlayerGetPosition(player), Triangle_GetPosition(triangle));
	//Get direction of vector
	triangle->velocity = CP_Vector_Scale(CP_Vector_Normalize(CP_Vector_Set(dir.x, 0)), 100.0f); //velocity = player.pos - enemy.pos
	
	//To move character
	CP_Vector newPos = CP_Vector_Add(triangle->character->renderer.position, CP_Vector_Scale(triangle->velocity, CP_System_GetDt() * triangle->mspeed));

	//To check if enemy hit wall, if yes, reverse direction
	if (IsColliding_WorldPos(tileMap, newPos.x + TRI_WIDTH * 0.5f, newPos.y) == false && IsColliding_WorldPos(tileMap, newPos.x - TRI_WIDTH * 0.5f, newPos.y) == false)//check left and right collision between triangle and wall/hole
	{
		if (IsColliding_WorldPos(tileMap, newPos.x, newPos.y - TRI_HEIGHT * 0.5f - 9.8f) == true) //check bottom
		{
			triangle->character->renderer.position = newPos;
		}
	}

	//Check distance between player and triangle
	float chardistance = CP_Vector_Distance(PlayerGetPosition(player), Triangle_GetPosition(triangle));

	//If distance between player and triangle more than 3 tile size, triangle back to patrol state
	if (chardistance > 3*tileMap->tileSize)
	{
		triangle->triangle_state = PATROLSTATE;
	}
}

//Function for when triangle collide with others
void Triangle_collision(void* triangle, void* other_character) //other_character include such as player and circle
{
	Character* player = other_character; //set other_character to player

	if (player->tag != CHARACTER_ENEMY_TRIANGLE) //chceck the tag of enemy is not triangle
	{
		Character* characterTri = triangle;
		EnemyTriangle* tri;
		tri = characterTri->mainObject;
		
		if (characterTri->tag == CHARACTER_ENEMY_TRIANGLE && player->tag == CHARACTER_PLAYER)
		{
			//Get the direction
			CP_Vector dir = CP_Vector_Subtract(*CharacterGetPosition(player), *CharacterGetPosition(characterTri));
			//Get direction of vector
			tri->velocity = CP_Vector_Scale(CP_Vector_Normalize(CP_Vector_Set(dir.x, 0)), 100.0f); //velocity = player.pos - enemy.pos
		}
		
		if (player->tag == CHARACTER_PLAYER && tri->isDead == 0) //when collide, if triangle not dead, player dead
		{
			player->SetDead(player);
		}
		if (player->tag == CHARACTER_ENEMY_SPIKES)
		{
			tri->velocity.x *= -1;
		}
	}
}

//functions for enemy triangle's death
void Triangle_dead(void* triangle)
{
	Character* characterTri = triangle;
	EnemyTriangle* tri = characterTri->mainObject;
	tri->isDead = 1;
	PlaySoundEffect(SOUND_POWERUP);
	ParticleSystem* ps = VariableListGetItemAt((&tri->character->particleSystemList), 0);
	CharacterCreateParticles(ps, 10, Triangle_GetPosition(tri));	
}

//function for enemy triangle's respawn
static void Triangle_respawn(EnemyTriangle* triangle, Player* player)
{
	if (player->isDead == 1)
	{
		triangle->character->active = 1;
		triangle->isDead = 0;
		triangle->timer_D = 0.5f;
		triangle->triangle_state = PATROLSTATE;
	}
}


void Triangle_update(EnemyTriangle* triangle, Player* player, TileMap* tileMap)
{
	if (triangle->isDead == 1)
	{

		triangle->timer_D -= CP_System_GetDt();

		if (triangle->timer_D <= 0.0f)
		{
			triangle->character->active = 0;
		}

		if (player->isDead == 1)
		{
			Triangle_respawn(triangle, player);
		}
	}

	//Variable triangle access enum TRIANGLESTATE
	switch (triangle->triangle_state)
	{
	//in PATROLSTATE call patrol function
	case PATROLSTATE:
		Triangle_patrol(triangle, player, tileMap);
		break;
	//in CHASESTATE call chase function
	case CHASESTATE:
		Triangle_chase(triangle, player, tileMap);
		break;
	}

}
