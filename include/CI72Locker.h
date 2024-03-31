#ifndef CI72Locker_h_
#define CI72Locker_h_

#include "../resource.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Types.h"

#define DISABLE_FULL_SCREEN_LOCK

class Game {
private:
	SDL_Rect window_dimensions;
	SDL_Renderer* renderer;
	SDL_Window* window;
    TTF_Font* font;
    SDL_Surface* text;

public:
	Game();
	~Game();
	
	bool shutdown;

	int init();
	int input();
	int update();
	int render();

	int terminate();

};

#endif