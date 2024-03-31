#ifndef CI72Locker_h_
#define CI72Locker_h_

#include "../resource.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Types.h"
#include "Player.h"

#define DISABLE_FULL_SCREEN_LOCK

struct Input {
    bool mouseHeld{ false };
    SDL_Point mousePosition;
    bool exit{ false };

};

class Game;
extern Game* game;

class Game {
private:
	SDL_Rect window_dimensions;
	SDL_Renderer* renderer;
	SDL_Window* window;
    TTF_Font* font;
    SDL_Surface* text;
    Player player;

public:
    Input input_state;
    BulletSpawner bulletSpawner;
	Game();
	~Game();
	
	bool shutdown;

	int init();
	int input();
	int update();
	int render();

    SDL_Rect getWindowDimensions();

    SDL_Renderer* getRenderer();

	int terminate();

};

#endif