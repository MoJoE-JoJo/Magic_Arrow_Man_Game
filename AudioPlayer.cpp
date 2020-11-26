#pragma once

#include "AudioPlayer.hpp"

using namespace sre;
using namespace std;

void AudioPlayer::init() {
    // Mix_OpenAudio(int frequency, Uint16 format,
// int channels, int chunksize)
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        cout << "Cannot initialize audio output" << endl;
        return;
    }
}

void AudioPlayer::startBackgroundMusic(MusicType music) {
    if (musicPlaying) stopBackgroundMusic();
    switch (music) {
        Mix_Music* music;
    case MusicType::Background_Game: {
        music = Mix_LoadMUS("Audio/Music/bg_game.ogg");
        if (music == nullptr) {
            cout << "Cannot load music" << endl;
            return;
        }
        musicPlaying = true;
        Mix_PlayMusic(music, -1);

        break;
    }
    case MusicType::Background_Menu: {
        music = Mix_LoadMUS("Audio/Music/bg_menu.wav");
        if (music == nullptr) {
            cout << "Cannot load music" << endl;
            return;
        }
        musicPlaying = true;
        Mix_PlayMusic(music, -1);

        break;
    }
    }
}

void AudioPlayer::stopBackgroundMusic() {
    Mix_HaltMusic();
}

void AudioPlayer::playSound(SoundType sound) {
    // Mix_LoadWAV(char *file)
    Mix_Chunk* high = Mix_LoadWAV("Audio/Bow/bow_1.wav");

    // Mix_PlayChannel(int channel, Mix_Chunk *chunk, int loops)
    Mix_PlayChannel(-1, high, 0);
}
