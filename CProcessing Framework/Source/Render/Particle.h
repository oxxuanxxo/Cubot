/*!
@file		Particle.h
@author		Daniel Chua Yee Chen (yeechendaniel.chua)
@course		CSD1400
@brief		Contains implementations for functions used for
			each particle.

			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/


#ifndef _PARTICLE_H
#define _PARTICLE_H

#include "Renderer.h"
#include <stdbool.h>


typedef struct Particle
{
	Renderer mesh;
	CP_Vector velocity;
	float speed;
	float lifeTime;

	bool enabled;

} Particle;


void Particle_Update(Particle* particle);

#endif