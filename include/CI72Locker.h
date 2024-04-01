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

#define DISABLE_FULL_SCREEN_LOCK

struct Input {
    bool mouseHeld{ false };
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

	Game();
	~Game();
	
	bool shutdown;

	int init();
	int input();
	int update(float delta);
	int render();

    void drawText(TTF_Font* font, std::string text, SDL_Rect position);
    SDL_Rect getWindowDimensions();

    SDL_Renderer* getRenderer();
    SDL_Window* getWindow();

	int terminate();

};

#endif