/*!
@file       EnemyCircle.c
@author     Chua Yip Xuan (yipxuan.chua)
@course     CSD1400
@section    PASCAL-AM
@brief      This file consists of functions that will create a circle enemy
			The circle enemy's AI (finite state machine) will go from idling state to charging state after time intervals or when player is near.
			Visual cue (alert cue) is implemented 1.5s before it charges, as many feedbacks in the user testing stated that it was too sudden.

			Copyright © 2020 DigiPen, All rights reserved.
*/
/*__________________________________________________________________________*/

#include "EnemyCircle.h"
#include "../Sound/Sound.h"
#include <math.h>
#define CIRCLE_WIDTH 60 //(player1->character_cir->renderer.scale.x)
#define CIRCLE_HEIGHT 60 //(player1->character_cir.renderer.scale.y)

//create enemy
//create circle enemy and set initial states
void CircleEnemy_Create(EnemyCircle* enemy_cir, CP_Vector position_cir, CP_Vector collOffSet)
{
	enemy_cir->character_cir = CreateCharacter(enemy_cir, position_cir, CP_Vector_Zero(), CIRCLE_WIDTH, CIRCLE_HEIGHT, CHARACTER_ENEMY_CIRCLE, IMAGE_CIRCLE);
	enemy_cir->character_cir->active = 1;
	enemy_cir->isDead = 0;
	enemy_cir->vel_cir = CP_Vector_Set(100.0f, 0.0f);
	enemy_cir->timer_idle = 0.0f;
	enemy_cir->timer_charge = 0.0f;
	enemy_cir->timer_death = 0.5f;
	enemy_cir->states_cir = CIR_IDLE;
	enemy_cir->timer_changedir = 0.0f;
	
	//for collision
	AddCallBack(enemy_cir->character_cir, CircleEnemy_CollidePlayer);
	//AddCallBack(enemy_cir->character_cir, CircleEnemy_CollideEnemy);

	//when player dash at enemy, this function will be called
	AddDeadCallBack(enemy_cir->character_cir, CircleEnemy_Dead);

	//Particle system for death effects it will spit out "circles" when circle dies
	//CreateParticleSystem: emmission rate, angle it will cover, duration, speed, size of particles, vector set either (0,1) or (1,0), image
	ParticleSystem* ps = CreateParticleSystem(60.0f, 360.0f, 0.3f, 300.0f, 10.0f, CP_Vector_Zero(), CP_Vector_Zero(), IMAGE_CIRCLEPARTICLE, 200);
	ps->numParticles = 0; //start from 0
	VariableListAdd(&enemy_cir->character_cir->particleSystemList, ps);
	
	//alert cue particles
	ParticleSystem* psAlert = CreateParticleSystem(1, 0, 0.3f, 0, 45.0f, CP_Vector_Zero(), CP_Vector_Set(0, 1.0f), IMAGE_ALERT, 240);
	psAlert->numParticles = 1; 
	VariableListAdd(&enemy_cir->character_cir->particleSystemList, psAlert);// item 0
}

//check circle enemy's state every frame
void CircleEnemy_Update(EnemyCircle* enemy_cir, Player* player1, TileMap* tilemap, float deltaTime)
{
	//check if circle enemy is dead, if is dead, set it to inactive
	//if player is dead, respawn circle
	if (enemy_cir->isDead == 1)
	{
		enemy_cir->timer_death -= deltaTime;
		if (enemy_cir->timer_death <= 0.0f)
		{
			enemy_cir->character_cir->active = 0;
		}
		if (player1->isDead == 1)
		{
			CircleEnemy_Respawn(enemy_cir);
		}
		return;
	}
	//switch statement for enemy states
	switch (enemy_cir->states_cir)
	{
		case CIR_IDLE:
		{
			CircleEnemy_Idle(enemy_cir, player1, tilemap,deltaTime);
			break;
		}
		case CIR_DEAD:
		{
			CircleEnemy_Dead(enemy_cir);
			break;
		}
		case CIR_CHARGE:
		{
			CircleEnemy_Charge(enemy_cir, player1, tilemap, deltaTime);
			break;
		}
		default:
		{
			break;
		}
	}
}

