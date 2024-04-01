#ifndef BulletSpawner_h_
#define BulletSpawner_h_

#include <SDL.h>
#include "../include/Collision.h"

#define MAX_BULLETS 1024

struct Bullet {
    SDL_FPoint velocity;
    SDL_FPoint position;
    SDL_Point hitcircle_offset;
    HitCircle hitcircle;
    //float rotation;
    bool isEgg;
    bool isCash;
    bool active;

};

class BulletSpawner {
private:
    bool outOfBounds(Bullet* bullet);
    int next_bullet;
    int animation_frame;
    int tick;

public:
    Bullet bullets[MAX_BULLETS] = {};

    BulletSpawner();
    ~BulletSpawner();

    enum Type {
        ION,
        EGG,
        CASH,
        AMOUNT
    };

    const HitCircle hitcircles[Type::AMOUNT] = {
        {1, 3, 10},
        {5, 5, 9},
        {15, 12, 22},
    };


    void spawnIon(SDL_FPoint pos);
    void spawnEgg(SDL_FPoint pos, SDL_FPoint velocity);
    void spawnCash(SDL_FPoint pos, SDL_FPoint velocity);
    void cashCollided(int id);
    void eggCollided(int id);

    void render();
    void update();

    void cleanup();
};

#endif