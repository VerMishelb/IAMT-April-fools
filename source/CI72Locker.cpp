#include "../include/CI72Locker.h"
#include <stdio.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "../include/TaskmgrKiller.h"
#include "../include/Texture.h"
#include <time.h>


Game *game = nullptr;


Game::Game() : window_dimensions({0, 0, 800, 600}), renderer(nullptr), window(nullptr), shutdown(false), font(nullptr), font_impact(nullptr) {
    if (!game) {
        game = this;
    }
}

Game::~Game() {}



int Game::init() {
    srand(time(NULL));
    if (!SDL_SetHint(SDL_HINT_ALLOW_ALT_TAB_WHILE_GRABBED, "0")) {
        fprintf_s(stdout, "Couldn't set alt tab hint.\n");
    }
    if (!SDL_SetHint(SDL_HINT_FORCE_RAISEWINDOW, "1")) {
        fprintf_s(stdout, "Couldn't set force raise window hint.\n");
    }
    int init_return_val = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	if (init_return_val != 0) {
		fprintf_s(stdout, "%d SDL Error %d: %s\n", __LINE__, init_return_val, SDL_GetError());
		shutdown = true;
		return -1;
	}
    init_return_val = TTF_Init();
    if (init_return_val) {
        fprintf_s(stdout, "%d SDL_ttf Error %d: %s\n", __LINE__, init_return_val, TTF_GetError());
        return -1;
    }
    SDL_GetDisplayBounds(0, &window_dimensions);

#ifdef DISABLE_FULL_SCREEN_LOCK
    window_dimensions = { 50, 50, 800, 600 };
#endif

    window = SDL_CreateWindow("IAMT 3.5.2 GUI",
        window_dimensions.x, window_dimensions.y, window_dimensions.w, window_dimensions.h,
        SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS
#ifndef DISABLE_FULL_SCREEN_LOCK
        | SDL_WINDOW_ALWAYS_ON_TOP
#endif
    );
    if (!window) {
        fprintf_s(stdout, "%d SDL Error: %s\n", __LINE__, SDL_GetError());
        shutdown = true;
        return -1;
    }
    SDL_ShowCursor(SDL_DISABLE);


    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf_s(stdout, "%d SDL Error: %s\n", __LINE__, SDL_GetError());
        shutdown = true;
        return -1;
    }

    init_return_val = IMG_Init(IMG_INIT_PNG);
    if (!(init_return_val & IMG_INIT_PNG)) {
        fprintf_s(stdout, "%d SDL_image Error: %s\n", __LINE__, IMG_GetError());
        shutdown = true;
        return -1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        fprintf_s(stdout, "%d SDL_mixer Error: %s\n", __LINE__, Mix_GetError());
        shutdown = true;
    }
    music_player.init();


    font = TTF_OpenFont("C:\\Windows\\Fonts\\vgasysr.fon", 0);
    if (!font) {
        fprintf_s(stdout, "%s\n", TTF_GetError());
        return -1;
    }
    font_impact = TTF_OpenFont("C:\\Windows\\Fonts\\impact.ttf", 72);
    if (!font_impact) {
        fprintf_s(stdout, "%s\n", TTF_GetError());
        return -1;
    }

    Texture::loadAtlas();

    music_player.playLooped();

    level_loader.changeState(LevelLoader::State::INTRO);

    return 0;
}

int Game::input() {
    //fprintf_s(stdout, "Game::input()\n");
    SDL_Event evt;
    

    while (SDL_PollEvent(&evt)) {
        switch (evt.type) {
        case SDL_QUIT:
#ifndef DISABLE_FULL_SCREEN_LOCK
            shutdown = true;
#endif
            break;
        case SDL_KEYDOWN:
            if (evt.key.keysym.sym == SDLK_ESCAPE) {
                input_state.exit = true;
            }
            if (evt.key.keysym.sym == SDLK_MINUS || evt.key.keysym.sym == SDLK_KP_MINUS) {
                music_player.changeVolume(-16);
            }
            if (evt.key.keysym.sym == SDLK_EQUALS || evt.key.keysym.sym == SDLK_KP_PLUS) {
                music_player.changeVolume(16);
            }
            if (evt.key.keysym.sym == SDLK_F5) {
                input_state.dbg_reload = true;
            }
            if (evt.key.keysym.sym == SDLK_KP_ENTER || evt.key.keysym.sym == SDLK_RETURN) {
                input_state.enter = true;
            }
            break;
        case SDL_KEYUP:
            if (evt.key.keysym.sym == SDLK_ESCAPE) {
                input_state.exit = false;
            }
            if (evt.key.keysym.sym == SDLK_F5) {
                input_state.dbg_reload = false;
            }
            if (evt.key.keysym.sym == SDLK_KP_ENTER || evt.key.keysym.sym == SDLK_RETURN) {
                input_state.enter = false;
            }
            break;
        case SDL_WINDOWEVENT:
            if (evt.window.event == SDL_WINDOWEVENT_FOCUS_LOST || evt.window.event == SDL_WINDOWEVENT_LEAVE) {
#ifndef DISABLE_FULL_SCREEN_LOCK
                SDL_RaiseWindow(window);
#endif
            }
            if (evt.window.event == SDL_WINDOWEVENT_MINIMIZED || evt.window.event == SDL_WINDOWEVENT_MAXIMIZED) {
#ifndef DISABLE_FULL_SCREEN_LOCK
                SDL_RestoreWindow(window);
#endif
            }
            break;
        default:
            break;
        }
    }

    Uint32 mouse_vars = SDL_GetGlobalMouseState(&input_state.mousePosition.x, &input_state.mousePosition.y);
    input_state.mouseHeld = SDL_BUTTON(mouse_vars) & SDL_BUTTON_LEFT;

    return 0;
}