//idle state
void CircleEnemy_Idle(EnemyCircle* enemy_cir, Player* player1, TileMap* tilemap, float deltaTime)
{
	float enemyOffSet, tileOffSet;

	//new position = circle's current position + (velocity * dt)
	CP_Vector newPos_cir = CP_Vector_Add(enemy_cir->character_cir->renderer.position,
		CP_Vector_Scale(enemy_cir->vel_cir, CP_System_GetDt()));

	CP_Vector* position_cir = CharacterGetPosition(enemy_cir->character_cir);

	enemyOffSet = (CharacterGetWidth(enemy_cir->character_cir) * 0.5f);
	tileOffSet = ((tilemap->tileSize) * 0.5f);

	//gets tile pos(world) (tilemap, enemy_cir position_cir x - enemy_cir off set, enemy_cir position_cir y - enemyoffset) 
	//zeroed off enemy_cir pos to check back tile
	Tile* tileBehind = GetTile_WorldPos(tilemap, position_cir->x - enemyOffSet, position_cir->y - enemyOffSet - 1);

	//gets tile pos(world) (tilemap, enemy_cir position_cir x + enemyoffset, enemy_cir position_cir y + enemyoffset) 
	//0.5 of enemy_cir pos plus offset check the tile infront
	Tile* tileFront = GetTile_WorldPos(tilemap, position_cir->x + enemyOffSet, position_cir->y - enemyOffSet - 1);

	//fore tile
	Tile* tileFore = GetTile_WorldPos(tilemap, position_cir->x + enemyOffSet, position_cir->y + 1); 

	//hind tile
	Tile* tileHind = GetTile_WorldPos(tilemap, position_cir->x - enemyOffSet, position_cir->y +1);

	enemy_cir->character_cir->renderer.position = newPos_cir;


	enemy_cir->timer_changedir += deltaTime;
	if (enemy_cir->timer_changedir > 8.0f)
	{
		enemy_cir->vel_cir.x *= -1.0f;
		enemy_cir->timer_changedir = 0.0f;
	}

	if (tileBehind && !IsColliding_Tile(tileBehind)) // IsCollidingTile returns false if tile is null, else it returns tileblock
	//if(check for tileBehind  && tileBehind is not tile)
	{
		CP_Vector* enemyPos = CharacterGetPosition(enemy_cir->character_cir);
		enemyPos->x = Tile_GetWorldPos(tilemap, tileBehind->row, tileBehind->column).x + tileOffSet + enemyOffSet + 1;
		//rounding error so +1;
		enemy_cir->vel_cir.x *= -1.0f;
	}
	else if (tileHind && IsColliding_Tile(tileHind))
	//else if(check for tileHind  && tileHind is tile) 
	{
		CP_Vector* enemyPos = CharacterGetPosition(enemy_cir->character_cir);
		enemyPos->x = Tile_GetWorldPos(tilemap, tileHind->row, tileHind->column).x + tileOffSet + enemyOffSet + 1;
		enemy_cir->vel_cir.x *= -1.0f;
	}


	if (tileFront && !IsColliding_Tile(tileFront))
	//if(check tileFront && tileFront not a tile) 
	{
		CP_Vector* enemyPos = CharacterGetPosition(enemy_cir->character_cir);
		enemyPos->x = Tile_GetWorldPos(tilemap, tileFront->row, tileFront->column).x - tileOffSet - enemyOffSet -1 ;
		enemy_cir->vel_cir.x *= -1.0f;
	}
	else if (tileFore && IsColliding_Tile(tileFore))
	//else if (check tileFore && tileFore is a tile)
	{
		CP_Vector* enemyPos = CharacterGetPosition(enemy_cir->character_cir);
		enemyPos->x = Tile_GetWorldPos(tilemap, tileFore->row, tileFore->column).x - tileOffSet - enemyOffSet - 1;
		enemy_cir->vel_cir.x *= -1.0f;
	}

	//peDist is the distance between enemy and player
	//if the distance between the enemy and player is less than 400.0f, enemy circle will go to charge and idle states at time intervals
	//else (if the distance between the enemy and player is more than 400.0f) it will only be idling.
	CP_Vector peDist = CP_Vector_Subtract(PlayerGetPosition(player1), Enemy_GetPos(enemy_cir));
	if (peDist.x < 400.0f)
	{
		enemy_cir->timer_idle += deltaTime;
		if (enemy_cir->timer_idle >= 0.5f)
		{
			CircleEnemy_ShowAlert(enemy_cir); //show alert 1.5s before it charges
		}
		if (enemy_cir->timer_idle >= 2.0f)
		{
			enemy_cir->states_cir = CIR_CHARGE;
			enemy_cir->timer_idle = 0.0f;
		}
	}
	else
	{
		enemy_cir->states_cir = CIR_IDLE;
	}
}

