/*!
@file		EnemyTurret.c
@author		Leong Jun Xiang	(junxiang.leong)
@course		CSD1400
@brief		contains the different states for the AI to aim and shoot,
			creation of the turrret
			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/
#include "EnemyTurret.h"
#include "Bullet.h"
#define SHOOTTIMER 2.0f //time for turret to take aim and shoot
void CreateEnemyTurret(EnemyTurret*object,CP_Vector position)
{
	object->character = CreateCharacter(object, position, CP_Vector_Zero(), 60, 60, CHARACTER_ENEMY_TURRET, IMAGE_TURRET);
	object->shootTimer = SHOOTTIMER;
	object->state = IDLE;
}
void EnemyTurretIdle(EnemyTurret* object, Player* player)
{
	CP_Vector *playerpos = CharacterGetPosition(player->character);
	float length = CP_Vector_Distance(*playerpos, *CharacterGetPosition(object->character));
	if (length < 500)
		object->state = AIM;
}
void EnemyTurretAim(EnemyTurret* object, Player* player, float dt)
{
	if (object->shootTimer == SHOOTTIMER)
	{
		CP_Vector* playerpos = CharacterGetPosition(player->character);
		object->oldPlayerPos.x = playerpos->x;
		object->oldPlayerPos.y = playerpos->y;
	}
	object->shootTimer -= dt;
	if (object->shootTimer < 0)
	{
		object->shootTimer = SHOOTTIMER;
		object->state = SHOOT;
	}
}
void EnemyTurretShoot(VariableList* bulletlist, EnemyTurret* object, float dt)
{
	CP_Vector dir = CP_Vector_Subtract(object->oldPlayerPos, *CharacterGetPosition(object->character));
	dir = CP_Vector_Normalize(dir);
	CP_Vector offset = CP_Vector_Add(*CharacterGetPosition(object->character),CP_Vector_Scale(dir,50));
	BulletShoot(bulletlist, offset, CP_Vector_Scale(dir, 100.f), 30, 30, CHARACTER_ENEMY_TURRET);
	object->state = IDLE;
}
void EnemyTurretUpdate(VariableList* bulletList,EnemyTurret* object, Player* player, float dt)
{
	if (!object->character->active)
		return;
	switch (object->state)
	{
	case IDLE:
		EnemyTurretIdle(object, player);
		break;
	case AIM:
		EnemyTurretAim(object, player, dt);
		break;
	case SHOOT:
		EnemyTurretShoot(bulletList,object, dt);
		break;
	}
}