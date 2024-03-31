#include "../include/BulletSpawner.h"
#include "../include/CI72Locker.h"
#include "../include/Texture.h"
#include <stdio.h>

BulletSpawner::BulletSpawner() : next_bullet(0) {
    for (int i = 0; i < 256; ++i) {
        bullets[i].position = { 0, 0 };
        bullets[i].velocity = { 0,0 };
        bullets[i].isEgg = false;
        bullets[i].active = false;
    }
}

BulletSpawner::~BulletSpawner() {}

void BulletSpawner::spawnIon(SDL_FPoint pos) {
    bullets[next_bullet].active = true;
    bullets[next_bullet].position = pos;
    bullets[next_bullet].velocity = { 0, -10 };
    bullets[next_bullet].isEgg = false;
    ++next_bullet;
    if (next_bullet > 255) { next_bullet = 0; }
}

void BulletSpawner::spawnEgg(SDL_FPoint pos, SDL_FPoint velocity) {
    bullets[next_bullet].active = true;
    bullets[next_bullet].position = pos;
    bullets[next_bullet].velocity = velocity;
    bullets[next_bullet].isEgg = true;
    ++next_bullet;
    if (next_bullet > 255) { next_bullet = 0; }
}

void BulletSpawner::render() {
    for (int i = 0; i < 256; ++i) {
        if (bullets[i].active) {
            SDL_Rect spriteRect = {
               bullets[i].position.x, bullets[i].position.y,
               (Texture::texture_rect[((bullets[i].isEgg) ? Texture::ID::SPR_EGG : Texture::ID::SPR_ION)].w),
               (Texture::texture_rect[((bullets[i].isEgg) ? Texture::ID::SPR_EGG : Texture::ID::SPR_ION)].h)
            };

            fprintf_s(stdout, "bullet: %d, %d, %d, %d\n", spriteRect.x, spriteRect.y, spriteRect.w, spriteRect.h);
            SDL_RenderCopy(game->getRenderer(), Texture::atlas_texture,
                ((bullets[i].isEgg) ? &Texture::texture_rect[Texture::ID::SPR_EGG] : &Texture::texture_rect[Texture::ID::SPR_ION]),
                &spriteRect);
        }
    }
}

bool BulletSpawner::outOfBounds(Bullet* bullet) {
    return (!(bullet->active) ||
        bullet->position.x > game->getWindowDimensions().w + 50 ||
        bullet->position.x < -50 ||
        bullet->position.y > game->getWindowDimensions().h + 50 ||
        bullet->position.y < -50);
}

void BulletSpawner::update() {
    for (int i = 0; i < 256; ++i) {
        if (outOfBounds(&bullets[i])) {
            bullets[i].active = false;
        }
        bullets[i].position.x += bullets[i].velocity.x;
        bullets[i].position.y += bullets[i].velocity.y;
    }
}