/*!
@file		BoxCollider.h
@author		Leong Jun Xiang	(junxiang.leong)
@course		CSD1400
@brief		a generic aabb collision
Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/
#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H
#include "../../CProcessing/inc/cprocessing.h"
//basic box collider
typedef struct BoxCollider
{
	float width;
	float height;
	CP_Vector offset;

}BoxCollider;

void BoxColliderSet(BoxCollider* collider,CP_Vector offset, float w, float h);
//void BoxColliderCopy(struct BoxCollider old, struct BoxCollider new);

int BoxColliderCheckCollision(BoxCollider a, CP_Vector vec_a, BoxCollider b, CP_Vector vec_b);
#endif // !BOXCOLLIDER_H
