#ifndef BulletSpawner_h_
#define BulletSpawner_h_

#include <SDL.h>

#define MAX_BULLETS 512

struct Bullet {
    SDL_FPoint velocity;
    SDL_FPoint position;
    //float rotation;
    bool isEgg;
    bool isCash;
    bool active;
};

class BulletSpawner {
private:
    Bullet bullets[MAX_BULLETS] = {};
    bool outOfBounds(Bullet* bullet);
    int next_bullet;
    int animation_frame;
    int tick;

public:
    BulletSpawner();
    ~BulletSpawner();

    void spawnIon(SDL_FPoint pos);
    void spawnEgg(SDL_FPoint pos, SDL_FPoint velocity);
    void spawnCash(SDL_FPoint pos, SDL_FPoint velocity);

    void render();
    void update();
};

#endif