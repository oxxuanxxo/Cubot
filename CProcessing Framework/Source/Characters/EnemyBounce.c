/*!
@file		EnemyBounce.c
@author		Leong Jun Xiang	(junxiang.leong)
@course		CSD1400
@brief		contains the creation of the object and collision response of it
Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/
#include "EnemyBounce.h"
#include "Player.h"
#include "../Sound/Sound.h"
#include <math.h>
#define BOUNCESTRENGTH 700.0f
void CreateEnemyBounce(EnemyBounce* object, CP_Vector position)
{
	CP_Vector collider_offset = CP_Vector_Set(0, 0);
	object->character = CreateCharacter(object, position, collider_offset, 60.f, 60.f, CHARACTER_EMEMY_BOUNCE, IMAGE_SLIMESPRITESHEET);
	object->bounceStrength = BOUNCESTRENGTH;
	AddCallBack(object->character, CollideWithObject);

	ParticleSystem* ps = CreateParticleSystem(60, 360.0f, 1, 100.0f, 10, CP_Vector_Zero(), CP_Vector_Set(0, 1.0f), IMAGE_SLIMEPARTICLES, 60);
	ps->numParticles = 0;
	VariableListAdd(&object->character->particleSystemList, ps);// item 0


	Renderer_InitSpriteSheet(&object->character->renderer, IMAGE_SLIMESPRITESHEET, 128, 1152, 9, 1);
}

static void CollideWithObject(void* thisobj, void* other)
{
	Character * bounce = thisobj;
	Character* object = other;
	if (object->tag == CHARACTER_PLAYER)
	{
		Player* player = object->mainObject;
		
		EnemyBounce* enemy = bounce->mainObject;
		CP_Vector dir = CP_Vector_Subtract(*CharacterGetPosition(object), *CharacterGetPosition(bounce));
		dir = CP_Vector_Normalize(dir);

		if (fabs(dir.x) > fabs(dir.y))
		{
			dir.x = dir.x > 0 ? 1.0f : -1.0f;
			player->velocity.x = dir.x * enemy->bounceStrength;
		}
		else
		{
			PlayerJump(player);
			dir.y = dir.y > 0 ? 1.0f : -1.0f;
			player->velocity.y = dir.y * enemy->bounceStrength;
			Renderer_PlayAnimation(&enemy->character->renderer, 0, 9, 20, 0);
		}
		PlaySoundEffect(SOUND_BOUNCE);
		ParticleSystem* ps = VariableListGetItemAt(&enemy->character->particleSystemList, 0);
		CP_Vector pos = *CharacterGetPosition(player->character);
		pos.y -= CharacterGetWidth(player->character) * 0.5f;
		CharacterCreateParticles(ps, 20, pos);
	}
}