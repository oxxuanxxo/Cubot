/*!
@file		ParticleSysteem.h
@author		Daniel Chua Yee Chen (yeechendaniel.chua)
@course		CSD1400
@brief		Contains implementations for functions used to generate
			a particle emitter system.

			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/


#ifndef _PARTICLE_SYSTEM_H
#define _PARTICLE_SYSTEM_H

#include "Particle.h"
#include "../Misc/VariableList.h"
#include "../GlobalDefine.h"
#include "../Camera/Camera.h"

typedef struct ParticleSystem
{
	//list containing all the particles.
	VariableList particleList;
	//Number of particles to be spawned every second.
	float emissionRate;
	//Maximum angle away from the velocity specified
	float emissionAngle;
	//the life time of each particle
	float lifeTime;
	//position of the emitter
	CP_Vector position;
	//The direction in which the particles will move
	CP_Vector direction;
	//image of each particle
	IMAGE_TYPE particleImage;
	//speed of each particle
	float speed;
	//size of each particle
	float size;
	//alpha of each image
	int alpha;
	//number of particles to emit, if this is set to -1, the system will emit limitlessly.
	int numParticles;

	//private variable to be used as a timer for emitting particles
	float timer;

} ParticleSystem;

ParticleSystem* CreateParticleSystem(float emissionRate, float emissionAngle, float lifeTime, float speed, float size, CP_Vector position ,CP_Vector direrction, IMAGE_TYPE image, int alpha);
void ParticleSystem_Update(ParticleSystem* pSystem);
void ParticleSystem_Render(Camera* cam, ParticleSystem* pSystem);
void ParticleSystem_Destroy(ParticleSystem* pSystem);

#endif