
/*!
@file		Character.h
@author		Leong Jun Xiang	(junxiang.leong)
@course		CSD1400
@brief		declaration for all Character based function and struct
			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/
#ifndef CHARACTER_H
#define CHARACTER_H

#include "../../CProcessing/inc/cprocessing.h"
#include "../Misc/BoxCollider.h"
#include "../Misc/VariableList.h"
#include "../GlobalDefine.h"
#include "../Render/Renderer.h"
#include "../Camera/Camera.h"
#include "../Render/ParticleSystem.h"
typedef struct Character
{
	//if its active it will update
	int active;
	//enum define in globaldefine
	CharacterTag tag;
	Renderer renderer;
	//checks for collision
	BoxCollider collider;

	VariableList particleSystemList;
	//address to the mainobject eg(player,enemycircle)
	void* mainObject;
	//fucntion pointer that have void return type void and takes in 2 void* 
	void (*CollisionCallBack)(void* mainObject, void* targetObject);

	void (*SetDead)(void* character);
}Character;

//takes in position and the name of the character(give a unique name)
//collider_offset is position for the collider away from the center
//IMAGE_TYPE is an enum declared in GlobalDefine.h
//CharacterTag is an enum de
Character* CreateCharacter(void* mainObject,CP_Vector pos, CP_Vector collider_offset, float width, float height, CharacterTag tag,IMAGE_TYPE image);

float CharacterGetWidth(Character* c);
float CharacterGetHeight(Character* c);

//returns the address of the position 
CP_Vector* CharacterGetPosition(Character* c);

void CharacterRender(const Camera* cam, const Character* c);
//save for physics
void CharacterUpdate(Character* c);
// CharacterList Functions
void CharacterListUpdate(VariableList* characterlist, float dt);
//collision checking for whole list
void CharacterListCollision(VariableList* characterlist);
void CharacterListRender(VariableList* characterlist, Camera* cam);
void AddCallBack(Character* c, void (*FunctionPtr)(void* mainObject, void* targetObject));
void AddDeadCallBack(Character* c, void (*SetDead)(void* character));

void CharacterCreateParticles(ParticleSystem* ps, int amount, CP_Vector position);

void CharacterListDelete(VariableList* characterList);
#endif