//using particles to show alert cues
void CircleEnemy_ShowAlert(EnemyCircle* enemy_cir)
{
	CP_Vector alertOffSet = CP_Vector_Set(0, 55.0f);
	ParticleSystem* psAlert = VariableListGetItemAt((&enemy_cir->character_cir->particleSystemList), 1);
	CharacterCreateParticles(psAlert, 1, CP_Vector_Add(Enemy_GetPos(enemy_cir), alertOffSet));
}

//charging state 
void CircleEnemy_Charge(EnemyCircle* enemy_cir, Player* player1, TileMap* tilemap, float deltaTime)
{
	/*CP_Vector alertOffSet = CP_Vector_Set(0, 60.0f);
	ParticleSystem* psAlert = VariableListGetItemAt((&enemy_cir->character_cir->particleSystemList), 1);
	CharacterCreateParticles(psAlert, 1, CP_Vector_Add(Enemy_GetPos(enemy_cir) , alertOffSet ) );*/

	float enemyOffSet, tileOffSet;
	CP_Vector newVel = CP_Vector_Scale(enemy_cir->vel_cir, 4.5);

	CP_Vector newPos_cir = CP_Vector_Add(enemy_cir->character_cir->renderer.position,
		CP_Vector_Scale(newVel, CP_System_GetDt()));

	CP_Vector* position_cir = CharacterGetPosition(enemy_cir->character_cir);
	enemyOffSet = (CharacterGetWidth(enemy_cir->character_cir) * 0.5f);
	tileOffSet = ((tilemap->tileSize) * 0.5f);

	//gets tile pos(world) (tilemap, enemy_cir position_cir x - enemy_cir off set, enemy_cir position_cir y - enemyoffset) //zeroed off enemy_cir pos to check back tile
	Tile* tileBehind = GetTile_WorldPos(tilemap, position_cir->x - enemyOffSet, position_cir->y - enemyOffSet - 1);
	//gets tile pos(world) (tilemap, enemy_cir position_cir x + enemyoffset, enemy_cir position_cir y + enemyoffset) //0.5 of enemy_cir pos plus offset check the tile infront
	Tile* tileFront = GetTile_WorldPos(tilemap, position_cir->x + enemyOffSet, position_cir->y - enemyOffSet - 1);//front
	//fore tile
	Tile* tileFore = GetTile_WorldPos(tilemap, position_cir->x + enemyOffSet, position_cir->y + 1); // forward tile
	//hind tile
	Tile* tileHind = GetTile_WorldPos(tilemap, position_cir->x - enemyOffSet, position_cir->y + 1);//back tile

	enemy_cir->character_cir->renderer.position = newPos_cir;

	if (tileBehind && !IsColliding_Tile(tileBehind)) // IsCollidingTile returns false if tile is null, else it returns tileblock
	//if (check for tileBehind  && tileBehind is not tile)
	{
		CP_Vector* enemyPos = CharacterGetPosition(enemy_cir->character_cir);
		enemyPos->x = Tile_GetWorldPos(tilemap, tileBehind->row, tileBehind->column).x + tileOffSet + enemyOffSet + 1;
		//rounding error so +1;
		enemy_cir->vel_cir.x *= -1.0f;
	}
	else if (tileHind && IsColliding_Tile(tileHind))
	//if(check for tileHind  && tileHind is a tile)
	{
		CP_Vector* enemyPos = CharacterGetPosition(enemy_cir->character_cir);
		enemyPos->x = Tile_GetWorldPos(tilemap, tileHind->row, tileHind->column).x + tileOffSet + enemyOffSet + 1;
		enemy_cir->vel_cir.x *= -1.0f;
	}


	if (tileFront && !IsColliding_Tile(tileFront))
	//if (check tileFront && tileFront is not a tile)
	{
		CP_Vector* enemyPos = CharacterGetPosition(enemy_cir->character_cir);
		enemyPos->x = Tile_GetWorldPos(tilemap, tileFront->row, tileFront->column).x - tileOffSet - enemyOffSet - 1;
		enemy_cir->vel_cir.x *= -1.0f;
	}
	else if (tileFore && IsColliding_Tile(tileFore))
	//if (check tileFore && tileFore is a tile)
	{
		CP_Vector* enemyPos = CharacterGetPosition(enemy_cir->character_cir);
		enemyPos->x = Tile_GetWorldPos(tilemap, tileFore->row, tileFore->column).x - tileOffSet - enemyOffSet - 1;
		enemy_cir->vel_cir.x *= -1.0f;
	}
	
	//time interval, go into idle state after charging for two seconds
	enemy_cir->timer_charge += deltaTime;

	if (enemy_cir->timer_charge >= 2.0f)
	{
		enemy_cir->states_cir = CIR_IDLE;
		enemy_cir->timer_charge = 0.0f;
	}
}

