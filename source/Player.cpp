#include "../include/Player.h"
#include "../include/CI72Locker.h"
#include "../include/Texture.h"
#include <stdio.h>

Player::Player() : position({ 4,0 }), shootingCooldown(0) {}
Player::~Player() {}

void Player::render() {
    SDL_Rect spriteRect = {
               position.x, position.y,
               Texture::texture_rect[Texture::ID::SPR_SHIP].w,
               Texture::texture_rect[Texture::ID::SPR_SHIP].h
    };

    //fprintf_s(stdout, "dest: %d, %d, %d, %d\n", spriteRect.x, spriteRect.y, spriteRect.w, spriteRect.h);
    SDL_RenderCopy(game->getRenderer(), Texture::atlas_texture, &Texture::texture_rect[Texture::ID::SPR_SHIP],
        &spriteRect);
}

void Player::update() {
    //fprintf_s(stdout, "I updated\n");
    this->position.x = game->input_state.mousePosition.x - game->getWindowDimensions().x - 24;
    this->position.y = game->input_state.mousePosition.y - game->getWindowDimensions().y - 17;

    if (game->input_state.mouseHeld/* && !shootingCooldown*/) {
        SDL_FPoint bullet_position = { position.x, position.y };
        game->bulletSpawner.spawnIon(bullet_position);
        shootingCooldown = 500;
    }

    if (shootingCooldown) {
        --shootingCooldown;
    };
}