#ifndef Player_h_
#define Player_h_

#include <SDL.h>
#include "../include/BulletSpawner.h"
#include "../include/Collision.h"

class Player {
    SDL_Point position;
    HitCircle hitcircle;
    SDL_Point hitcircle_offset;
    int shootingCooldown;

public:
    Player();
    ~Player();

    bool isDead{ false };
    bool canShoot{ true };

    void render();
    void update();
    SDL_FPoint getPosition();
};

#endif