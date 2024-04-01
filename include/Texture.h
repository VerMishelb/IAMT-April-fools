#ifndef Texture_h_
#define Texture_h_

#include <SDL.h>


namespace Texture {
    SDL_Texture* loadAtlas();

    extern SDL_Texture* atlas_texture;
    extern SDL_Surface* atlas_surface;
    extern SDL_Rect texture_rect[];

    enum ID {
        NONE,
        SPR_BACKGROUND,
        SPR_SHIP,
        SPR_CHICKEN,
        SPR_ION,
        SPR_EGG,
        SPR_DICK_CLARK,
        SPR_BIG_CHICKEN,
        SPR_CASH_0,
        SPR_CASH_1,
        SPR_CASH_2,
        SPR_CASH_3,
        SPR_CASH_4,
        SPR_CASH_5,
        SPR_CASH_6,
        SPR_CASH_7,
        TOTAL
    };
};


#endif