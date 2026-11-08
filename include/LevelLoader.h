#ifndef LevelLoader_h_
#define LevelLoader_h_

#include "../include/Player.h"

class LevelLoader {
private:
    int state;

    void loadState(int state);
    void unloadState();

    Player player;

public:
    LevelLoader();
    ~LevelLoader();

    enum State {
        INTRO,
        TITLE,
        LVL_CHICKENS,
        LVL_DICK_CLARK,
        LVL_USELESS,
        GAME_OVER,
        AMOUNT
    };
    
    #define changeState(state) changeState_(state, #state)
    void changeState_(int state, const char* state_name = nullptr);

    void update();
    void render();
    SDL_FPoint getPlayerPosition();
};

#endif