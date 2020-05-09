#pragma once

#ifndef _SOUND_ENGINE_H_
#define _SOUND_ENGINE_H_

//Sound Engine
//Naming convention: m for map, p for local variables, v for vectors
#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <string>

#include "fmod_studio.hpp"
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
	FMOD::Studio::System* studioSystem;

	int nextChannel;
	
	void Update();

	typedef std::map <std::string, FMOD::Sound*> sound_map;
	typedef std::map <int, FMOD::Channel*> channel_map;
	typedef std::map <std::string, FMOD::Studio::EventInstance*> event_map;
	typedef std::map <std::string, FMOD::Studio::Bank*> bank_map;
	
	bank_map mBanks;
	event_map mEvents;
	sound_map mSounds;
	channel_map mChannels;
};


class SoundEngine
{
public:
	static int Error(FMOD_RESULT result);

	static void init();
	static void update();
	static void shutdown();

	//functions for FMOD Studio API only
	void loadBank(const std::string&, FMOD_STUDIO_LOAD_BANK_FLAGS);
	void loadEvent(const std::string&);
	void playEvent(const std::string&);
	void stopEvent(const std::string&, bool immediate = false);
	void getEventParam(const std::string&, const std::string&, float*);
	void setEventParam(const std::string&, const std::string&, float);
	
	//why is this const?
	bool isEventPlaying(const std::string&) const;
	
	//functions for FMOD API
	int playSounds(const std::string&, const Vector3&, float);
	void loadSounds(const std::string&, bool, bool, bool);
	void unloadSounds(const std::string&);
	void set3DListenAndOrientation(const Vector3& vPos = Vector3{ 0, 0, 0 }, float fVoldB = 0.0f);
	void setChannel3DPos(int, const Vector3&);
	void setChannelVol(int channelID, float voldB);
	void stopChannel(int);
	void stopAllChannels();

	void printChannel();

	void isPlaying(int) const;

	FMOD_VECTOR Vec3toFMOD(const Vector3&);
	float dbToVolume(float);
	float VolumeTodB(float);
};

#endif // !_SOUND_ENGINE_H