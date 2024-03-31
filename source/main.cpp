#include "../include/CI72Locker.h"
#include <thread>
#include <chrono>
#include <iostream>

int main(int argc, char** argv) {
	fprintf_s(stdout, "Launching...\n");
	Game game{};
    if (game.init()) {
        game.terminate();
        return -1;
    }
	while (!game.shutdown) {
		game.input();
		game.update();
		game.render();
		//std::this_thread::sleep_for(std::chrono::milliseconds(16));
		SDL_Delay(16);
	}
	fprintf_s(stdout, "Main loop ended\n");
	game.terminate();
	return 0;
}