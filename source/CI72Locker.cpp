#include "../include/CI72Locker.h"
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <SDL_syswm.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "../include/TaskmgrKiller.h"
#include "../include/Texture.h"
#include "../include/MishDiscordActivity.h"
#include <time.h>


Game* game = nullptr;

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
        SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_SKIP_TASKBAR
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

    mishdiscordactivity.init();
    discord::Activity activity{};
    activity.GetAssets().SetLargeImage("cimt_icon_large");
    activity.GetAssets().SetLargeText("I'll replace it");
    activity.SetDetails("Sophisticated GUI, indeed");
    activity.SetState("You fell for it");
    mishdiscordactivity.setActivity(activity);

    return 0;
}

int Game::input() {
    //fprintf_s(stdout, "Game::input()\n");
    SDL_Event evt;
    

    while (SDL_PollEvent(&evt)) {
        switch (evt.type) {
        case SDL_QUIT:
#ifdef DISABLE_FULL_SCREEN_LOCK /* ffs */
            shutdown = true;
#endif
            break;
        case SDL_KEYDOWN: {
            if (evt.key.keysym.sym == SDLK_LCTRL || evt.key.keysym.sym == SDLK_RCTRL) {
                input_state.emergency |= EMERGENCY_CTRL;
            }
            if (evt.key.keysym.sym == SDLK_7 || evt.key.keysym.sym == SDLK_KP_7) {
                input_state.emergency |= EMERGENCY_7;
            }
            if (evt.key.keysym.sym == SDLK_2 || evt.key.keysym.sym == SDLK_KP_2) {
                input_state.emergency |= EMERGENCY_2;
            }
            if ((input_state.emergency & (EMERGENCY_2 | EMERGENCY_7 | EMERGENCY_CTRL)) == 7) {
                input_state.exit = true;
            }
            else {
                input_state.exit = false;
            }
            if (evt.key.keysym.sym == SDLK_MINUS || evt.key.keysym.sym == SDLK_KP_MINUS) {
                music_player.changeVolume(-16);
            }
            if (evt.key.keysym.sym == SDLK_EQUALS || evt.key.keysym.sym == SDLK_KP_PLUS) {
                music_player.changeVolume(16);
            }
            if (evt.key.keysym.sym == SDLK_F2) {
                input_state.dbg_reload = true;
            }
            if (evt.key.keysym.sym == SDLK_KP_ENTER || evt.key.keysym.sym == SDLK_RETURN) {
                input_state.enter = true;
            }
            if (evt.key.keysym.sym == SDLK_SPACE || evt.key.keysym.sym == SDLK_z) {
                fprintf_s(stdout, "I have been called\n");
            }
            // This is a very paradoxical statement because even if I breakpoint the next line it
            // moves the breakpoint to "break" after the statement, automatically, skipping the "if"
            // even though the "if" is true.
            if (evt.key.keysym.sym == SDLK_SPACE || evt.key.keysym.sym == SDLK_z) {
                input_state.keyboardHeld == true;
            }
            break;
        }
        case SDL_KEYUP:
            if (evt.key.keysym.sym == SDLK_LCTRL || evt.key.keysym.sym == SDLK_RCTRL) {
                input_state.emergency = input_state.emergency & ~(EMERGENCY_CTRL);
            }
            if (evt.key.keysym.sym == SDLK_7 || evt.key.keysym.sym == SDLK_KP_7) {
                input_state.emergency = input_state.emergency & ~(EMERGENCY_7);
            }
            if (evt.key.keysym.sym == SDLK_2 || evt.key.keysym.sym == SDLK_KP_2) {
                input_state.emergency = input_state.emergency & ~(EMERGENCY_2);
            }
            if (evt.key.keysym.sym == SDLK_F2) {
                input_state.dbg_reload = false;
            }
            if (evt.key.keysym.sym == SDLK_KP_ENTER || evt.key.keysym.sym == SDLK_RETURN) {
                input_state.enter = false;
            }
            if (evt.key.keysym.sym == SDLK_SPACE || evt.key.keysym.sym == SDLK_z) {
                input_state.keyboardHeld == false;
            }
            break;
        case SDL_WINDOWEVENT: {
                if ((evt.window.event == SDL_WINDOWEVENT_FOCUS_LOST || evt.window.event == SDL_WINDOWEVENT_LEAVE) && !gameFinished) {
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
        }
        default:
            break;
        }
    }

    Uint32 mouse_vars = SDL_GetGlobalMouseState(&input_state.mousePosition.x, &input_state.mousePosition.y);
    input_state.mouseHeld = input_state.keyboardHeld || (SDL_BUTTON(mouse_vars) & SDL_BUTTON_LEFT);
    fprintf_s(stdout, "mouseHeld: %d\nkbHeld: %d\n\n", input_state.mouseHeld, input_state.keyboardHeld);

    return 0;
}

int Game::update(float delta) {
    //fprintf_s(stdout, "Game::update()\n");
#ifndef DISABLE_FULL_SCREEN_LOCK
    if (!gameFinished) {
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
        if (input_state.mousePosition.y < window_dimensions.y + 1) {
            input_state.mousePosition.y = window_dimensions.y + window_dimensions.h - 2;
            hasToWarp = true;
        }
        if (hasToWarp) { SDL_WarpMouseGlobal(input_state.mousePosition.x, input_state.mousePosition.y); }
    }
#endif

    if (input_state.exit) {
        shutdown = true;
    }

    level_loader.update();
    mishdiscordactivity.update();

    return 0;
}

int Game::render() {
    //fprintf_s(stdout, "Game::render()\n");
    SDL_SetRenderDrawColor(renderer, 0, 0, 127, 255);
    SDL_RenderClear(renderer);

    

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
    mishdiscordactivity.cleanup();
    return 0;
}

SDL_Renderer* Game::getRenderer() {
    return this->renderer;
}

SDL_Rect Game::getWindowDimensions() {
    return window_dimensions;
}

SDL_Point Game::getDesktopDimensions() {
    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(0, &mode);
    return { mode.w, mode.h };
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

bool Game::makeWindowTransparent() {
    COLORREF colourKey = RGB(255, 0, 255);
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);  // Initialize wmInfo
    SDL_GetWindowWMInfo(window, &wmInfo);
    HWND hWnd = wmInfo.info.win.window;

    // Change window type to layered (https://stackoverflow.com/a/3970218/3357935)
    SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

    // Set transparency color
    return SetLayeredWindowAttributes(hWnd, colourKey, 0, LWA_COLORKEY);
}