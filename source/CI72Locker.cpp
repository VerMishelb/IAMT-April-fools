#include "../include/CI72Locker.h"
#include <stdio.h>
#include <SDL_ttf.h>
#include "../include/TaskmgrKiller.h"

const char* testtextline = ":(\nA problem has been detected with your puter, please kill.\n";

Game::Game() : window_dimensions({0, 0, 800, 600}), renderer(nullptr), window(nullptr), shutdown(false), text(nullptr), font(nullptr) {

}

Game::~Game() {}



int Game::init() {
    if (!SDL_SetHint(SDL_HINT_ALLOW_ALT_TAB_WHILE_GRABBED, "0")) {
        fprintf_s(stdout, "Couldn't set alt tab hint.\n");
    }
    if (!SDL_SetHint(SDL_HINT_FORCE_RAISEWINDOW, "1")) {
        fprintf_s(stdout, "Couldn't set force raise window hint.\n");
    }
    int init_return_val = SDL_Init(SDL_INIT_VIDEO);
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
        SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP  /*SDL_WINDOW_FULLSCREEN*/);
    if (!window) {
        fprintf_s(stdout, "%d SDL Error: %s\n", __LINE__, SDL_GetError());
        shutdown = true;
        return -1;
    }


    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf_s(stdout, "%d SDL Error: %s\n", __LINE__, SDL_GetError());
        shutdown = true;
        return -1;
    }

    font = TTF_OpenFont("C:\\Windows\\Fonts\\vgasysr.fon", 0);
    if (!font) {
        fprintf_s(stdout, "Could not open vgasysr.fon\n");
        return -1;
    }

    return 0;
}

int Game::input() {
    SDL_Event evt;
    

    while (SDL_PollEvent(&evt)) {
        switch (evt.type) {
        case SDL_QUIT:
            shutdown = true;
            break;
        case SDL_KEYDOWN:
            if (evt.key.keysym.sym == SDLK_ESCAPE) {
                shutdown = true;
            }
            if (evt.key.keysym.sym == SDLK_x) {
                fprintf_s(stdout, "Trying to kill task manager...\n");
                TaskmgrKiller::KillTaskmgr();
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

    return 0;
}

int Game::update() {
    // Make a separate process for killing task manager every second or so
    int x = 0, y = 0;
    SDL_GetGlobalMouseState(&x, &y);

#ifndef DISABLE_FULL_SCREEN_LOCK
    if (x > window_dimensions.x + window_dimensions.w - 2) {
        x = window_dimensions.x + 1;
    }
    if (x < window_dimensions.x + 1) {
        x = window_dimensions.x + window_dimensions.w - 2;
    }

    if (y > window_dimensions.y + window_dimensions.h - 2) {
        y = window_dimensions.y + 1;
    }
    if (y < window_dimensions.y + 1) {
        y = window_dimensions.y + window_dimensions.h - 2;
    }
    SDL_WarpMouseGlobal(x, y);
#endif
    return 0;
}

int Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);

    text = TTF_RenderUTF8_Solid_Wrapped(font, testtextline, { 0xFF, 0xFF, 0xFF, 0xFF }, 0);
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text);
    SDL_FreeSurface(text);
    SDL_Rect text_rect{ 10, 10, text->w, text->h };
    
    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
    SDL_DestroyTexture(text_texture);
    SDL_RenderPresent(renderer);
    return 0;
}

int Game::terminate() {
	fprintf_s(stdout, "Shutting down...\n");
    TTF_Quit();
    SDL_Quit();
    return 0;
}