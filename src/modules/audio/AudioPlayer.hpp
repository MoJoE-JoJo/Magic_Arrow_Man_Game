#pragma once
#include "sre/SDLRenderer.hpp"
#include "SDL_mixer.h"

enum class SoundType { BowShooting, ArrowHitting, PlayerJumping, ArrowReturning };
enum class MusicType { Background_Game };

class AudioPlayer {
public:
	void startBackgroundMusic(MusicType music, int volume);
	void stopBackgroundMusic();
	void playSound(SoundType sound, int volume);

	int musicVolume;
	int standardSoundVolume;
private:
	void init(int musicVolume, int standardSoundVolume);
	void startRandomSound(SoundType type, int minNumber, int maxNumber, int volume);
	void startSound(SoundType type, int index, int volume);

	bool musicPlaying = false;
	bool initialized = false;
	std::vector<Mix_Chunk*> jumpSounds;
	std::vector<Mix_Chunk*> bowSounds;
	std::vector<Mix_Chunk*> arrowHitSounds;
	std::vector<Mix_Chunk*> arrowReturnSounds;

	friend class MAMGame;
};