//respawn function, to be used in update
void CircleEnemy_Respawn(EnemyCircle* enemy_cir)
{
	if (enemy_cir->isDead == 1)
	{
		enemy_cir->character_cir->active = 1;
		enemy_cir->isDead = 0;
		enemy_cir->vel_cir = CP_Vector_Set(100.0f, 0.0f);
		enemy_cir->timer_idle = 0.0f;
		enemy_cir->timer_charge = 0.0f;
		enemy_cir->timer_death = 0.5f;
		enemy_cir->states_cir = CIR_IDLE;
	}
}

//when player dash and collide with enemy circle, enemy circle will die 
//animated with particle system
void CircleEnemy_Dead(void* enemy_cir)
{
	Character* circle1 = (Character*)(enemy_cir);
	EnemyCircle* circleE = circle1->mainObject;
	circleE->isDead = 1;
	//show sprite death animation
	//death timer, after animation then set inactive
	PlaySoundEffect(SOUND_POWERUP);
	//call particle system here when enemy dies
	ParticleSystem* ps = VariableListGetItemAt((&circleE->character_cir->particleSystemList), 0);
	CharacterCreateParticles(ps, 30, Enemy_GetPos(circleE));
}

//check if colliding with player
void CircleEnemy_CollidePlayer(void* circle, void* otherenemy)
{
	Character* other = otherenemy;
	Character* circle1 = circle;
	EnemyCircle* enemy_cir = circle1->mainObject;

	//kill the player if enemy is not dead
	if (other->tag == CHARACTER_PLAYER && enemy_cir->isDead == 0)
	{
		other->SetDead(other);
	}
	if (other->tag == CHARACTER_ENEMY_SPIKES || other->tag == CHARACTER_ENEMY_TURRET)
	{
		CP_Vector pos = CP_Vector_Subtract(*CharacterGetPosition(circle1), *CharacterGetPosition(other));
		float totalWidth = CharacterGetWidth(circle1)*0.5f + CharacterGetWidth(other) * 0.5f;
		float difference = totalWidth - fabsf(pos.x);
		if(enemy_cir->vel_cir.x >= 0)
			CharacterGetPosition(circle1)->x = CharacterGetPosition(circle1)->x + (difference * -1.0f);
		else
			CharacterGetPosition(circle1)->x = CharacterGetPosition(circle1)->x + (difference);
		enemy_cir->vel_cir.x *= -1.0f;
	}
	
}

//void CircleEnemy_CollideEnemy(void* circle, void* otherenemy)
//{
//	Character* other = otherenemy;
//	Character* circle2 = circle;
//	EnemyCircle* enemy_cir = circle2->mainObject;
//
//	if (other->tag == CHARACTER_PLAYER && enemy_cir->isDead == 0)
//	{
//		other->SetDead(other);
//	}
//	//if not player, change direction
//	if ( other->tag == CHARACTER_ENEMY_SPIKES)
//	{
//		enemy_cir->vel_cir.x *= -1.0f;
//	}
//}

//get position for enemy
CP_Vector Enemy_GetPos(EnemyCircle* enemy_cir)
{
	return enemy_cir->character_cir->renderer.position;
}

