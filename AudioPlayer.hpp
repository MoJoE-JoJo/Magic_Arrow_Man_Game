#pragma once
#include "sre/SDLRenderer.hpp"
#include "SDL_mixer.h"

//enum class GameState { Won, Running };
enum class SoundType{ BowShooting, ArrowHitting, PlayerJumping};
enum class MusicType{ Background_Game, Background_Menu};

class AudioPlayer {
public:

	
private:
	void init();
	void startBackgroundMusic(MusicType music);
	void stopBackgroundMusic();
	void playSound(SoundType sound);

	bool musicPlaying = false;

	friend class MAMGame;
};