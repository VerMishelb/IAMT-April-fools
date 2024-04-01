#include "../include/MusicPlayer.h"
#include "SDL_mixer.h"
#include <stdio.h>
#include "../include/mus_music3.h"
#include "../include/snd_gunsh.h"
#include "../include/snd_oo.h"

MusicPlayer::MusicPlayer() :
    mus_bg(nullptr),
    snd_gunsh(nullptr),
    snd_oo(nullptr)
{
    
}

MusicPlayer::~MusicPlayer() {
    Mix_FreeMusic(mus_bg);
}

void MusicPlayer::init() {
    Mix_AllocateChannels(32);

    SDL_RWops* mus_file = SDL_RWFromConstMem(data_music3, data_music3_size);
    mus_bg = Mix_LoadMUS_RW(mus_file, 1);
    if (!mus_bg) {
        fprintf_s(stdout, "%s", Mix_GetError());
    }

    mus_file = SDL_RWFromConstMem(data_snd_gunsh, data_snd_gunsh_size);
    snd_gunsh = Mix_LoadWAV_RW(mus_file, 1);
    if (!snd_gunsh) {
        fprintf_s(stdout, "%s", Mix_GetError());
    }

    mus_file = SDL_RWFromConstMem(data_snd_oo, data_snd_oo_size);
    snd_oo = Mix_LoadWAV_RW(mus_file, 1);
    if (!snd_oo) {
        fprintf_s(stdout, "%s", Mix_GetError());
    }


    setVolume(80);
}

void MusicPlayer::playLooped() {
    Mix_PlayMusic(mus_bg, -1);
}

void MusicPlayer::playSfx(int sfx_id) {
    switch (sfx_id) {
    case ID::SND_GUNSH:
        Mix_PlayChannel(-1, snd_gunsh, 0);
        break;
    case ID::SND_OO:
        Mix_PlayChannel(-1, snd_oo, 0);
        break;
    default:
        break;
    }
}


void MusicPlayer::changeVolume(int difference) {
    volume += difference;
    if (volume < 0) { volume = 0; }
    else if (volume > 128) { volume = 128; }
    Mix_Volume(-1, volume);
    Mix_VolumeMusic(volume);
    fprintf_s(stdout, "new volume: %d\n", volume);
}

void MusicPlayer::setVolume(int value) {
    volume = value;
    if (volume < 0) { volume = 0; }
    else if (volume > 128) { volume = 128; }
    Mix_Volume(-1, volume);
    Mix_VolumeMusic(volume);
    fprintf_s(stdout, "new volume: %d\n", volume);
}