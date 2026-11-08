#line 2 "MusicPlayer.cpp"
#include "../include/MusicPlayer.h"
#include <SDL3_mixer/SDL_mixer.h>
#include <stdio.h>
#include "minidebug.h"

#ifdef CI72_EMBEDDED_ASSETS
#include "assets/all_audio.h"
#endif

#define VME_AUDIO_LOAD_ARG(x) x, x##_size, #x

MusicPlayer::MusicPlayer() :
    mus_bg(nullptr),
    snd_gunsh(nullptr),
    snd_oo(nullptr)
{
    
}

MusicPlayer::~MusicPlayer() {
    MIX_Quit();
}

bool MusicPlayer::init() {
    if (!MIX_Init()){
        miniprintf("MIX_Init failed. SDL_error: %s", SDL_GetError());
    }
    
    // SDL_AudioSpec audiospec{SDL_AUDIO_S16, 2, 44100};
    if ((mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr)) == nullptr) {
        miniprintf("Couldn't create the mixer. SDL_error: %s", SDL_GetError());
        return false;
    }

    track_mus = MIX_CreateTrack(mixer);
    if (!track_mus) {
        miniprintf("Couldn't create track_mus: %s", SDL_GetError());
        return false;
    }
    for (size_t i = 0; i < tracks_total; i++) {
        tracks_sfx[i] = MIX_CreateTrack(mixer);
        if (!tracks_sfx[i]) {
            miniprintf("Couldn't create tracks_sfx[%d]. SDL_error: %s", i, SDL_GetError());
            return false;
        }
    }

#ifdef CI72_EMBEDDED_ASSETS
    mus_bg =            loadAudio(VME_AUDIO_LOAD_ARG(mus_music3_mid));
    snd_gunsh =         loadAudio(VME_AUDIO_LOAD_ARG(snd_gunsh_wav), true);
    snd_oo =            loadAudio(VME_AUDIO_LOAD_ARG(snd_oo_wav), true);
    snd_chicken_hit =   loadAudio(VME_AUDIO_LOAD_ARG(snd_chicken_hit_wav), true);
    snd_chicken_die =   loadAudio(VME_AUDIO_LOAD_ARG(snd_chicken_die_ogg), true);
    snd_dc_hit =        loadAudio(VME_AUDIO_LOAD_ARG(snd_dc_hit_wav), true);
    snd_dc_teleport =   loadAudio(VME_AUDIO_LOAD_ARG(snd_dc_teleport_wav), true);
    snd_dc_death =      loadAudio(VME_AUDIO_LOAD_ARG(snd_dc_death_wav), true);
    snd_cash =          loadAudio(VME_AUDIO_LOAD_ARG(snd_cash_ogg), true);
#else
    mus_bg =            loadAudio("assets/mus_music3.mid"); // the evil file brought me more pain than the entirety of SDL3
    snd_gunsh =         loadAudio("assets/snd_gunsh.wav", true);
    snd_oo =            loadAudio("assets/snd_oo.wav", true); // this one as well, for some reason.
    snd_chicken_hit =   loadAudio("assets/snd_chicken_hit.wav", true);
    snd_chicken_die =   loadAudio("assets/snd_chicken_die.ogg", true);
    snd_dc_hit =        loadAudio("assets/snd_dc_hit.wav", true);
    snd_dc_teleport =   loadAudio("assets/snd_dc_teleport.wav", true);
    snd_dc_death =      loadAudio("assets/snd_dc_death.wav", true);
    snd_cash =          loadAudio("assets/snd_cash.ogg", true);
#endif


    setVolume(80);
    return true;
}

void MusicPlayer::playLooped() {
    if (!MIX_SetTrackAudio(track_mus, mus_bg)) {
        miniprintf("SetTrackAudio failed: %s", SDL_GetError());
    }
    SDL_PropertiesID options = SDL_CreateProperties();
    if (!options) {
        SDL_Log("Couldn't create play options: %s", SDL_GetError());
        return;
    }

    if (!SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, -1)) {
        miniprintf("Could not set property: %s", SDL_GetError());
    }
    if (!MIX_PlayTrack(track_mus, options)) {
        miniprintf("PlayTrack failed: %s", SDL_GetError());
    }
    SDL_DestroyProperties(options);
}


int current_track = 0;
bool MusicPlayer::playSfx(int sfx_id) {
    switch (sfx_id) {
    case ID::SND_GUNSH:
        MIX_SetTrackAudio(tracks_sfx[current_track], snd_gunsh);
        break;
    case ID::SND_OO:
        MIX_SetTrackAudio(tracks_sfx[current_track], snd_oo);
        break;
    case ID::SND_CHICKEN_HIT:
        MIX_SetTrackAudio(tracks_sfx[current_track], snd_chicken_hit);
        break;
    case ID::SND_CHICKEN_DIE:
        MIX_SetTrackAudio(tracks_sfx[current_track], snd_chicken_die);
        break;
    case ID::SND_DC_HIT:
        MIX_SetTrackAudio(tracks_sfx[current_track], snd_dc_hit);
        break;
    case ID::SND_DC_TELEPORT:
        MIX_SetTrackAudio(tracks_sfx[current_track], snd_dc_teleport);
        break;
    case ID::SND_DC_DEATH:
        MIX_SetTrackAudio(tracks_sfx[current_track], snd_dc_death);
        break;
    case ID::SND_CASH:
        MIX_SetTrackAudio(tracks_sfx[current_track], snd_cash);
        break;
    default:
        return false;
    }

    MIX_PlayTrack(tracks_sfx[current_track++], 0);
    if (current_track >= tracks_total) { current_track = 0; }
    return true;
}


void MusicPlayer::addVolume(int difference) {
    volume += difference;
    if (volume < 0) { volume = 0; }
    else if (volume > 128) { volume = 128; }
    MIX_SetMixerGain(mixer, volume/128.f);
    miniprintf("new volume: %d", volume);
}

void MusicPlayer::setVolume(int value) {
    volume = value;
    if (volume < 0) { volume = 0; }
    else if (volume > 128) { volume = 128; }
    MIX_SetMixerGain(mixer, volume/128.f);
    miniprintf("new volume: %d", volume);
}

void MusicPlayer::shutUp() {
    MIX_StopTrack(track_mus, 0); // Is it supposed to affect music only?
}


MIX_Audio* MusicPlayer::loadAudio(std::string filename, bool predecode) {
    auto r = MIX_LoadAudio(mixer, filename.c_str(), predecode);
    if (!r) {
        miniprintf("Couldn't load audio \'%s\': %s", filename.c_str(), SDL_GetError());
    }
    return r;
}

MIX_Audio* MusicPlayer::loadAudio(const void* embedded_file_name, size_t embedded_file_size, const char* var_name, bool predecode) {
    SDL_IOStream* mus_file = SDL_IOFromConstMem(embedded_file_name, embedded_file_size);
    if (!mus_file) {
        miniprintf("Couldn't load embedded IO stream \'%s\': %s", var_name, SDL_GetError());
        return nullptr;
    }
    auto r = MIX_LoadAudio_IO(mixer, mus_file, predecode, true);
    if (!r) {
        miniprintf("Couldn't load audio \'%s\': %s", var_name, SDL_GetError());
    }
    return r;
}