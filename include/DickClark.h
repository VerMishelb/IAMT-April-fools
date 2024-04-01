#ifndef DickClark_h_
#define DickClark_h_

#include <SDL.h>
#include "../include/Collision.h"

#define MAX_DICKCLARKS 3

struct DickClark_enemy {
    SDL_FPoint position;
    SDL_FPoint draw_position;
    SDL_FPoint next_position;
    HitCircle hitcircle;
    int teleport_timeout;
    int health;
    bool active;
};

class DickClark {
private:
    int next_dickclark;
    int position_offset;
    SDL_Point hitcircle_offset;
    int active_dickclarks;

public:
    int shoot_cooldown;
    bool level_finished;
    DickClark_enemy dickclarks[MAX_DICKCLARKS];

    DickClark();
    ~DickClark();

    void update();
    void render();

    void cleanup();
};

#endif