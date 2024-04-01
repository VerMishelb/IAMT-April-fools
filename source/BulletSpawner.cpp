 #include "../include/BulletSpawner.h"
#include "../include/CI72Locker.h"
#include "../include/Texture.h"
#include <stdio.h>

BulletSpawner::BulletSpawner() : next_bullet(0), animation_frame(0) {
    for (int i = 0; i < MAX_BULLETS; ++i) {
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
    bullets[next_bullet].isCash = false;
    bullets[next_bullet].hitcircle = hitcircles[ION];
    bullets[next_bullet].hitcircle_offset = { hitcircles[ION].x, hitcircles[ION].y };
    ++next_bullet;
    if (next_bullet > MAX_BULLETS - 1) { next_bullet = 0; }
}

void BulletSpawner::spawnEgg(SDL_FPoint pos, SDL_FPoint velocity) {
    //fprintf_s(stdout, "Called spawnEgg with position %f %f\n", pos.x, pos.y);
    bullets[next_bullet].active = true;
    bullets[next_bullet].position = pos;
    bullets[next_bullet].velocity = velocity;
    bullets[next_bullet].isEgg = true;
    bullets[next_bullet].isCash = false;
    bullets[next_bullet].hitcircle = hitcircles[EGG];
    bullets[next_bullet].hitcircle_offset = { hitcircles[EGG].x, hitcircles[EGG].y };
    ++next_bullet;
    if (next_bullet > MAX_BULLETS - 1) { next_bullet = 0; }
}

void BulletSpawner::spawnCash(SDL_FPoint pos, SDL_FPoint velocity) {
    //fprintf_s(stdout, "Called spawnEgg with position %f %f\n", pos.x, pos.y);
    bullets[next_bullet].active = true;
    bullets[next_bullet].position = pos;
    bullets[next_bullet].velocity = velocity;
    bullets[next_bullet].isEgg = false;
    bullets[next_bullet].isCash = true;
    bullets[next_bullet].hitcircle = hitcircles[CASH];
    bullets[next_bullet].hitcircle_offset = {hitcircles[CASH].x, hitcircles[CASH].y};
    ++next_bullet;
    if (next_bullet > MAX_BULLETS - 1) { next_bullet = 0; }
}

void BulletSpawner::render() {
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (bullets[i].active) {
            if (!bullets[i].isCash) {
                SDL_Rect spriteRect = {
                   bullets[i].position.x, bullets[i].position.y,
                   (Texture::texture_rect[((bullets[i].isEgg) ? Texture::ID::SPR_EGG : Texture::ID::SPR_ION)].w),
                   (Texture::texture_rect[((bullets[i].isEgg) ? Texture::ID::SPR_EGG : Texture::ID::SPR_ION)].h)
                };

                //fprintf_s(stdout, "bullet: %d, %d, %d, %d\n", spriteRect.x, spriteRect.y, spriteRect.w, spriteRect.h);
                SDL_RenderCopy(game->getRenderer(), Texture::atlas_texture,
                    ((bullets[i].isEgg) ? &Texture::texture_rect[Texture::ID::SPR_EGG] : &Texture::texture_rect[Texture::ID::SPR_ION]),
                    &spriteRect);
            }
            else {
                SDL_Rect spriteRect = {
                   bullets[i].position.x, bullets[i].position.y,
                   Texture::texture_rect[Texture::ID::SPR_CASH_0 + animation_frame].w,
                   Texture::texture_rect[Texture::ID::SPR_CASH_0 + animation_frame].h
                };

                //fprintf_s(stdout, "bullet: %d, %d, %d, %d\n", spriteRect.x, spriteRect.y, spriteRect.w, spriteRect.h);
                SDL_RenderCopy(game->getRenderer(), Texture::atlas_texture,
                    &Texture::texture_rect[Texture::ID::SPR_CASH_0 + animation_frame],
                    &spriteRect);
            }
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
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (outOfBounds(&bullets[i])) {
            bullets[i].active = false;
        }
        bullets[i].position.x += bullets[i].velocity.x;
        bullets[i].position.y += bullets[i].velocity.y;
        bullets[i].hitcircle.x = bullets[i].position.x + bullets[i].hitcircle_offset.x;
        bullets[i].hitcircle.y = bullets[i].position.y + bullets[i].hitcircle_offset.y;
    }
    ++tick;
    if (tick > 1) {
        tick = 0;
        ++animation_frame;
        if (animation_frame >= 8) { animation_frame = 0; };
    }
}

void BulletSpawner::cashCollided(int id) {
    bullets[id].active = false;
    // TODO: Play the sound
    // Increase score? Who would be reading it?
}

void BulletSpawner::eggCollided(int id) {
    bullets[id].active = false;
    // TODO: Play the sound?
}

void BulletSpawner::cleanup() {
    for (int i = 0; i < MAX_BULLETS; ++i) {
        bullets[i].active = false;
    }
}