int Game::update(float delta) {
    //fprintf_s(stdout, "Game::update()\n");
#ifndef DISABLE_FULL_SCREEN_LOCK
    bool hasToWarp = false;
    if (input_state.mousePosition.x > window_dimensions.x + window_dimensions.w - 2) {
        input_state.mousePosition.x = window_dimensions.x + 1;
        hasToWarp = true;
    }
    if (input_state.mousePosition.x < window_dimensions.x + 1) {
        input_state.mousePosition.x = window_dimensions.x + window_dimensions.w - 2;
        hasToWarp = true;
    }

    if (input_state.mousePosition.y > window_dimensions.y + window_dimensions.h - 2) {
        input_state.mousePosition.y = window_dimensions.y + 1;
        hasToWarp = true;
    }
    if (input_state.mousePosition.y< window_dimensions.y + 1) {
        input_state.mousePosition.y = window_dimensions.y + window_dimensions.h - 2;
        hasToWarp = true;
    }
    if (hasToWarp) { SDL_WarpMouseGlobal(input_state.mousePosition.x, input_state.mousePosition.y); }
#endif

    if (input_state.exit) {
        shutdown = true;
    }

    level_loader.update();

    return 0;
}

int Game::render() {
    //fprintf_s(stdout, "Game::render()\n");
    SDL_SetRenderDrawColor(renderer, 0, 0, 127, 255);
    SDL_RenderClear(renderer);

    // Render background
    SDL_Rect spriteRect = {
               0,0,
               window_dimensions.w,
               window_dimensions.h
    };

    for (int i = 0; i <= window_dimensions.h / Texture::texture_rect[Texture::ID::SPR_BACKGROUND].h; ++i) {
        for (int j = 0; j <= window_dimensions.w / Texture::texture_rect[Texture::ID::SPR_BACKGROUND].w; ++j) {
            spriteRect = {
                Texture::texture_rect[Texture::ID::SPR_BACKGROUND].w * j,
                Texture::texture_rect[Texture::ID::SPR_BACKGROUND].h * i,
                Texture::texture_rect[Texture::ID::SPR_BACKGROUND].w,
                Texture::texture_rect[Texture::ID::SPR_BACKGROUND].h
            };
            //fprintf_s(stdout, "dest: %d, %d, %d, %d\n", spriteRect.x, spriteRect.y, spriteRect.w, spriteRect.h);
            SDL_RenderCopy(renderer, Texture::atlas_texture, &Texture::texture_rect[Texture::ID::SPR_BACKGROUND],
                &spriteRect);
        }
    }

    level_loader.render();

   /* char fps_text[8] = { 0 };

    _ultoa_s(SDL_GetTicks64(), fps_text, 10);
    drawText(font_impact, fps_text, {10, 200, 500});*/

    SDL_RenderPresent(renderer);
    return 0;
}

int Game::terminate() {
	fprintf_s(stdout, "Shutting down...\n");
    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
    return 0;
}

SDL_Renderer* Game::getRenderer() {
    return this->renderer;
}

SDL_Rect Game::getWindowDimensions() {
    return window_dimensions;
}

SDL_Window* Game::getWindow() {
    return window;
}

void Game::drawText(TTF_Font* font, std::string text_, SDL_Rect position) {
    SDL_Surface* text = TTF_RenderUTF8_Solid_Wrapped(font, text_.c_str(), {0xFF, 0xFF, 0xFF, 0xFF}, position.w);
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text);
    SDL_QueryTexture(text_texture, NULL, NULL, &position.w, &position.h);

    SDL_RenderCopy(renderer, text_texture, NULL, &position);
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text);
}