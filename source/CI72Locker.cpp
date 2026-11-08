#line 2 "CI72Locker.cpp"
#include "../include/CI72Locker.h"
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
// #include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "../include/TaskmgrKiller.h"
#include "../include/Texture.h"
#include "minidebug.h"
#include <time.h>


Game *game = nullptr;


Game::Game() : window_dimensions({0, 0, 800, 600}), renderer(nullptr), window(nullptr), shutdown(false) /*, font(nullptr), font_impact(nullptr)*/ {
    if (!game) {
        game = this;
    }
}

Game::~Game() {}



int Game::init() {
    srand(time(NULL)); // Почему это здесь-то?..
    miniprintfvalloc("%d", SDL_VERSION);
    if (!SDL_SetHint(SDL_HINT_ALLOW_ALT_TAB_WHILE_GRABBED, "0")) {
        miniprintf("Couldn't set SDL_HINT_ALLOW_ALT_TAB_WHILE_GRABBED to 0.");
    }
    if (!SDL_SetHint(SDL_HINT_FORCE_RAISEWINDOW, "1")) {
        miniprintf("Couldn't set SDL_HINT_FORCE_RAISEWINDOW to 1.");
    }
    
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		miniprintf("SDL_Init failed: %s", SDL_GetError());
		shutdown = true;
		return -1;
	}
    // SDL_ttf не собирается без freetype. На ноутбуке нет freetype, на даче нет интернета, у меня нет растровой системы шрифтов.
    /* init_return_val = TTF_Init();
    if (init_return_val) {
        fprintf_s(stdout, "%d SDL_ttf Error %d: %s\n", __LINE__, init_return_val, TTF_GetError());
        return -1;
    } */

    auto display = SDL_GetPrimaryDisplay();
    if (SDL_GetDisplayBounds(display, &window_dimensions)) { // THIS RETURNS 800x600! WHY?!
        miniprintfvalloc("%d, %d", window_dimensions.w, window_dimensions.h);
    }
    else {
        miniprintf("SDL_GetDisplayBounds failed: %s", SDL_GetError());
    }
    auto window_flags = 0ULL
#ifdef MALICIOUS
    | SDL_WINDOW_BORDERLESS | SDL_WINDOW_UTILITY | SDL_WINDOW_ALWAYS_ON_TOP
#endif
    ;

#ifndef MALICIOUS
    window_dimensions = { 50, 50, 640*2, 480*2 };
    miniprintfvalloc("%d, %d (windowed)", window_dimensions.w, window_dimensions.h);
#endif


    static const char* title = 
    #ifdef MALICIOUS
        "IAMT 3.5.2 GUI"
    #else
        "CI72 winlocker let's make this a very long title so it annoys me EVERY SINGLE TIME I have to end it through the task manager because I forgot to change some variable and it's broken again"
    #endif
    ;

if (!SDL_CreateWindowAndRenderer(title, window_dimensions.w, window_dimensions.h, window_flags, &window, &renderer)
    ) {
        miniprintf("Couldn't create window & renderer: %s", SDL_GetError());
        shutdown = true;
        return -1;
    }
    SDL_HideCursor();

    if (!music_player.init()) {
        miniprintf("Music_player.init failed.");
        shutdown = true;
        return -1;
    }


/*     font = TTF_OpenFont("C:\\Windows\\Fonts\\bahnschrift.ttf", 24);
    if (!font) {
        fprintf_s(stdout, "%s\n", TTF_GetError());
        return -1;
    }
    font_impact = TTF_OpenFont("C:\\Windows\\Fonts\\wingding.ttf", 72);
    if (!font_impact) {
        fprintf_s(stdout, "%s\n", TTF_GetError());
        return -1;
    }
 */
    Texture::loadAtlas();
    music_player.playLooped(); // а где аргумент-то? лол
    level_loader.changeState(LevelLoader::State::INTRO);
    return 0;
}

