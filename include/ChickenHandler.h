#ifndef ChickenHandler_h_
#define ChickenHandler_h_

#include <SDL.h>
#include "../include/Collision.h"

#define MAX_CHICKENS 128

struct Chicken {
    SDL_FPoint position;
    SDL_FPoint draw_position;
    HitCircle hitcircle;
    int health;
    bool active;

};

class ChickenHandler {
private:
    int next_chicken;
    int position_offset;
    SDL_Point hitcircle_offset = { 27,31 };
    int active_chickens;

public:
    int shoot_cooldown;
    bool level_finished;
    Chicken chickens[MAX_CHICKENS];
    ChickenHandler();
    ~ChickenHandler();

    void spawn(SDL_FPoint position, int health);

    void update();
    void render();

    void cleanup();
};

#endif