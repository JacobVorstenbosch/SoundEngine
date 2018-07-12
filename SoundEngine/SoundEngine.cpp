/* Reference from https://codyclaborn.me/tutorial/making-a-basic-fmod-audio-engine-in-c.html#implementation */
/* http://gdcvault.com/play/1022060/How-to-Write-an-Audio */
#include "SoundEngine.h"

int SoundEngine::Error(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		return 1;
	}
	return 0;
}

SoundSystem* ssObject = nullptr;

SoundSystem::SoundSystem()
{
	system = NULL;
	SoundEngine::Error(FMOD::System_Create(&system));
	SoundEngine::Error(system->init(32, FMOD_INIT_NORMAL, 0));
}

SoundSystem::~SoundSystem()
{
	SoundEngine::Error(system->release());
}


//HOW DOES THIS WORK WTF
void SoundSystem::Update()
{
	std::vector<channel_map::iterator> stoppedChannels;
	for (auto it = mChannels.begin(), end = mChannels.end(); it != end; ++it)
	{
		bool pisPlaying = false;
		it->second->isPlaying(&pisPlaying);
		if (!pisPlaying)
		{
			stoppedChannels.push_back(it);
		}
	}
	for (auto& it : stoppedChannels)
	{
		mChannels.erase(it);
	}
	SoundEngine::Error(system->update());
}

//instance of the SoundSystem object - this allows the SoundEngine to access its functionality

void SoundEngine::init()
{
	ssObject = new SoundSystem;
}

void SoundEngine::update()
{
	ssObject->Update();
}

void SoundEngine::loadSounds(const std::string& pFile, bool is3D, bool isLoop, bool isStream)
{
	auto foundFile = ssObject->mSounds.find(pFile);
	if (foundFile != ssObject->mSounds.end())
		return;

	//setting the modes dependent on how you set them above
	FMOD_MODE sMode = FMOD_DEFAULT;
	sMode |= is3D ? FMOD_3D : FMOD_2D;
	sMode |= isLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	sMode |= isStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	FMOD::Sound* pSound = nullptr;
	std::string pPath = "media//";
	pPath.append(pFile.c_str());
	SoundEngine::Error(ssObject->system->createSound(pPath.c_str(), sMode, nullptr, &pSound));

	//if there is a sound added, add it to the map as its designated name and sound file name (NOT THE DAMN /MEDIA/
	if (pSound)
		ssObject->mSounds[pFile] = pSound;
}

void SoundEngine::unloadSounds(const std::string& pFile)
{
	auto foundFile = ssObject->mSounds.find(pFile);
	//if (foundFile != ssObject->mSounds.end())
	//	return;

	SoundEngine::Error(foundFile->second->release());
	ssObject->mSounds.erase(foundFile);
}

//debating taking out the Vector3, there's no reason for it - just have the FMOD_VECTOR
//be warned, FMOD_VECTOR is left handed - +z is into the screen, negative is toward user
int SoundEngine::playSounds(const std::string& pFile, const FMOD_VECTOR& vPos)
{
	int channelID = ssObject->nextChannel++;

	auto foundFile = ssObject->mSounds.find(pFile);
	if (foundFile != ssObject->mSounds.end())
	{
		//loadSounds(pFile) ; //-> the tutorial seems to have fucked up
		foundFile = ssObject->mSounds.find(pFile);
		if (foundFile == ssObject->mSounds.end())
			return channelID;
	}

	FMOD::Channel* pChannel = nullptr;
	SoundEngine::Error(ssObject->system->playSound(foundFile->second, nullptr, true, &pChannel));
	if (pChannel)
	{
		FMOD_MODE pCurrentMode;
		foundFile->second->getMode(&pCurrentMode);
		if (pCurrentMode & FMOD_3D)
		{
			//FMOD_VECTOR pos = Vec3toFMOD(vPos);
			SoundEngine::Error(pChannel->set3DAttributes(&vPos, NULL));
		}
		SoundEngine::Error(pChannel->setVolume(1.0f)); //add actual funtion in a bit
		SoundEngine::Error(pChannel->setPaused(false));
		ssObject->mChannels[channelID] = pChannel;
	}
	return channelID;
}


//Not sure why he has this function
FMOD_VECTOR SoundEngine::Vec3toFMOD(const Vector3& vec)
{
	FMOD_VECTOR fvec3;
	fvec3.x = vec.x;
	fvec3.y = vec.y;
	fvec3.z = vec.z;
	return fvec3;
}

float SoundEngine::dbToVolume(float dB)
{
	return powf(10.0f, 0.05f * dB);
}

float  SoundEngine::VolumeTodB(float volume)
{
	return 20.0f * log10f(volume);
}