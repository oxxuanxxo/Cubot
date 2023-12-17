/*!
@file		Bullet.h
@author		Leong Jun Xiang	(junxiang.leong)
@course		CSD1400
@brief		contains the bullet function declaration
Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/
#ifndef BULLET_H
#define BULLET_H
#include "../../CProcessing/inc/cprocessing.h"
#include "../Misc/BoxCollider.h"
#include "../Misc/VariableList.h"
#include "../GlobalDefine.h"
#include "../Render/Renderer.h"
#include "../Camera/Camera.h"
#include "Character.h"
typedef struct Bullet
{
	int active;
	float bulletLifetime;
	CharacterTag bulletOwner;
	CP_Vector velocity;
	BoxCollider collider;
	Renderer renderer;
}Bullet;
typedef void (*BulletCallback)(Bullet* bullet, Character* character);
Bullet* BulletCreate(VariableList* bulletList);
Bullet* GetInactiveBullet(VariableList* bulletList);
void BulletSet(Bullet* bullet,CP_Vector pos,CP_Vector velocity, float width, float height, CharacterTag bulletowner);
void BulletUpdate(Bullet * bul,float dt);
void BulletShoot(VariableList* bulletList, CP_Vector pos, CP_Vector velocity, float width, float height, CharacterTag owner);

//takes in a list of bullets
void BulletListUpdate(VariableList* bulletlist,float dt);
void BulletListRender(VariableList*bulletlist,Camera* cam);

//collision
void BulletListCheckCollision(VariableList* bulletlist, VariableList* listofcharacter,float dt);
void BulletListCheckCollisionAgainst(VariableList* bulletlist, Character* character, float dt , BulletCallback func);
#endif  //BULLET_H

