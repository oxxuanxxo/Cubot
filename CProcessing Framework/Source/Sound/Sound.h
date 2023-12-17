/*!
@file		Sound.h
@author		Leong Jun Xiang	(junxiang.leong)
@course		CSD1400
@brief		a manager system for the sound assets, able to play sounds at
			different pitches
			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/
#ifndef SOUND_H
#define SOUND_H
#include "../../CProcessing/inc/cprocessing.h"
#include "../Misc/VariableList.h"

typedef struct Sound
{
	CP_Sound sound;
	float volume;
	float pitch_max;
	float pitch_min;
}Sound;
typedef enum SOUNDEFFECT
{
	SOUND_COWBELL = 0,
	SOUND_DASH,
	SOUND_POWERUP,
	SOUND_LAND,
	SOUND_HIT,
	SOUND_ADDHEALTH,//new
	SOUND_NEWDASH, //new
	SOUND_JUMP, //new
	SOUND_RESTART, //new
	SOUND_SELECTBTN, //new
	SOUND_BOUNCE,
	SOUND_WIN,
}SOUNDEFFECT;

static VariableList g_soundList;
void SoundLoad(void);
static Sound* CreateSound(const char* file , float min_pitch,float max_pitch,float volume);
void PlaySoundEffect(SOUNDEFFECT sound);

void FreeSound(void);
#endif
