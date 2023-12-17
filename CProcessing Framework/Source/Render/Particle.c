/*!
@file		Particle.c
@author		Daniel Chua Yee Chen (yeechendaniel.chua)
@course		CSD1400
@brief		Contains implementations for functions used for
			each particle.

			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/

#include "Particle.h"

void Particle_Update(Particle* particle)
{
	if (particle->enabled == true)
	{
		particle->lifeTime -= CP_System_GetDt();
		if (particle->lifeTime < 0)
		{
			particle->enabled = false;
		}

		particle->mesh.position = CP_Vector_Add(particle->mesh.position, CP_Vector_Scale(particle->velocity, particle->speed * CP_System_GetDt()));
	}

}