#ifndef CI72Locker_h_
#define CI72Locker_h_

#include "../resource.h"
#include <SDL3/SDL.h>
#if USE_TTF_FONTS == 1 // Бля, да как там его
#include <SDL3_ttf/SDL_ttf.h>
#endif
#include <string>
#include "Types.h"
#include "Player.h"
#include "../include/ChickenHandler.h"
#include "../include/MusicPlayer.h"
#include "../include/LevelLoader.h"
#include "../include/DickClark.h"


#define EMERGENCY_CTRL 1
#define EMERGENCY_7 2
#define EMERGENCY_2 4

struct Input {
    int emergency{ 0 };
    bool mouseHeld{ false };
    SDL_FPoint mousePosition;
    bool exit{ false };
    bool dbg_reload;
    bool enter{ false };
};

class Game;
extern Game* game;

class Game {
private:
	SDL_Rect window_dimensions;
    SDL_Rect desktop_dimensions;
	SDL_Renderer* renderer {nullptr};
	SDL_Window* window {nullptr}, *window_bigchicken {nullptr}, *window_bigchicken_eggs[3] = {nullptr};

public:
    // TTF_Font* font;
    // TTF_Font* font_impact;
    Input input_state;
    BulletSpawner bulletSpawner;
    MusicPlayer music_player;
    ChickenHandler chicken_handler;
    DickClark dickclark_handler;
    LevelLoader level_loader;

	Game();
	~Game();
	
	bool shutdown;

	int init();
	int input();
	int update(float delta);
	int render();

    #if USE_TTF_FONTS == 1
    void drawText(TTF_Font* font, std::string text, SDL_Rect position);
    #else
    #define drawText() ; // FIXME drawText no TTF stub.
    #endif
    SDL_Rect getWindowDimensions();
    SDL_Point getDesktopDimensions();

    SDL_Renderer* getRenderer();
    SDL_Window* getWindow();
    SDL_Window* getWindowBigChicken();
    SDL_Window* getWindowEgg();

	int terminate();

    bool makeWindowTransparent();
    bool gameFinished = false;
};

#endif