#ifndef Player_h_
#define Player_h_

#include <SDL.h>
#include "../include/BulletSpawner.h"

class Player {
    SDL_Point position;
    int shootingCooldown;

public:
    Player();
    ~Player();

    void render();
    void update();
};

#endif