int Game::input() { // Зачем я возвращал инт? О чём я думал?
    SDL_Event evt;

    while (SDL_PollEvent(&evt)) {
        switch (evt.type) {
        case SDL_EVENT_QUIT:
#ifndef MALICIOUS
            shutdown = true;
#endif
            break;
        case SDL_EVENT_KEY_DOWN:
            if (evt.key.key == SDLK_LCTRL || evt.key.key == SDLK_RCTRL) {
                input_state.emergency |= EMERGENCY_CTRL;
            }
            if (evt.key.key == SDLK_7 || evt.key.key == SDLK_KP_7) {
                input_state.emergency |= EMERGENCY_7;
            }
            if (evt.key.key == SDLK_2 || evt.key.key == SDLK_KP_2) {
                input_state.emergency |= EMERGENCY_2;
            }
            if ((input_state.emergency & (EMERGENCY_2 | EMERGENCY_7 | EMERGENCY_CTRL)) == 7) {
                input_state.exit = true;
            }
            else {
                input_state.exit = false;
            }
            if (evt.key.key == SDLK_MINUS || evt.key.key == SDLK_KP_MINUS) {
                music_player.addVolume(-16);
            }
            if (evt.key.key == SDLK_EQUALS || evt.key.key == SDLK_KP_PLUS) {
                music_player.addVolume(16);
            }
            if (evt.key.key == SDLK_F2) {
                input_state.dbg_reload = true;
            }
            if (evt.key.key == SDLK_KP_ENTER || evt.key.key == SDLK_RETURN) {
                input_state.enter = true;
            }
#ifndef MALICIOUS // Debug stuff
            if (evt.key.key == SDLK_KP_1) {
                level_loader.changeState(LevelLoader::State::TITLE);
            }
            if (evt.key.key == SDLK_KP_2) {
                level_loader.changeState(LevelLoader::State::LVL_CHICKENS);
            }
            if (evt.key.key == SDLK_KP_3) {
                level_loader.changeState(LevelLoader::State::LVL_DICK_CLARK);
            }
            if (evt.key.key == SDLK_KP_4) {
                level_loader.changeState(LevelLoader::State::LVL_USELESS);
            }
#endif
            break;
        case SDL_EVENT_KEY_UP:
            if (evt.key.key == SDLK_LCTRL || evt.key.key == SDLK_RCTRL) {
                input_state.emergency = input_state.emergency & ~(EMERGENCY_CTRL);
            }
            if (evt.key.key == SDLK_7 || evt.key.key == SDLK_KP_7) {
                input_state.emergency = input_state.emergency & ~(EMERGENCY_7);
            }
            if (evt.key.key == SDLK_2 || evt.key.key == SDLK_KP_2) {
                input_state.emergency = input_state.emergency & ~(EMERGENCY_2);
            }
            if (evt.key.key == SDLK_F2) {
                input_state.dbg_reload = false;
            }
            if (evt.key.key == SDLK_KP_ENTER || evt.key.key == SDLK_RETURN) {
                input_state.enter = false;
            }
            break;
        case SDL_EVENT_WINDOW_FOCUS_LOST:
        case SDL_EVENT_WINDOW_MOUSE_LEAVE: {
            if (!gameFinished) {
#ifdef MALICIOUS
                SDL_RaiseWindow(window);
#endif
                }
            }
            break;
        case SDL_EVENT_WINDOW_MINIMIZED: // [[fallthrough]] thing might be necessary to shut the warnings up.
        case SDL_EVENT_WINDOW_MAXIMIZED: {
#ifdef MALICIOUS
                SDL_RestoreWindow(window);
#endif
            break;
        }
        default:
            break;
        }
    }
#ifdef MALICIOUS
    Uint32 mouse_vars = SDL_GetGlobalMouseState(&input_state.mousePosition.x, &input_state.mousePosition.y);
#else
    Uint32 mouse_vars = SDL_GetMouseState(&input_state.mousePosition.x, &input_state.mousePosition.y);
#endif
    input_state.mouseHeld = SDL_BUTTON_MASK(mouse_vars) & SDL_BUTTON_LEFT;

    return 0;
}

int Game::update(float delta) {
    //fprintf_s(stdout, "Game::update()\n");
#ifdef MALICIOUS
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
#undef minidebug_miniprintf_func
#define minidebug_miniprintf_func printf
	miniprintf("Shutting down...\n");
    // TTF_Quit();
    SDL_Quit(); // Пару раз подвисло, пока с SDL3_mixer копался. Почему?
    miniprintf("All systems shut down.\n");
    // miniprintf("*** Press Enter to close the console ***\n"); getc(stdin);
#undef minidebug_miniprintf_func
#define minidebug_miniprintf_func SDL_Log // На всякий случай. Мало ли, что там препроцессор придумает.
    return 0;
}

SDL_Renderer* Game::getRenderer() {
    return this->renderer;
}

SDL_Rect Game::getWindowDimensions() {
    return window_dimensions;
}

SDL_Point Game::getDesktopDimensions() {
    const SDL_DisplayMode* mode;
    SDL_DisplayID display = SDL_GetDisplayForWindow(window);
    mode = SDL_GetCurrentDisplayMode(display);
    return { mode->w, mode->h };
}

SDL_Window* Game::getWindow() {
    return window;
}
    SDL_Window* Game::getWindowBigChicken() {
        return window_bigchicken;
    }
    SDL_Window* Game::getWindowEgg() {
        return window_bigchicken_eggs[0];
    }

/* void Game::drawText(TTF_Font* font, std::string text_, SDL_Rect position) {
    SDL_Surface* text = TTF_RenderUTF8_Solid_Wrapped(font, text_.c_str(), {0xFF, 0xFF, 0xFF, 0xFF}, position.w);
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text);
    SDL_QueryTexture(text_texture, NULL, NULL, &position.w, &position.h);

    SDL_RenderTexture(renderer, text_texture, NULL, &position);
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text);
} */

bool Game::makeWindowTransparent() {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, __func__, "Transparent window is not ported to SDL3 yet.\n", NULL);
    SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    SDL_MinimizeWindow(window);
    
    auto props = SDL_CreateProperties();
    SDL_SetPointerProperty(props, SDL_PROP_WINDOW_CREATE_PARENT_POINTER, window);
    SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, "Big chicken #2147483701");
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, 123);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, 120);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_TRANSPARENT_BOOLEAN, true);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_ALWAYS_ON_TOP_BOOLEAN, true);
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_TOOLTIP_BOOLEAN, true);

    // A tooltip requires a parent window according to sdlwiki/SDL3/SDL_WindowFlags.html. Not sure how to work with that if I only need one window, though.
    window_bigchicken = SDL_CreateWindowWithProperties(props);
    ERRCHK_SDL(window_bigchicken);
    SDL_DestroyProperties(props);
    // renderer = SDL_CreateRenderer(window_bigchicken, nullptr);
    ERRCHK_SDL(renderer = SDL_CreateRenderer(window_bigchicken, nullptr));
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    Texture::loadAtlas();
    // COLORREF colourKey = RGB(255, 0, 255);
    // SDL_SysWMinfo wmInfo;
    // SDL_VERSION(&wmInfo.version);  // Initialize wmInfo
    // SDL_GetWindowWMInfo(window, &wmInfo);
    // HWND hWnd = (HWND)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
    //wmInfo.info.win.window;

    // Change window type to layered (https://stackoverflow.com/a/3970218/3357935)
    // SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

    // Set transparency color
    // return SetLayeredWindowAttributes(hWnd, colourKey, 0, LWA_COLORKEY);
    return true;
}