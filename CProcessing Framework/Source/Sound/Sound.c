/*!
@file		Sound.c
@author		Leong Jun Xiang	(junxiang.leong)
@course		CSD1400
@brief		a manager system for the sound assets, able to play sounds at
			different pitches
			Copyright © 2020 DigiPen, All rights reserved.
*//*_________________________________________________________________________*/
#include "Sound.h"
#include <stdlib.h>
void SoundLoad(void)
{
	VariableListCreate(&g_soundList, 14);
	VariableListAdd(&g_soundList, CreateSound("./Assets/Cowbell.wav",3.0f, 4.0f, 1));
	VariableListAdd(&g_soundList, CreateSound("./Assets/Dash.wav", 1.0f, 2.0f, 1));
	VariableListAdd(&g_soundList, CreateSound("./Assets/Powerup.wav", 1.5f, 2.5f, 0.5f));
	VariableListAdd(&g_soundList, CreateSound("./Assets/Land.wav", 1.0f, 2.0f, 1));
	VariableListAdd(&g_soundList, CreateSound("./Assets/Hit.wav", 0.1f, 1.0f, 1));

	//new
	VariableListAdd(&g_soundList, CreateSound("./Assets/NewAddHealth.wav", 1.0f, 1.0f, 1.0f));
	VariableListAdd(&g_soundList, CreateSound("./Assets/NewDash.wav", 0.8f, 0.8f, 0.5f));
	VariableListAdd(&g_soundList, CreateSound("./Assets/NewJump.wav", 1.0f, 1.0f, 0.7f));
	VariableListAdd(&g_soundList, CreateSound("./Assets/NewRestart.wav", 0.8f, 0.8f, 0.15f));
	VariableListAdd(&g_soundList, CreateSound("./Assets/NewSelectButton.wav", 1.0f, 1.0f, 1.0f));
	VariableListAdd(&g_soundList, CreateSound("./Assets/Bounce.wav", 0.9f, 0.9f, 0.5f));

	VariableListAdd(&g_soundList, CreateSound("./Assets/Winning.mp3", 1.0f, 1.0f, 1.0f));
}
static Sound* CreateSound(const char* file, float min_pitch, float max_pitch, float volume)
{
	Sound* s = malloc(sizeof(Sound));
	if (s)
	{
		CP_Sound sound = CP_Sound_Load(file);
		s->sound = sound;
		s->pitch_min = min_pitch;
		s->pitch_max = max_pitch;
		s->volume = volume;
		return s;
	}
	return NULL;
}
void PlaySoundEffect(SOUNDEFFECT sound)
{
	Sound *s = VariableListGetItemAt(&g_soundList, sound);
	float range = CP_Random_RangeFloat(s->pitch_min, s->pitch_max);
	CP_Sound_PlayAdvanced(s->sound, s->volume, range, 0, CP_SOUND_GROUP_0);
}
void FreeSound(void)
{
	for (int i = 0; i < g_soundList.length; ++i)
	{
		Sound *s = VariableListGetItemAt(&g_soundList, i);
		CP_Sound_Free(s->sound);
	}
	VariableListFree(&g_soundList);
}