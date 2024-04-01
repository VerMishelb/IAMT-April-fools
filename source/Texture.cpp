#include "../include/Texture.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "../include/CI72Locker.h"

#include "../include/TextureAtlas.h"

SDL_Texture* Texture::atlas_texture = nullptr;
SDL_Surface* Texture::atlas_surface = nullptr;
SDL_Rect Texture::texture_rect[] = {
        {0, 0, 256, 256},
        {0,0,32,32},
        {32,0,48,41},
        {80,0,60,60},
        {140,0,5,24},
        {145,0,18,21},
        {163,0,83,129},
        {133,129,123,120},
        {0,60,36,33},//cash_0
        {36,60,36,33},
        {72,60,36,33},
        {108,60,36,33},
        {0,93,36,33},
        {36,98,36,33},
        {72,98,36,33},
        {108,98,36,33}
};


SDL_Texture* Texture::loadAtlas() {
    SDL_RWops* atlas_file = SDL_RWFromConstMem(data_textureAtlas, data_textureAtlas_size);
    SDL_Surface* atlas_surface = IMG_Load_RW(atlas_file, 1);

    if (!atlas_surface) {
        fprintf_s(stdout, "Could not load images %s.\n", IMG_GetError());
        return nullptr;
    }

    atlas_texture = NULL;

    atlas_texture = SDL_CreateTextureFromSurface(game->getRenderer(), atlas_surface);
    SDL_FreeSurface(atlas_surface);
    return atlas_texture;
}
