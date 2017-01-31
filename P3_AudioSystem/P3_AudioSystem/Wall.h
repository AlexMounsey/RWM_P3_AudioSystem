#pragma once
#include "SDL.h"
#include <vector>


struct RGB {
public:
	Uint8 r;
	Uint8 g;
	Uint8 b;
};
class Wall {
	friend class AudioManager;
protected:

	Wall();
	SDL_Rect  m_rect;
	
public:
	void setRect(SDL_Rect  rect);
	void render(SDL_Renderer &renderer);
};