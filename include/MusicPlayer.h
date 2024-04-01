#ifndef MusicPlayer_h_
#define MusicPlayer_h_

#include <SDL.h>
#include <SDL_mixer.h>

class MusicPlayer {
private:
    int volume = 128;

public:
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
        AMOUNT
    };
    Mix_Music* mus_bg;

    Mix_Chunk* snd_gunsh;
    Mix_Chunk* snd_oo;
    Mix_Chunk* snd_chicken_hit;
    Mix_Chunk* snd_chicken_die;
    Mix_Chunk* snd_dc_hit;
    Mix_Chunk* snd_dc_teleport;
    Mix_Chunk* snd_dc_death;
    Mix_Chunk* snd_cash;

    MusicPlayer();
    ~MusicPlayer();

    void init();
    void playLooped();
    void playSfx(int sfx_id);
    void shutUp();

    void changeVolume(int difference);
    void setVolume(int value);
};

#endif