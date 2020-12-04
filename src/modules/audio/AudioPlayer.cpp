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
        //Load all of the sounds, so that they are only loaded once, which is fine as they are used throughout all of the game
        jumpSounds.push_back(Mix_LoadWAV("Assets/Audio/Jump/jump1.wav"));
        jumpSounds.push_back(Mix_LoadWAV("Assets/Audio/Jump/jump2.wav"));
        jumpSounds.push_back(Mix_LoadWAV("Assets/Audio/Jump/jump3.wav"));
        bowSounds.push_back(Mix_LoadWAV("Assets/Audio/Bow/shoot1.wav"));
        bowSounds.push_back(Mix_LoadWAV("Assets/Audio/Bow/shoot2.wav"));
        arrowHitSounds.push_back(Mix_LoadWAV("Assets/Audio/Bow/hit.wav"));
        arrowReturnSounds.push_back(Mix_LoadWAV("Assets/Audio/Bow/return.wav"));
        
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
            startRandomSound(sound, 0, 1, volume);
            break;
        }
        case SoundType::PlayerJumping:
            startRandomSound(sound, 0, 1, volume);
            break;
        case SoundType::ArrowHitting:
            startSound(sound, 0, volume);
            break;
        case SoundType::ArrowReturning:
            startSound(sound, 0, volume);
            break;
        }
}

void AudioPlayer::startRandomSound(SoundType type, int min, int max, int volume) {
    auto num = min + (rand() % (max - min + 1));
    startSound(type, num, volume);
}

void AudioPlayer::startSound(SoundType type, int index, int volume) {
    switch (type) {
        case SoundType::BowShooting: {
            if (volume == 0) Mix_VolumeChunk(bowSounds[index], standardSoundVolume);
            else Mix_VolumeChunk(bowSounds[index], volume);
            Mix_PlayChannel(-1, bowSounds[index], 0);
            break;
        }
        case SoundType::PlayerJumping:
            if (volume == 0) Mix_VolumeChunk(jumpSounds[index], standardSoundVolume);
            else Mix_VolumeChunk(jumpSounds[index], volume);
            Mix_PlayChannel(-1, jumpSounds[index], 0);
            break;
        case SoundType::ArrowHitting:
            if (volume == 0) Mix_VolumeChunk(arrowHitSounds[index], standardSoundVolume);
            else Mix_VolumeChunk(arrowHitSounds[index], volume);
            Mix_PlayChannel(-1, arrowHitSounds[index], 0);
            break;
        case SoundType::ArrowReturning:
            if (volume == 0) Mix_VolumeChunk(arrowReturnSounds[index], standardSoundVolume);
            else Mix_VolumeChunk(arrowReturnSounds[index], volume);
            Mix_PlayChannel(-1, arrowReturnSounds[index], 0);
            break;
    }
}


