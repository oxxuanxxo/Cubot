/*!
@file		EnemyTurret.h
@author		Leong Jun Xiang	(junxiang.leong)
@course		CSD1400
@brief		contains the different states for the AI to aim and shoot,
			creation of the turrret
			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/
#ifndef ENEMYTURRET_H
#define ENEMYTURRET_H
#include "Character.h"
#include "Player.h"
#include "Bullet.h"
#include "../Misc/VariableList.h"
typedef enum TURRETSTATE
{
	IDLE,
	AIM,
	SHOOT,
}TURRETSTATE;

typedef struct EnemyTurret
{
	float shootTimer;
	TURRETSTATE state;
	CP_Vector oldPlayerPos;
	Character* character;
}EnemyTurret;

void CreateEnemyTurret(EnemyTurret*object,CP_Vector position);
void EnemyTurretIdle(EnemyTurret* object,Player* player);
void EnemyTurretAim(EnemyTurret* object,Player* player, float dt);
void EnemyTurretShoot(VariableList* bulletlist,EnemyTurret* object, float dt);
void EnemyTurretUpdate(VariableList* bulletlist,EnemyTurret* object, Player* player, float dt);
#endif
