#pragma once
#include "sre/SDLRenderer.hpp"
#include "SDL_mixer.h"

//enum class GameState { Won, Running };
enum class SoundType{ BowShooting, ArrowHitting, PlayerJumping};
enum class MusicType{ Background_Game, Background_Menu};

class AudioPlayer {
public:
	void startBackgroundMusic(MusicType music, int volume);
	void stopBackgroundMusic();
	void playSound(SoundType sound, int volume);

	int musicVolume;
	int standardSoundVolume;
private:
	void init(int musicVolume, int standardSoundVolume);
	void startRandomSound(char* folder, char* fileName, char* fileType, int minNumber, int maxNumber, int volume);
	void startSound(const char* file, int volume);

	bool musicPlaying = false;
	bool initialized = false;

	friend class MAMGame;
};