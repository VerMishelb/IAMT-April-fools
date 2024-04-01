#include "../include/ChickenHandler.h"
#include "../include/CI72Locker.h"
#include "../include/Texture.h"
#include <stdlib.h>

ChickenHandler::ChickenHandler() : shoot_cooldown(200), next_chicken(0), position_offset(0), active_chickens(0), level_finished(false) {
    for (int i = 0; i < MAX_CHICKENS; ++i) {
        chickens[i].health = 3;
        chickens[i].active = false;
        chickens[i].position = { 0,0 };
        chickens[i].hitcircle = {0,0,15};
    }
}

ChickenHandler::~ChickenHandler() {}


void ChickenHandler::spawn(SDL_FPoint position, int health) {
    chickens[next_chicken].active = true;
    chickens[next_chicken].health = health;
    chickens[next_chicken].position = position;
    chickens[next_chicken].draw_position = { chickens[next_chicken].position.x + position_offset, chickens[next_chicken].position.y };
    chickens[next_chicken].hitcircle.x = chickens[next_chicken].draw_position.x + hitcircle_offset.x;
    chickens[next_chicken].hitcircle.y = chickens[next_chicken].draw_position.y + hitcircle_offset.y;
    ++active_chickens;
    ++next_chicken;
    if (next_chicken > MAX_CHICKENS - 1) { next_chicken = 0; }
}

void ChickenHandler::update() {
    for (int i = 0; i < MAX_CHICKENS; ++i) {
        if (chickens[i].active) {
            chickens[i].draw_position = { chickens[i].position.x + position_offset, chickens[i].position.y };
            chickens[i].hitcircle.x = chickens[i].draw_position.x + hitcircle_offset.x;
            chickens[i].hitcircle.y = chickens[i].draw_position.y + hitcircle_offset.y;

            for (int ib = 0; ib < MAX_BULLETS; ++ib) {
                if (game->bulletSpawner.bullets[ib].active &&
                    !(game->bulletSpawner.bullets[ib].isEgg) &&
                    !(game->bulletSpawner.bullets[ib].isCash) &&
                    collides(chickens[i].hitcircle, game->bulletSpawner.bullets[ib].hitcircle))
                {
                    //fprintf_s(stdout, "Bullet hit the chikcne\n");
                    game->bulletSpawner.bullets[ib].active = false;
                    if (--chickens[i].health <= 0 && chickens[i].active) {
                        game->bulletSpawner.spawnCash(
                            { float(chickens[i].hitcircle.x), float(chickens[i].hitcircle.y) },
                            { 0, 3 });
                        chickens[i].active = false;
                        --active_chickens;
                        fprintf_s(stdout, "active_chickens %d\n", active_chickens);
                        if (active_chickens == 0) {
                            level_finished = true;
                        }
                        game->music_player.playSfx(MusicPlayer::ID::SND_CHICKEN_DIE);
                    }

                    game->music_player.playSfx(MusicPlayer::ID::SND_CHICKEN_HIT);
                }
            }
        }
    }

    if (shoot_cooldown <= 0) {
        for (int q = 0; q < 10; ++q) {
            shoot_cooldown = active_chickens+1;
            int shooting_chicken = rand() % MAX_CHICKENS;
            while (!(chickens[shooting_chicken].active) && active_chickens > 0) {
                ++shooting_chicken;
                if (shooting_chicken > MAX_CHICKENS - 1) {
                    shooting_chicken = 0;
                }
            }
        game->bulletSpawner.spawnEgg(chickens[shooting_chicken].draw_position, { 0, 8 });
        }
        game->music_player.playSfx(MusicPlayer::ID::SND_OO);
    }

    --shoot_cooldown;
    position_offset += 2;
    if (position_offset > 25) {
        position_offset = 0;
    }
}

void ChickenHandler::render() {
    for (int i = 0; i < MAX_CHICKENS; ++i) {
        if (chickens[i].active) {
            SDL_Rect spriteRect = {
               chickens[i].draw_position.x, chickens[i].draw_position.y,
               Texture::texture_rect[Texture::ID::SPR_CHICKEN].w,
               Texture::texture_rect[Texture::ID::SPR_CHICKEN].h
            };

            //fprintf_s(stdout, "bullet: %d, %d, %d, %d\n", spriteRect.x, spriteRect.y, spriteRect.w, spriteRect.h);
            SDL_RenderCopy(game->getRenderer(), Texture::atlas_texture,
                &Texture::texture_rect[Texture::ID::SPR_CHICKEN],
                &spriteRect);
        }
    }
}

void ChickenHandler::cleanup() {
    for (int i = 0; i < MAX_CHICKENS; ++i) {
        chickens[i].active = false;
    }
    level_finished = false;
    next_chicken = 0;
    active_chickens = 0;
}