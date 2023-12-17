/*!
@file		EnemySpikes.c
@author		Leong Jun Xiang	(junxiang.leong)
@course		CSD1400
@brief		contains a creation for the spikes and the collision response 
			with the player
			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/
#include "EnemySpikes.h"
#include "Player.h"
void CreateEnemySpikes(EnemySpikes* spikes, CP_Vector position)
{
	spikes->character = CreateCharacter(spikes, position, CP_Vector_Zero(), 60, 60, CHARACTER_ENEMY_SPIKES, IMAGE_SPIKE);
	if (spikes->character)
	{
		spikes->character->collider.height *= 0.9f;
		spikes->character->collider.width *= 0.9f;

		AddCallBack(spikes->character, OnCollideWithPlayer);
	}
}
static void OnCollideWithPlayer(void* spikes, void* player)
{
	Character* p = player;
	if (p->tag == CHARACTER_PLAYER)
	{
		p->SetDead(p);
	}
}