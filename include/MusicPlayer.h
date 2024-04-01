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
        SND_EXPLOSION,
        SND_WIN,
        AMOUNT
    };
    Mix_Music* mus_bg;
    Mix_Chunk* snd_gunsh;
    Mix_Chunk* snd_oo;

    MusicPlayer();
    ~MusicPlayer();

    void init();
    void playLooped();
    void playSfx(int sfx_id);

    void changeVolume(int difference);
    void setVolume(int value);
};

#endif