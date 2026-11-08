#ifndef MusicPlayer_h_
#define MusicPlayer_h_

#include <string>
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

class MusicPlayer {
public:
    static const size_t tracks_total = 16;
    enum ID {
        NONE,
        MUS_MUSIC3,
        SND_GUNSH,
        SND_OO,
        SND_CHICKEN_HIT,
        SND_CHICKEN_DIE,
        SND_DC_HIT,
        SND_DC_TELEPORT,
        SND_DC_DEATH,
        SND_CASH,
        LAST_ID = SND_CASH
    };
    
    MusicPlayer();
    ~MusicPlayer();

    bool init();
    void playLooped();
    bool playSfx(int sfx_id);
    void shutUp();

    void addVolume(int difference);
    void setVolume(int value);

private:
    MIX_Audio* loadAudio(std::string filename, bool predecode = false);
    MIX_Audio* loadAudio(const void* embedded_file_name, size_t embedded_file_size, const char* var_name, bool predecode = false);
    MIX_Audio* mus_bg;
    // A quick reminder that all snd is vorbis ogg, and mus is a midi.

    MIX_Audio* snd_gunsh;
    MIX_Audio* snd_oo;
    MIX_Audio* snd_chicken_hit;
    MIX_Audio* snd_chicken_die;
    MIX_Audio* snd_dc_hit;
    MIX_Audio* snd_dc_teleport;
    MIX_Audio* snd_dc_death;
    MIX_Audio* snd_cash;


    int volume = 128;
    MIX_Mixer* mixer = nullptr;
    MIX_Track* tracks_sfx[tracks_total] = {nullptr};
    MIX_Track* track_mus = nullptr;

};

#endif