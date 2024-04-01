#ifndef Player_h_
#define Player_h_

#include <SDL.h>
#include "../include/BulletSpawner.h"
#include "../include/Collision.h"

class Player {
    SDL_Point position;
    int shootingCooldown;
    HitCircle hitcircle;
    SDL_Point hitcircle_offset;

public:
    Player();
    ~Player();

    bool isDead{ false };
    bool canShoot{ true };

    void render();
    void update();
};

#endif