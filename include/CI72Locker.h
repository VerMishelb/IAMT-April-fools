#ifndef CI72Locker_h_
#define CI72Locker_h_

#include "../resource.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Types.h"
#include "Player.h"
#include "../include/ChickenHandler.h"
#include "../include/MusicPlayer.h"
#include "../include/LevelLoader.h"
#include "../include/DickClark.h"
#include "../include/MishDiscordActivity.h"

// #define DISABLE_FULL_SCREEN_LOCK
#define EMERGENCY_CTRL 1
#define EMERGENCY_7 2
#define EMERGENCY_2 4
#define EMERGENCY 7

#define BIGCHICKEN_C 1
#define BIGCHICKEN_H 2
#define BIGCHICKEN_K 4
#define BIGCHICKEN_N 8
#define BIGCHICKEN 15

struct Input {
    int emergency{ 0 };
    int bigchicken{ 0 };
    bool mouseHeld{ false };
    bool keyboardHeld{ false };
    SDL_Point mousePosition;
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
	SDL_Renderer* renderer;
	SDL_Window* window;

public:
    TTF_Font* font;
    TTF_Font* font_impact;
    Input input_state;
    BulletSpawner bulletSpawner;
    MusicPlayer music_player;
    ChickenHandler chicken_handler;
    DickClark dickclark_handler;
    LevelLoader level_loader;
    MishDiscordActivity mishdiscordactivity;

	Game();
	~Game();
	
	bool shutdown;

	int init();
	int input();
	int update(float delta);
	int render();

    void drawText(TTF_Font* font, std::string text, SDL_Rect position);
    SDL_Rect getWindowDimensions();
    SDL_Point getDesktopDimensions();

    SDL_Renderer* getRenderer();
    SDL_Window* getWindow();

	int terminate();

    bool makeWindowTransparent();
    bool gameFinished = false;
};

#endif