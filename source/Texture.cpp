#include "../include/Texture.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include "../include/CI72Locker.h"
#include "minidebug.h"

#include "assets/TextureAtlas_png.h"

SDL_Texture* Texture::atlas_texture = nullptr;
SDL_Surface* Texture::atlas_surface = nullptr;
SDL_FRect Texture::texture_rect[] = {
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
#ifdef CI72_EMBEDDED_ASSETS
    SDL_IOStream* atlas_file = SDL_IOFromConstMem(TextureAtlas_png, TextureAtlas_png_size);
    SDL_Surface* atlas_surface = IMG_Load_IO(atlas_file, true);
#else
    SDL_Surface* atlas_surface = IMG_Load("assets/img_textureatlas.png");
#endif

    if (!atlas_surface) {
        miniprintf("Could not load the texture atlas: %s.", SDL_GetError());
        return nullptr;
    }

    atlas_texture = SDL_CreateTextureFromSurface(game->getRenderer(), atlas_surface);
    if (!atlas_texture) {
        miniprintf("Could not create texture from surface: %s.", SDL_GetError());
    }
    SDL_DestroySurface(atlas_surface);
    return atlas_texture;
}
