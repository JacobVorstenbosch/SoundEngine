#include "SoundEngine.h"

int main()
{
	SoundEngine sound;
	Vector3 va = { 0,0,0 };

	sound.init();
	sound.loadSounds("fire.mp3", false, true, true);
	sound.playSounds("fire.mp3", va, 10.0f);

	sound.update();
	getchar();
}