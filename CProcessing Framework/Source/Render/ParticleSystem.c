/*!
@file		ParticleSysteem.c
@author		Daniel Chua Yee Chen (yeechendaniel.chua)
@course		CSD1400
@brief		Contains implementations for functions used to generate
			a particle emitter system.

			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/


#include "ParticleSystem.h"
#include "RenderUtil.h"
#include <stdlib.h>

ParticleSystem* CreateParticleSystem(float emissionRate, float emissionAngle, float lifeTime, float speed, float size,CP_Vector position , CP_Vector direction, IMAGE_TYPE image, int alpha)
{
	ParticleSystem* result = (ParticleSystem*)malloc(sizeof(ParticleSystem));
	if (result)
	{
		result->emissionRate = emissionRate;
		result->emissionAngle = emissionAngle;
		result->lifeTime = lifeTime;
		result->speed = speed;
		result->numParticles = 0;
		
		if (direction.x == 0 && direction.y == 0)
		{
			direction = CP_Vector_Set(1, 0);
		}
		result->direction = CP_Vector_Normalize(direction);
		result->position = position;
		result->size = size;
		result->particleImage = image;
		result->alpha = alpha;
		VariableListCreate( &(result->particleList), (int)(emissionRate * (lifeTime +1)));
		for (int i = 0; i < (int)(emissionRate * (lifeTime + 1) ); ++i)//+1 for padding
		{
			Particle* p = (Particle*)malloc(sizeof(Particle));
			if (p)
			{
				p->enabled = false;
				p->mesh.image = image;
				p->mesh.alpha = alpha;
				p->mesh.rotation = 0.0f;
				
				VariableListAdd(&(result->particleList), p);
			}
			
		}
	}
	return result;
}

static Particle* FindAvailableParticle(ParticleSystem* pSystem)
{
	for (int i = 0; i < pSystem->particleList.length; ++i)
	{
		Particle* p = (Particle*)VariableListGetItemAt(&(pSystem->particleList), i);
		if (p->enabled == false)
		{
			return p;
		}
	}
	return NULL;

}

void ParticleSystem_Update(ParticleSystem* pSystem)
{
	pSystem->timer -= CP_System_GetDt();

	if (pSystem->numParticles != 0)
	{
		//check for spawning particles.
		if (pSystem->timer < 0.0f)
		{
			pSystem->timer = 1.0f / pSystem->emissionRate;
			Particle* p = FindAvailableParticle(pSystem);
			if (p)
			{
				p->enabled = true;
				p->lifeTime = pSystem->lifeTime;
				p->mesh.image = pSystem->particleImage;
				p->mesh.position = pSystem->position;
				float angle = CP_Random_RangeFloat(-(pSystem->emissionAngle), pSystem->emissionAngle);
				p->velocity = CP_Vector_MatrixMultiply(CP_Matrix_Rotate(angle), pSystem->direction);
				p->speed = pSystem->speed;
				p->mesh.scale = CP_Vector_Set(pSystem->size, pSystem->size);
				p->mesh.alpha = pSystem->alpha;

				pSystem->numParticles--;
				if (pSystem->numParticles < 0)
				{
					pSystem->numParticles = -1;
				}

			}
		}
	}
	for (int i = 0; i < pSystem->particleList.length; ++i)
	{
		Particle* p = (Particle*)VariableListGetItemAt(&(pSystem->particleList), i);
		if (p->enabled == true)
		{
			Particle_Update(p);
		}
	}
}

void ParticleSystem_Render(Camera* cam, ParticleSystem* pSystem)
{
	for (int i = 0; i < pSystem->particleList.length; ++i)
	{
		Particle* p = (Particle*)VariableListGetItemAt(&(pSystem->particleList), i);
		if (p->enabled == true)
		{
			Render_Image(cam, &(p->mesh) );
		}
	}
}

void ParticleSystem_Destroy(ParticleSystem* pSystem)
{
	VariableListFree(&(pSystem->particleList) );
	free(pSystem);
}