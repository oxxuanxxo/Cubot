/*!
@file		Character.c
@author		Leong Jun Xiang	(junxiang.leong)
@course		CSD1400
@brief		the base data type for all the Enemies , player
			handles rendering and collision between all characters
			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/
#include <stdlib.h>
#include "Character.h"
#include "../GlobalDefine.h"
#include "../Render/RenderUtil.h"
Character* CreateCharacter(void* mainObject,CP_Vector pos, CP_Vector collider_offset, float width, float height, CharacterTag tag,IMAGE_TYPE imgtype)
{
	

	Character* object = malloc(sizeof(Character));
	if (object)
	{
		object->CollisionCallBack = NULL;
		object->SetDead = NULL;
		object->mainObject = mainObject;
		BoxColliderSet(&object->collider, collider_offset,width,height);
		object->active = 1;
		object->tag = tag;
		object->renderer.position.x = pos.x;
		object->renderer.position.y = pos.y;
		object->renderer.scale.x = width;
		object->renderer.scale.y = height;
		object->renderer.image = imgtype;
		object->renderer.alpha = 255;
		object->renderer.rotation = 0.0f;
		object->renderer.numCol = 0;
		object->renderer.numRow = 0;
		

		VariableListCreate(&object->particleSystemList, 1);

		
		VariableListAdd(&g_characterList, object);
		return object;
	}
	return NULL;
}
float CharacterGetWidth(Character* c)
{
	return c->collider.width;
}
float CharacterGetHeight(Character* c)
{
	return c->collider.height;
}
CP_Vector* CharacterGetPosition(Character* c)
{
	return &c->renderer.position;
}
void CharacterRender(const Camera* cam,const Character* c)
{
	if (c->active)
	{
		if (c->renderer.numCol == 0)
			Render_Image(cam, &c->renderer);
		else
		{	
			Render_Sprite(cam, &c->renderer);
		}
		for (int i = 0; i < c->particleSystemList.length; ++i)
		{
			ParticleSystem* p_system = VariableListGetItemAt(&c->particleSystemList, i);
			if(p_system)
				ParticleSystem_Render(cam, p_system);
		}
	}
}
void CharacterUpdate(Character* c)
{
	for (int i = 0; i < c->particleSystemList.length; ++i)
	{
		ParticleSystem* p_system = VariableListGetItemAt(&c->particleSystemList, i);
		if (p_system)
			ParticleSystem_Update(p_system);
	}
}
void CharacterListUpdate(VariableList* characterlist, float dt)
{
	
}
//handles all collision using g_characterlist
void CharacterListCollision(VariableList* characterlist)
{
	Character* item;
	Character* item2;
	for (int i = 0; i < characterlist->length; ++i)
	{
		item = VariableListGetItemAt(characterlist, i);
		if (item->active == 0)
			continue;
		else
			CharacterUpdate(item);
		for (int j = i+1; j < characterlist->length; ++j)
		{
			item2 = VariableListGetItemAt(characterlist, j);
			if (item2->active)
			{
				if (BoxColliderCheckCollision(item->collider, *CharacterGetPosition(item), item2->collider, *CharacterGetPosition(item2)))
				{
					//do smth here
					if(item->CollisionCallBack)
						item->CollisionCallBack(item, item2);
					if(item2->CollisionCallBack)
						item2->CollisionCallBack(item2, item);
				}
			}
		}
	}
}
void CharacterListRender(VariableList* characterlist, Camera* cam)
{
	for (int i = 0; i < characterlist->length; ++i)
	{
		Character* c = VariableListGetItemAt(characterlist, i);
		CharacterRender(cam, c);
	}
}
void AddCallBack(Character* c, void (*FunctionPtr)(void* mainObject, void* targetObject))
{
	c->CollisionCallBack = FunctionPtr;
}
void AddDeadCallBack(Character* c, void (*SetDead)(void* character))
{
	c->SetDead = SetDead;
}
void CharacterCreateParticles(ParticleSystem* p, int amount, CP_Vector position)
{
	p->position = position;
	p->numParticles = amount;
}

void CharacterListDelete(VariableList* characterList)
{
	for (int i = 0; i < characterList->length; ++i)
	{
		Character* c = VariableListGetItemAt(characterList, i);
		for (int j = 0; j < c->particleSystemList.length; ++j)
		{
			ParticleSystem* p_system = VariableListGetItemAt(&c->particleSystemList, j);
			if (p_system)
			{
				ParticleSystem_Destroy(p_system);
				p_system = NULL;
			}
		}
		VariableListFree(&c->particleSystemList);
	}
	VariableListFree(characterList);
}