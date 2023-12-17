/*!
@file		Bullet.c
@author		Leong Jun Xiang	(junxiang.leong)
@course		CSD1400
@brief		a generic projectile struct that can render type of 
			projectiles and track the owner of the projectile
			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/
#include <stdlib.h>
#include "Bullet.h"
#include "Character.h"
#include "../Render/RenderUtil.h"
#define BULLET_LIFETIME 3.0f
#define BULLET_IMAGE IMAGE_BULLET
void BulletSet(Bullet* bullet, CP_Vector pos, CP_Vector velocity, float width, float height, CharacterTag bulletowner)
{
	bullet->active = 1;
	bullet->velocity.x = velocity.x;
	bullet->velocity.y = velocity.y;

	bullet->renderer.position.x = pos.x;
	bullet->renderer.position.y = pos.y;
	bullet->renderer.image = BULLET_IMAGE;
	bullet->renderer.scale.x = width;
	bullet->renderer.scale.y = height;
	bullet->renderer.alpha = 255;

	bullet->bulletOwner = bulletowner;
	BoxColliderSet(&bullet->collider, CP_Vector_Zero(), width, height);
	bullet->bulletLifetime = BULLET_LIFETIME;
}
Bullet* BulletCreate(VariableList * bulletList)
{
	Bullet* temp = malloc(sizeof(Bullet));
	VariableListAdd(bulletList, temp);
	return temp;
}
Bullet* GetInactiveBullet(VariableList* bulletList)
{
	for (int i = 0; i < bulletList->length; ++i)
	{
		Bullet* bul = VariableListGetItemAt(bulletList,i);
		if (!bul->active)
			return bul;
	}
	return NULL;
}
void BulletShoot(VariableList * bulletList,CP_Vector pos, CP_Vector velocity, float width, float height, CharacterTag owner)
{
	Bullet* bullet = GetInactiveBullet(bulletList);
	if (bullet == NULL)
		bullet = BulletCreate(bulletList);
	BulletSet(bullet, pos, velocity, width, height, owner);
}
void BulletUpdate(Bullet* bul, float dt)
{
	if (bul->active)
	{
		bul->renderer.position.x += bul->velocity.x * dt;
		bul->renderer.position.y += bul->velocity.y * dt;
		bul->bulletLifetime -= dt;
		if (bul->bulletLifetime < 0)
		{
			bul->bulletLifetime = BULLET_LIFETIME;
			bul->active = 0;
		}
	}

}
void BulletListUpdate(VariableList* bulletlist, float dt)
{
	for (int i = 0; i < bulletlist->length; ++i)
	{
		BulletUpdate(VariableListGetItemAt(bulletlist, i), dt);
	}
}
void BulletListRender(VariableList* bulletlist, Camera* cam)
{
	for (int i = 0; i < bulletlist->length; ++i)
	{
		Bullet* bullet = VariableListGetItemAt(bulletlist, i);
		if(bullet->active)
			Render_Image(cam, &bullet->renderer);
	}
}
//check against all objects
void BulletListCheckCollision(VariableList* bulletlist,VariableList* listofcharacter, float dt)
{
	
	for (int i = 0; i < bulletlist->length; ++i)
	{
		Bullet* bullet = VariableListGetItemAt(bulletlist, i);
		if (bullet->active)
		{
			//UpdateBullet
			for (int j = 0; j < listofcharacter->length; ++j)
			{
				Character* item = VariableListGetItemAt(listofcharacter,j);
				if (item == NULL)
					continue;
				
				if (BoxColliderCheckCollision(bullet->collider, bullet->renderer.position, item->collider, *CharacterGetPosition(item)))
				{
					bullet->active = 0;
				}
			}
		}
	}
}
//check against one object and runs the function ptr
void BulletListCheckCollisionAgainst(VariableList* bulletlist, Character* character, float dt, BulletCallback func)
{
	if (!character->active)
		return;
	for (int i = 0; i < bulletlist->length; ++i)
	{
		Bullet* bullet = VariableListGetItemAt(bulletlist, i);
		if (bullet->active)
		{
			if (BoxColliderCheckCollision(bullet->collider, bullet->renderer.position, character->collider, *CharacterGetPosition(character)))
			{
				bullet->active = 0;
				func(bullet, character);
			}
		}
	}
}