#include "../include/Player.h"
#include "../include/CI72Locker.h"
#include "../include/Texture.h"
#include <stdio.h>

Player::Player() : position({ 4,0 }), shootingCooldown(0), hitcircle({ 0, 0, 12 }), hitcircle_offset({24, 17}) {}
Player::~Player() {}

void Player::render() {
    if (!isDead) {
        SDL_Rect spriteRect = {
                   position.x, position.y,
                   Texture::texture_rect[Texture::ID::SPR_SHIP].w,
                   Texture::texture_rect[Texture::ID::SPR_SHIP].h
        };

        //fprintf_s(stdout, "dest: %d, %d, %d, %d\n", spriteRect.x, spriteRect.y, spriteRect.w, spriteRect.h);
        SDL_RenderCopy(game->getRenderer(), Texture::atlas_texture, &Texture::texture_rect[Texture::ID::SPR_SHIP],
            &spriteRect);
    }
}

void Player::update() {
    //fprintf_s(stdout, "I updated\n");
    if (!isDead) {
        this->position.x = game->input_state.mousePosition.x - game->getWindowDimensions().x - 24;
        this->position.y = game->input_state.mousePosition.y - game->getWindowDimensions().y - 17;
        hitcircle.x = position.x + hitcircle_offset.x;
        hitcircle.y = position.y + hitcircle_offset.y;

        if (game->input_state.mouseHeld && canShoot && !shootingCooldown) {
            SDL_FPoint bullet_position = { position.x, position.y };
            game->bulletSpawner.spawnIon(bullet_position);
            game->music_player.playSfx(MusicPlayer::ID::SND_GUNSH);
            shootingCooldown = 8;
        }

        if (shootingCooldown) {
            --shootingCooldown;
        };

        // May God forgive me for such inefficient collision checks but I literally have no time
        for (int i = 0; i < MAX_CHICKENS; ++i) {
            if (game->chicken_handler.chickens[i].active &&
                collides(this->hitcircle, game->chicken_handler.chickens[i].hitcircle))
            {
                fprintf_s(stdout, "collidded with chickens[%d]\n", i);
                isDead = true;
            }
        }
        for (int i = 0; i < MAX_BULLETS; ++i) {
            if (game->bulletSpawner.bullets[i].active &&
                collides(this->hitcircle, game->bulletSpawner.bullets[i].hitcircle))
            {
                if (game->bulletSpawner.bullets[i].isEgg) {
                    fprintf_s(stdout, "collidded with egg[%d]\n", i);
                    game->bulletSpawner.eggCollided(i);
                    isDead = true;
                }
                else if (game->bulletSpawner.bullets[i].isCash) {
                    fprintf_s(stdout, "collided with cash[%d]\n", i);
                    game->bulletSpawner.cashCollided(i);
                }
            }
        }
    }
}