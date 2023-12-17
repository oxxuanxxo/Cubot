/*!
@file		BoxCollider.c
@author		Leong Jun Xiang	(junxiang.leong)
@course		CSD1400
@brief		a generic aabb collision
Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/
#include "BoxCollider.h"



void BoxColliderSet(BoxCollider* collider, CP_Vector offset, float w, float h)
{
	collider->width = w;
	collider->height = h;
	collider->offset.x = offset.x;
	collider->offset.y = offset.y;
}
int BoxColliderCheckCollision(BoxCollider a, CP_Vector vec_a, BoxCollider b, CP_Vector vec_b)
{
	float aw, ah, bw, bh;
	aw = a.width * 0.5f;
	ah = a.height * 0.5f;
	bw = b.width * 0.5f;
	bh = b.height * 0.5f;
	return ((vec_a.x + a.offset.x ) >= (vec_b.x - bw - aw + b.offset.x)) &&
		((vec_b.x + b.offset.x) >= (vec_a.x - aw - bw + a.offset.x)) &&
		((vec_a.y + a.offset.y) >= (vec_b.y - bh - ah + b.offset.y)) &&
		((vec_b.y + b.offset.y) >= (vec_a.y - ah - bh + a.offset.y));
}