/*!
@file		EnemySpikes.h
@author		Leong Jun Xiang	(junxiang.leong)
@course		CSD1400
@brief		contains a creation for the spikes and the collision response
			with the player
			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/
#ifndef ENEMYSPIKES_H
#define ENEMYSPIKES_H

#include "Character.h"

typedef struct EnemySpikes
{
	Character* character;
}EnemySpikes;

void CreateEnemySpikes(EnemySpikes* spikes, CP_Vector position);
static void OnCollideWithPlayer(void* spikes, void* player);
#endif // !ENEMYSPIKES_H
