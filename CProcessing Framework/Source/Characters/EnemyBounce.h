/*!
@file		EnemyBounce.h
@author		Leong Jun Xiang	(junxiang.leong)
@course		CSD1400
@brief		contains the declaration of the object and collision response of it
Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/
#ifndef ENEMYBOUNCE_H
#define ENEMYBOUNCE_H
#include "../../CProcessing/inc/cprocessing.h"
#include "Character.h"

typedef struct EnemyBounce
{
	Character* character;
	float bounceStrength;
}EnemyBounce;

void CreateEnemyBounce(EnemyBounce* object, CP_Vector position);
static void CollideWithObject(void* thisobj, void* other);
#endif // !ENEMYBOUNCE_H
