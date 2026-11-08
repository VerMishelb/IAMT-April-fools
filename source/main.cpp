#line 2 "main.cpp"
#include "../include/CI72Locker.h"
#include <iostream>
#include <SDL3/SDL_main.h>
#include "minidebug.h"

int main(int argc, char** argv) {
	miniprintf("Launching...");
	Game game{};
    if (game.init()) {
        game.terminate();
        return -1;
    }
	while (!game.shutdown) {
		game.input();
		game.update(1./60.);
		game.render();
		SDL_Delay(16);
	}
	miniprintf("Main loop ended.");
	game.terminate();
	return 0;
}