/*!
@file		player.c
@author		Leong Jun Xiang	(junxiang.leong)
@course		CSD1400
@brief		contains all player movement, collision , and collision response
			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/
#include "Player.h"
#include "../Sound/Sound.h"
#include "../Scenes/MainMenu.h"
#include "../Scenes/GameOver.h"
#include "../Scenes/Transition.h"

#define GRAVITY 800.f
#define JUMPSTR 550.f
#define SPEEDMULTIPLIER 5.0f
#define CHARACTERWIDTH 60.0f
#define CHARACTERHEIGHT 60.0f
#define DASHTIMER 0.3f
#define STUNTIMER 0.5f
#define DASHMULTIPLIER 3
#define RESPAWNTIMER 2.0f
void CreatePlayer(Player* object, CP_Vector position, float movespeed, int health)
{
	CP_Vector collider_offset = CP_Vector_Set(0, 0);
	object->character = CreateCharacter(object,position, collider_offset, CHARACTERWIDTH, CHARACTERHEIGHT, CHARACTER_PLAYER, IMAGE_PLAYER);

	Renderer_InitSpriteSheet(&object->character->renderer, IMAGE_PLAYERSPRITESHEET, 512, 512, 5, 5);
	object->waypoint = position;
	object->moveSpeed = movespeed;
	object->health = health;
	object->direction = CP_Vector_Zero();
	object->velocity = CP_Vector_Zero();
	
	object->isStunned = 0;
	object->stunTimer = STUNTIMER;
	object->respawnTimer = RESPAWNTIMER;

	object->isGodMode = 0;
	object->isJump = 0;
	object->isGround = 1;
	object->isDash = 0;
	object->isDead = 0;

	object->lookAt = 1;
	object->dashTimer = DASHTIMER;

	AddCallBack(object->character, CollideWithEnemy);
	AddDeadCallBack(object->character, PlayerSetDead);


	ParticleSystem* ps = CreateParticleSystem(10, 180.0f, 1, 30.0f, 30, CP_Vector_Zero(),CP_Vector_Set(1.0f,0), IMAGE_SMOKE, 60);
	ps->numParticles = 0;
	VariableListAdd(&object->character->particleSystemList, ps);// item 0

	ps = CreateParticleSystem(60, 0, 0.3f, 0, 45, CP_Vector_Zero(), CP_Vector_Set(0, 0.1f), IMAGE_PLAYERDASH, 60);
	ps->numParticles = 0;
	VariableListAdd(&object->character->particleSystemList, ps);// item 1

	ps = CreateParticleSystem(30, 180, 0.3f, 0, 45, CP_Vector_Zero(), CP_Vector_Set(0, 0.1f), IMAGE_PLAYERPARTICLE, 30);
	ps->numParticles = 0;
	VariableListAdd(&object->character->particleSystemList, ps);// item 2

}
void PlayerSetDirection(Player* object, CP_Vector dir)
{
	if (!object->isDash)
	{
		object->direction = dir;
		if (object->lookAt != dir.x && dir.x == 1)
			Renderer_PlayAnimation(&object->character->renderer, 0, 5, 20, 0);
		else if (object->lookAt != dir.x && dir.x == -1)
			Renderer_PlayAnimation(&object->character->renderer, 1, 5, 20, 0);
 		object->lookAt = dir.x;
	}
}
static void PlayerAcceleration_X(Player* object, float acceleration_x,float dt)
{
	if (!object->isStunned)
	{
		object->velocity.x += acceleration_x* object->direction.x * dt;
		object->velocity.x = CP_Math_ClampFloat(object->velocity.x, -object->moveSpeed, object->moveSpeed);
	}
}
static void PlayerAccleration_Y(Player* object, float acceleration_y, float dt)
{
	if (!object->isGround)
	{
		object->velocity.y -= GRAVITY * dt;
	}
}
void PlayerJump(Player* object)
{
	if (object->isJump == 0 
		&& object->isGround == 1 
		&& object->isDash == 0
		&& object->isStunned == 0)
	{
		object->isGround = 0;
		object->isJump = 1;
		object->velocity.y += JUMPSTR;
		PlaySoundEffect(SOUND_JUMP);
	}
}
void PlayerDash(Player* object)
{
	if (!object->isDash && object->dashTimer < -DASHTIMER* 2.0f && !object->isStunned)
	{
		object->dashTimer = DASHTIMER;
		object->isDash = 1;
		object->velocity.y = 0;
		PlaySoundEffect(SOUND_NEWDASH);
		if(object->lookAt == 1)
			Renderer_PlayAnimation(&object->character->renderer, 2, 5, 10, 0);
		else if(object->lookAt == -1)
			Renderer_PlayAnimation(&object->character->renderer, 3, 5, 10, 0);

	}
}
void PlayerStopDash(Player* object)
{
	object->isDash = 0;
	if (object->lookAt == 1)
		Renderer_PlayAnimation(&object->character->renderer, 0, 5, 10, 0);
	else if (object->lookAt == -1)
		Renderer_PlayAnimation(&object->character->renderer, 1, 5, 10, 0);
}
void PlayerStunned(Player* object, float timer)
{
	object->isStunned = 1;
	object->stunTimer = timer;
}
static void PlayerCheckWorldBounding(Player* object, TileMap* map)
{
	//float tileoffset = map->tileSize * 0.5f;
	float playeroffset = CharacterGetWidth(object->character) * 0.5f;
	float leftbound = -map->tileSize * 0.5f;
	float rightbound = (map->numCol - 1) * (map->tileSize);
	
	CP_Vector* playerpos = CharacterGetPosition(object->character);
	if (playerpos->x + playeroffset> rightbound)
	{
		playerpos->x = rightbound - playeroffset;
	}
	else if (playerpos->x - playeroffset < leftbound)
	{
		playerpos->x = leftbound + playeroffset;
	}
	else if (playerpos->y < leftbound)
	{
		if(object->isGodMode == 0)
			object->character->SetDead(object->character);
		else
		{
			object->isGodMode = 0;
			object->character->SetDead(object->character);
			object->isGodMode = 1;
		}
	}
	
}
static void PlayerCheckGround(Player* object,TileMap* map)
{
	//when touching ground
	CP_Vector* playerpos = CharacterGetPosition(object->character);
	float playeroffset = (CharacterGetWidth(object->character) * 0.5f);
	float tileOffset = (map->tileSize) * 0.5f;
	Tile* t = GetTile_WorldPos(map, playerpos->x, playerpos->y - tileOffset - playeroffset);
	Tile* t2 = GetTile_WorldPos(map, playerpos->x - tileOffset, playerpos->y - tileOffset - playeroffset);//back
	Tile* t3 = GetTile_WorldPos(map, playerpos->x + tileOffset, playerpos->y - tileOffset - playeroffset);//front

	if (object->isGround == 0)
	{
		if (t && IsColliding_Tile(t))//ground tile
		{
			float tileTop = Tile_GetWorldPos(map, t->row, t->column).y + tileOffset + playeroffset;
			if (playerpos->y <= tileTop)
			{
				//round player position to nearest grid in y axis
				playerpos->y = tileTop;
				object->isGround = 1;
				object->isJump = 0;
				object->velocity.y = object->velocity.y < 0 ? 0 : object->velocity.y;

				PlaySoundEffect(SOUND_LAND);
				ParticleSystem* ps = VariableListGetItemAt(&object->character->particleSystemList,0);
				CharacterCreateParticles(ps, 5, *CharacterGetPosition(object->character));
			}
		}
		else if (t2 && IsColliding_Tile(t2))//check with tile behind
		{
			CP_Vector tilepos = Tile_GetWorldPos(map, t2->row, t2->column);
			CP_Vector p = CP_Vector_Subtract(tilepos, *playerpos);
			if (p.y * p.y < p.x * p.x)//comparing the x and y vector of the object to tile if x is smaller move x else move y
			{
				playerpos->x = Tile_GetWorldPos(map, t2->row, t2->column).x + tileOffset + playeroffset;
				object->velocity.x = 0;
				object->isGround = 0;
			}
			else if(playerpos->y - tileOffset - playeroffset < tilepos.y)
			{
				object->isGround = 1;
				object->isJump = 0;
				object->velocity.y = 0;
				playerpos->y = Tile_GetWorldPos(map, t2->row, t2->column).y + tileOffset + playeroffset;

				PlaySoundEffect(SOUND_LAND);

				ParticleSystem* ps = VariableListGetItemAt(&object->character->particleSystemList, 0);
				CharacterCreateParticles(ps, 5, *CharacterGetPosition(object->character));
			}
		}
		else if (t3 && IsColliding_Tile(t3))//check with tile infront
		{
			CP_Vector tilepos = Tile_GetWorldPos(map, t3->row, t3->column);
			CP_Vector p = CP_Vector_Subtract(tilepos , *playerpos);
			
			if (p.x*p.x  >  p.y*p.y)
			{
				object->velocity.x = 0;
				object->isGround = 0;
				playerpos->x = tilepos.x - tileOffset - playeroffset;

			}
			else if(playerpos->y - tileOffset - playeroffset < tilepos.y)
			{

				object->isGround = 1;
				object->isJump = 0;

				object->velocity.y = 0;
				playerpos->y = tilepos.y + tileOffset + playeroffset;

				PlaySoundEffect(SOUND_LAND);
				ParticleSystem* ps = VariableListGetItemAt(&object->character->particleSystemList, 0);
				CharacterCreateParticles(ps, 5, *CharacterGetPosition(object->character));

			}
			
		}
	}
	else if (t && !IsColliding_Tile(t))//fall off ground
	{

		if (t2 && !IsColliding_Tile(t2) && t3&& !IsColliding_Tile(t3))
		{
			object->isGround = 0;
		}
	}
}
static void PlayerCheckCeiling(Player* object, TileMap* map)
{
	if (!object->isGround)
	{
		CP_Vector* playerpos = CharacterGetPosition(object->character);
		float playerOffset = (CharacterGetWidth(object->character) * 0.5f);
		float tileOffset = (map->tileSize * 0.5f) + playerOffset;
		Tile* t = GetTile_WorldPos(map, playerpos->x , playerpos->y + tileOffset); // top tile
		Tile* t2 = GetTile_WorldPos(map, playerpos->x + playerOffset, playerpos->y + playerOffset); // top diagonal front
		Tile* t3 = GetTile_WorldPos(map, playerpos->x - playerOffset, playerpos->y + playerOffset); // top diagonal back
		if (t && IsColliding_Tile(t))
		{
			float tileSide = Tile_GetWorldPos(map, t->row, t->column).y - tileOffset;
			if (playerpos->y > tileSide)
			{
				playerpos->y = tileSide;
				object->velocity.y = 0;
				object->isJump = 0;
			}
		}
		else if (t2 && IsColliding_Tile(t2))
		{
			
			CP_Vector tilepos = Tile_GetWorldPos(map, t2->row, t2->column);
			CP_Vector p = CP_Vector_Subtract(*playerpos, tilepos);
			// cant find the abs function use sq instead
			if (p.x * p.x > p.y * p.y)//comapring if the x coord is closer or y 			
			{
				playerpos->x = tilepos.x - tileOffset;
				object->velocity.x = 0;
			}
			else
			{
				playerpos->y = tilepos.y -tileOffset;
				object->velocity.y = object->velocity.y > 0 ? 0 : object->velocity.y;
				object->isJump = 0;
			}
		}
		else if (t3 && IsColliding_Tile(t3))
		{
			CP_Vector tilepos = Tile_GetWorldPos(map, t3->row, t3->column);
			CP_Vector p = CP_Vector_Subtract(*playerpos, tilepos);
			// cant find the abs function use sq instead
			if (p.x * p.x > p.y * p.y)//comapring if the x coord is closer or y 		
			{
				playerpos->x = tilepos.x +tileOffset;
				object->velocity.x = 0;
			}
			else
			{
				playerpos->y = tilepos.y -tileOffset;
				object->velocity.y = object->velocity.y > 0 ? 0 : object->velocity.y;
				object->isJump = 0;
			}
		}
	}
}
static void PlayerCheckForward(Player* object, TileMap* map)
{
	CP_Vector* playerpos = CharacterGetPosition(object->character);
	float playerOffset = (CharacterGetWidth(object->character) * 0.5f);
	float tileOffset = (map->tileSize * 0.5f) + playerOffset;
	Tile* t = GetTile_WorldPos(map, playerpos->x + playerOffset, playerpos->y); // forward tile
	if (t && IsColliding_Tile(t))
	{
		float tileSide = Tile_GetWorldPos(map, t->row, t->column).x - tileOffset;
		if (playerpos->x > tileSide)
		{
			playerpos->x = tileSide;
			object->velocity.x = 0;
		}
		return;
	}
	Tile* t2 = GetTile_WorldPos(map, playerpos->x - playerOffset, playerpos->y);//back tile
	if (t2 && IsColliding_Tile(t2))
	{
		float tileSide = Tile_GetWorldPos(map, t2->row, t2->column).x + tileOffset;
		if (playerpos->x < tileSide)
		{
			playerpos->x = tileSide;
			object->velocity.x = 0;
		}
		return;
	}
}
static void PlayerMovement(Player* object, TileMap* map, float dt)
{
	//Movment Inputs
	if (CP_Input_KeyTriggered(KEY_W) || CP_Input_KeyTriggered(KEY_UP))
	{
		PlayerJump(&player);
	}
	if (CP_Input_KeyDown(KEY_D) || CP_Input_KeyDown(KEY_RIGHT))
	{
		PlayerSetDirection(&player, CP_Vector_Set(1, 0));
	}
	if (CP_Input_KeyDown(KEY_A) || CP_Input_KeyDown(KEY_LEFT))
	{
		PlayerSetDirection(&player, CP_Vector_Set(-1, 0));
	}
	if (CP_Input_KeyTriggered(KEY_SPACE))
	{
		PlayerDash(&player);
	}
	if (CP_Input_KeyTriggered(KEY_F1))
	{
		object->isGodMode = !object->isGodMode;
		PlaySoundEffect(SOUND_POWERUP);
	}

	if (object->isDash)//sub-steps
	{
		for (int i = 0; i < DASHMULTIPLIER; ++i)
		{
			CharacterGetPosition(object->character)->x += (object->moveSpeed * object->lookAt) * dt;
			PlayerCheckForward(object, map);
			PlayerCheckGround(object, map);
			PlayerCheckCeiling(object, map);
		}
		CharacterGetPosition(object->character)->y += object->velocity.y * dt;
		PlayerCheckWorldBounding(object, map);
		ParticleSystem* ps = VariableListGetItemAt(&object->character->particleSystemList, 1);
		CharacterCreateParticles(ps, 1, *CharacterGetPosition(object->character));
	}
	else
	{
		CharacterGetPosition(object->character)->x += object->velocity.x * dt;
		CharacterGetPosition(object->character)->y += object->velocity.y * dt;
		PlayerCheckWorldBounding(object, map);
		PlayerCheckForward(object, map);
		PlayerCheckGround(object, map);
		PlayerCheckCeiling(object, map);
	}
}
void PlayerPhysicsUpdate(Player* object,TileMap* map, float dt)
{
	object->dashTimer -= dt;
	if (object->dashTimer < 0)
	{
		if (object->isDash)
		{
			PlayerStopDash(object);
		}
	}
	object->stunTimer -= dt;
	if (object->stunTimer < 0)
		object->isStunned = 0;
	PlayerDead(object, dt);
	if (object->isDead) // stop everything below if is dead
		return;

	//clamp the velocity
	if ((int)object->velocity.x == 0)
		object->velocity.x = 0;
	if (object->direction.x == 0)// fully stop
		object->velocity.x -= object->velocity.x* SPEEDMULTIPLIER * dt;
	else
		PlayerAcceleration_X(object, SPEEDMULTIPLIER *object->moveSpeed, dt);
	PlayerAccleration_Y(object, SPEEDMULTIPLIER * object->moveSpeed, dt);
	object->direction = CP_Vector_Zero();//reset dir to 0
	PlayerMovement(object,map,dt);
	//ParticleSystem_Update(object->particleSystem);
}
CP_Vector PlayerGetPosition(Player* object)
{
	return object->character->renderer.position;
}
void PlayerSetDead(void* p)
{
	Character* c = p;
	if (c->tag == CHARACTER_PLAYER)
	{
		Player* object = c->mainObject;
		if (object->isGodMode == 1 )
			return;
		if (object->isDead == 0 && object->isDash == 0)
		{
			ParticleSystem* ps = VariableListGetItemAt(&object->character->particleSystemList,2);
			CharacterCreateParticles(ps, 15, *CharacterGetPosition(object->character));
			object->isDead = 1;
			object->health -= 1;
			PlayerStunned(object, RESPAWNTIMER);
			PlaySoundEffect(SOUND_HIT);
			if (object->health == 0)
			{
				//Transit_GameOver();
				CP_Engine_SetNextGameState(GameOver_init, GameOver_update, GameOver_exit);
			}
		}
	}
}
static void PlayerDead(Player* object , float dt)
{
	if (object->isDead)
	{
		object->respawnTimer -= dt;
		if (object->respawnTimer > RESPAWNTIMER - 1)
		{
			Renderer* render = &object->character->renderer;
			render->scale.x -= CHARACTERWIDTH * dt;
			render->scale.y -= CHARACTERHEIGHT * dt;
		}
		else if (object->respawnTimer > 0) // shrinking
		{
			CP_Vector* playerpos = CharacterGetPosition(object->character);
			Renderer* render = &object->character->renderer;
			render->scale.x += CHARACTERWIDTH * dt;
			render->scale.y += CHARACTERHEIGHT * dt;
			*playerpos = object->waypoint;
		}
		else if(object->respawnTimer < 0) //respawn
		{
			object->respawnTimer = RESPAWNTIMER;
			object->isDead = 0;
			PlaySoundEffect(SOUND_RESTART);
			Renderer* render = &object->character->renderer;
			render->scale.x = CHARACTERWIDTH;
			render->scale.y = CHARACTERHEIGHT;
			Renderer_PlayAnimation(&object->character->renderer, 0, 5, 20, 0);
		}
	}
}

static void CollideWithEnemy(void* player1, void* enemy)
{
	Character* e = enemy;
	if (e->tag != CHARACTER_PLAYER )
	{
		Character* p = player1;
		Player* object = p->mainObject;
		if (object->isDead)
			return;
		if (object->isDash)
		{
			if (e->SetDead)
				e->SetDead(e);
			CP_Vector dir = CP_Vector_Subtract(*CharacterGetPosition(p), *CharacterGetPosition(e));
			object->velocity.x = dir.x * 5.0f;
			object->velocity.y = 300.0f;
			PlayerStopDash(object);
			object->isGround = 0;
			PlayerStunned(object, STUNTIMER);
			return;
		}
	}
}
void PlayerCollideWithBullet(Bullet* bullet, Character* object)
{

	Player* player1 = object->mainObject;

	if (player1->isDead)
		return;
	if (player1->isDash)
	{
		bullet->active = false;
		return;
	}
	PlayerSetDead(player1->character);
}