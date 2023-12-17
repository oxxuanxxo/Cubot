/*!
@file       EnemyCircle.h
@author     Chua Yip Xuan (yipxuan.chua)
@course     CSD1400
@section    PASCAL-AM
@brief      This file consists of functions that will create a circle enemy
			The circle enemy's AI (finite state machine) will go from idling state to charging state after time intervals or when player is near.
			Visual cue (alert cue) is implemented 1.5s before it charges, as many feedbacks in the user testing stated that it was too sudden.

			Copyright © 2020 DigiPen, All rights reserved.
*/
/*__________________________________________________________________________*/

#ifndef ENEMY_H
#define ENEMY_H
#include <stdbool.h>
#include "../../CProcessing/inc/cprocessing.h"
#include "Character.h"
#include "../Tiles/TileMap.h"
#include "../Characters/Player.h"

typedef enum CIR_ENEMYSTATES {
	CIR_IDLE,
	CIR_CHARGE,
	CIR_DEAD
}CIR_ENEMYSTATES;

typedef struct Enemy
{
	int isDead;
	float timer_idle, speed_cir, timer_charge, timer_death, timer_changedir;
	Character* character_cir;
	CIR_ENEMYSTATES states_cir;
	CP_Vector vel_cir;

}EnemyCircle;

//create & update
void CircleEnemy_Create(EnemyCircle* enemy_cir, CP_Vector position_cir, CP_Vector collOffSet);
void CircleEnemy_Update(EnemyCircle* enemy_cir, Player* player1, TileMap* tilemap, float deltaTime);

//states
void CircleEnemy_Idle(EnemyCircle* enemy_cir, Player* player1, TileMap* tilemap, float deltaTime);
void CircleEnemy_Dead(void* enemy_cir);
void CircleEnemy_Charge(EnemyCircle* enemy_cir, Player* player1, TileMap* tilemap, float deltaTime);

//Circle functions
CP_Vector Enemy_GetPos(EnemyCircle* enemy_cir);
void CircleEnemy_CollidePlayer(void* enemy_cir, void* otherenemy);
void CircleEnemy_CollideEnemy(void* circle, void* otherenemy);
void CircleEnemy_ShowAlert(EnemyCircle* enemy_cir);
void CircleEnemy_Respawn(EnemyCircle* enemy_cir);


#endif



