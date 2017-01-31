#ifndef BLOCK_H_
#define BLOCK_H_
#include "SDL.h"
class Block {
	SDL_Rect m_rect;

public:
	Block(float x , float y , float w, float h ) {
		m_rect.x = x;
		m_rect.y = y;
		m_rect.w = w;
		m_rect.h = h;
	}
	void draw(SDL_Renderer *renderer) {

			SDL_SetRenderDrawColor(renderer, 155, 155, 255, 255);
			SDL_RenderFillRect(renderer, &m_rect);

	}


	void setPos(float x, float y)
	{
		m_rect.x = x;
		m_rect.y = y;
	}

	SDL_Point getPos()
	{
		SDL_Point temp = { m_rect.x, m_rect.y };
		return temp;	
	}

};
#endif