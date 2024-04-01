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

    void changeState(int state);

    void update();
    void render();

};

#endif