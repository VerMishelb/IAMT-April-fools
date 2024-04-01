#include "../include/MusicPlayer.h"
#include "SDL_mixer.h"
#include <stdio.h>
#include "../include/mus_music3.h"
#include "../include/snd_gunsh.h"
#include "../include/snd_oo.h"
#include "../include/snd_chicken_hit.h"
#include "../include/snd_chicken_die.h"
#include "../include/snd_dc_hit.h"
#include "../include/snd_dc_teleport.h"
#include "../include/snd_dc_death.h"
#include "../include/snd_cash.h"

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

    mus_file = SDL_RWFromConstMem(data_snd_chicken_hit, data_snd_chicken_hit_size);
    snd_chicken_hit = Mix_LoadWAV_RW(mus_file, 1);
    if (!snd_chicken_hit) {
        fprintf_s(stdout, "%s", Mix_GetError());
    }

    mus_file = SDL_RWFromConstMem(data_snd_chicken_die, data_snd_chicken_die_size);
    snd_chicken_die = Mix_LoadWAV_RW(mus_file, 1);
    if (!snd_chicken_die) {
        fprintf_s(stdout, "%s", Mix_GetError());
    }

    mus_file = SDL_RWFromConstMem(data_snd_dc_hit, data_snd_dc_hit_size);
    snd_dc_hit = Mix_LoadWAV_RW(mus_file, 1);
    if (!snd_dc_hit) {
        fprintf_s(stdout, "%s", Mix_GetError());
    }

    mus_file = SDL_RWFromConstMem(data_snd_dc_teleport, data_snd_dc_teleport_size);
    snd_dc_teleport = Mix_LoadWAV_RW(mus_file, 1);
    if (!snd_dc_teleport) {
        fprintf_s(stdout, "%s", Mix_GetError());
    }

    mus_file = SDL_RWFromConstMem(data_snd_dc_death, data_snd_dc_death_size);
    snd_dc_death = Mix_LoadWAV_RW(mus_file, 1);
    if (!snd_dc_death) {
        fprintf_s(stdout, "%s", Mix_GetError());
    }

    mus_file = SDL_RWFromConstMem(data_snd_cash, data_snd_cash_size);
    snd_cash = Mix_LoadWAV_RW(mus_file, 1);
    if (!snd_cash) {
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
    case ID::SND_CHICKEN_HIT:
        Mix_PlayChannel(-1, snd_chicken_hit, 0);
        break;
    case ID::SND_CHICKEN_DIE:
        Mix_PlayChannel(-1, snd_chicken_die, 0);
        break;
    case ID::SND_DC_HIT:
        Mix_PlayChannel(-1, snd_dc_hit, 0);
        break;
    case ID::SND_DC_TELEPORT:
        Mix_PlayChannel(-1, snd_dc_teleport, 0);
        break;
    case ID::SND_DC_DEATH:
        Mix_PlayChannel(-1, snd_dc_death, 0);
        break;
    case ID::SND_CASH:
        Mix_PlayChannel(-1, snd_cash, 0);
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

void MusicPlayer::shutUp() {
    Mix_HaltMusic();
}