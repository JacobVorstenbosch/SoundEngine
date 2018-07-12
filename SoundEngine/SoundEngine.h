#pragma once


//Sound Engine
//Naming convention: m for map, p for local variables
#include <iostream>
#include <map>
#include <vector>
#include <math.h>

#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"

struct Vector3
{
	float x, y, z;
};

struct SoundSystem
{
	SoundSystem();
	~SoundSystem();

	FMOD::System *system;

	int nextChannel;
	
	void Update();

	typedef std::map <std::string, FMOD::Sound*> sound_map;
	typedef std::map <int, FMOD::Channel*> channel_map;
	
	sound_map mSounds;
	channel_map mChannels;
};


class SoundEngine
{
public:
	static int Error(FMOD_RESULT result);

	void init();
	void update();

	void loadSounds(const std::string&, bool, bool, bool);
	void unloadSounds(const std::string&);
	int playSounds(const std::string&, const FMOD_VECTOR&);

	FMOD_VECTOR Vec3toFMOD(const Vector3&);
	float dbToVolume(float);
	float VolumeTodB(float);
};