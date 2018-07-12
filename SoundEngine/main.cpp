#include "SoundEngine.h"

int main()
{
	SoundEngine sound;
	FMOD_VECTOR va = { 0,0,0 };

	sound.init();
	sound.loadSounds("fire.mp3", false, true, true);
	sound.playSounds("fire.mp3", va);

	sound.update();

	getchar();
}