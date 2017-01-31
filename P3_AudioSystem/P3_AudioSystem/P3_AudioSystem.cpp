#define SDL_MAIN_HANDLED 
#include <iostream>
#include "Game.h"

int main() {
	Game g;
	g.initSDL(800, 600, "AudioSystem");
	g.loop();

	return 0;
}