#include "../include/DickClark.h"
#include "../include/CI72Locker.h"
#include "../include/Texture.h"
#include <math.h>

DickClark::DickClark() :
    next_dickclark(0),
    position_offset(0),
    hitcircle_offset({ 42, 60 }),
    active_dickclarks(0),
    level_finished(false),
    shoot_cooldown(200)
{
    for (int i = 0; i < MAX_DICKCLARKS; ++i) {
        dickclarks[i].position = { 0,0 };
        dickclarks[i].draw_position = { 0,0 };
        dickclarks[i].next_position = { 0,0 };
        dickclarks[i].hitcircle = { 42, 60, 45 };
        dickclarks[i].teleport_timeout = 200;
        dickclarks[i].health = 100;
        dickclarks[i].active = false;
    }
}

DickClark::~DickClark() {}


void DickClark::spawn(SDL_FPoint position, int health) {
    dickclarks[next_dickclark].active = true;
    dickclarks[next_dickclark].health = health;
    dickclarks[next_dickclark].position = position;
    dickclarks[next_dickclark].draw_position = { dickclarks[next_dickclark].position.x + position_offset, dickclarks[next_dickclark].position.y };
    dickclarks[next_dickclark].next_position = { float(rand() % game->getWindowDimensions().w), float(rand() % game->getWindowDimensions().h) };
    dickclarks[next_dickclark].teleport_timeout = 200;
    dickclarks[next_dickclark].hitcircle.x = dickclarks[next_dickclark].draw_position.x + hitcircle_offset.x;
    dickclarks[next_dickclark].hitcircle.y = dickclarks[next_dickclark].draw_position.y + hitcircle_offset.y;
    ++active_dickclarks;
    ++next_dickclark;
    if (next_dickclark > MAX_DICKCLARKS - 1) { next_dickclark = 0; }
}



void DickClark::update() {
    for (int i = 0; i < MAX_DICKCLARKS; ++i) {
        if (dickclarks[i].active) {
            dickclarks[i].draw_position = { dickclarks[i].position.x + position_offset, dickclarks[i].position.y };
            dickclarks[i].hitcircle.x = dickclarks[i].draw_position.x + hitcircle_offset.x;
            dickclarks[i].hitcircle.y = dickclarks[i].draw_position.y + hitcircle_offset.y;

            // Collision check with bullets
            for (int ib = 0; ib < MAX_BULLETS; ++ib) {
                if (game->bulletSpawner.bullets[ib].active &&
                    !(game->bulletSpawner.bullets[ib].isEgg) &&
                    !(game->bulletSpawner.bullets[ib].isCash) &&
                    collides(dickclarks[i].hitcircle, game->bulletSpawner.bullets[ib].hitcircle))
                {
                    //fprintf_s(stdout, "Bullet hit the chikcne\n");
                    game->bulletSpawner.bullets[ib].active = false;
                    if (--dickclarks[i].health <= 0 && dickclarks[i].active) {
                        for (int qi = 10 + (rand() % 5); qi > 0; --qi) {
                            SDL_FPoint newvelocity = { rand() - RAND_MAX / 2, rand() - RAND_MAX / 2};
                            normalise(&newvelocity, 2 + rand() % 4);
                            game->bulletSpawner.spawnCash(
                                { float(dickclarks[i].hitcircle.x), float(dickclarks[i].hitcircle.y) },
                                newvelocity);
                        }

                        dickclarks[i].active = false;
                        --active_dickclarks;
                        game->music_player.playSfx(MusicPlayer::ID::SND_DC_DEATH);

                        fprintf_s(stdout, "active_dickclarks %d\n", active_dickclarks);
                        if (active_dickclarks == 0) {
                            level_finished = true;
                        }
                    }

                    game->music_player.playSfx(MusicPlayer::ID::SND_DC_HIT);
                }
            }

            --dickclarks[i].teleport_timeout;
            if (dickclarks[i].teleport_timeout <= 0) {
                dickclarks[i].teleport_timeout = 180;
                dickclarks[i].position = dickclarks[i].next_position;
                dickclarks[i].next_position = { float(rand() % game->getWindowDimensions().w), float(rand() % game->getWindowDimensions().h) };
                game->music_player.playSfx(MusicPlayer::ID::SND_DC_TELEPORT);
            }
        }
    }

    if (shoot_cooldown <= 0) {
        shoot_cooldown = 3;

        for (int shooting_dickclark = 0; shooting_dickclark < MAX_DICKCLARKS; ++shooting_dickclark) {
            if (dickclarks[shooting_dickclark].active == false) { continue; }
            SDL_FPoint newvelocity = {
                game->level_loader.getPlayerPosition().x - dickclarks[shooting_dickclark].draw_position.x,
                game->level_loader.getPlayerPosition().y - dickclarks[shooting_dickclark].draw_position.y };
            normalise(&newvelocity, 3);
            game->bulletSpawner.spawnEgg(dickclarks[shooting_dickclark].draw_position, newvelocity);

        }

        //game->music_player.playSfx(MusicPlayer::ID::SND_OO);
    }

    --shoot_cooldown;
    position_offset += 2;
    if (position_offset > 25) {
        position_offset = 0;
    }
}


void DickClark::render() {
    for (int i = 0; i < MAX_DICKCLARKS; ++i) {
        if (dickclarks[i].active) {
            SDL_Rect spriteRect = {
               dickclarks[i].draw_position.x, dickclarks[i].draw_position.y,
               Texture::texture_rect[Texture::ID::SPR_DICK_CLARK].w,
               Texture::texture_rect[Texture::ID::SPR_DICK_CLARK].h
            };

            //fprintf_s(stdout, "bullet: %d, %d, %d, %d\n", spriteRect.x, spriteRect.y, spriteRect.w, spriteRect.h);
            SDL_RenderCopy(game->getRenderer(), Texture::atlas_texture,
                &Texture::texture_rect[Texture::ID::SPR_DICK_CLARK],
                &spriteRect);

            if (dickclarks[i].teleport_timeout < 40 && dickclarks[i].teleport_timeout % 2) {
                SDL_Rect newposRect = {
                    dickclarks[i].next_position.x, dickclarks[i].next_position.y,
                    Texture::texture_rect[Texture::ID::SPR_DICK_CLARK].w,
                    Texture::texture_rect[Texture::ID::SPR_DICK_CLARK].h
                };
                SDL_RenderCopy(game->getRenderer(), Texture::atlas_texture,
                    &Texture::texture_rect[Texture::ID::SPR_DICK_CLARK],
                    &newposRect);
            }
        }
    }
}

void DickClark::cleanup() {
    for (int i = 0; i < MAX_DICKCLARKS; ++i) {
        dickclarks[i].active = false;
    }
    level_finished = false;
    next_dickclark = 0;
    active_dickclarks = 0;
}

void DickClark::normalise(SDL_FPoint* velocity, float markiplier) {
    float length = sqrt(velocity->x * velocity->x + velocity->y * velocity->y);
    velocity->x = velocity->x / length * markiplier;
    velocity->y = velocity->y / length * markiplier;
}