#ifndef BulletSpawner_h_
#define BulletSpawner_h_

#include <SDL.h>

struct Bullet {
    SDL_FPoint velocity;
    SDL_FPoint position;
    //float rotation;
    bool isEgg;
    bool active;
};

class BulletSpawner {
private:
    Bullet bullets[256] = {};
    bool outOfBounds(Bullet* bullet);
    int next_bullet;

public:
    BulletSpawner();
    ~BulletSpawner();

    void spawnIon(SDL_FPoint pos);
    void spawnEgg(SDL_FPoint pos, SDL_FPoint velocity);

    void render();
    void update();
};

#endif