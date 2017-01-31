#pragma once
#include "SDL.h"
#include "AudioManager.h"



class Game {
private:
	SDL_Window * window;
	SDL_Renderer * renderer;

	Wall * w1;
	Wall * w2;
	Wall * w3;
	Wall * w4;
	Wall * w5;
	Raycaster * s1;
	Raycaster * s2;
	Raycaster * s3;
	Raycaster * s4;
	bool sourcePlaced = false;
	bool listenerPlaced = false;
	SDL_Rect sr1;
	SDL_Rect sr2;
	SDL_Rect sr3;
	SDL_Rect sr4;
	SDL_Rect sr5;
public:
	Game();
	bool initSDL(int width, int height, const char * title);
	void loop();
	void KeyboardEvents();
};


