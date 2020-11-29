#pragma once
#include <string>
#include <time.h>
#include <stdio.h>
#include <stdlib.h> 

#include "AudioPlayer.hpp"

using namespace sre;
using namespace std;

void AudioPlayer::init(int musicVolume, int standardSoundVolume) {
    // Mix_OpenAudio(int frequency, Uint16 format,
    // int channels, int chunksize)
    if (!initialized) {
        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
            cout << "Cannot initialize audio output" << endl;
            return;
        }
        this->musicVolume = musicVolume;
        this->standardSoundVolume = standardSoundVolume;
        initialized = true;
    }
}

void AudioPlayer::startBackgroundMusic(MusicType music, int volume) {
    if (!initialized) {
        cout << "AudioPlayer not initialized" << endl;
        return;
    }
    if (!musicPlaying) {
        switch (music) {
            Mix_Music* music;
            case MusicType::Background_Game: 
                music = Mix_LoadMUS("Assets/Audio/Music/bg_game.wav");
                if (music == nullptr) {
                    cout << "Cannot load music" << endl;
                    return;
                }
                musicPlaying = true;
                if (volume == 0) Mix_VolumeMusic(musicVolume);
                else Mix_VolumeMusic(volume);
                Mix_PlayMusic(music, -1);

                break;
        }
    }
}

void AudioPlayer::stopBackgroundMusic() {
    Mix_HaltMusic();
    musicPlaying = false;
}

void AudioPlayer::playSound(SoundType sound, int volume) {
    switch (sound) {
        case SoundType::BowShooting: {
            startRandomSound("Assets/Audio/Bow", "bow", "wav", 1, 3, volume);
            break;
        }
        case SoundType::PlayerJumping:
            startRandomSound("Assets/Audio/Jump", "jump", "wav", 1, 3, volume);
            break;
        case SoundType::ArrowHitting:
            startSound("Assets/Audio/Bow/hit.wav", volume);
            break;
    }
}

void AudioPlayer::startRandomSound(char* folderName, char* fileName, char* fileType, int min, int max, int volume) {
    auto num = min + (rand() % (max - min + 1));
    auto str = (std::string(folderName) + "/" + fileName + std::to_string(num) + "." + fileType);
    const char* c = str.data();
    startSound(c, volume);
}

void AudioPlayer::startSound(const char* file, int volume) {
    Mix_Chunk* sound;
    sound = Mix_LoadWAV(file);
    if (sound == nullptr) {
        cout << "Cannot load sound" << endl;
        return;
    }
    if (volume == 0) Mix_VolumeChunk(sound, standardSoundVolume);
    else Mix_VolumeChunk(sound, volume);
    Mix_PlayChannel(-1, sound, 0);
}


