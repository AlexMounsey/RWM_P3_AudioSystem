#include "Wall.h"





Wall::Wall()
{
}

void Wall::setRect(SDL_Rect rect)
{
	m_rect = rect;
}
void Wall::render(SDL_Renderer &renderer)
{
	SDL_SetRenderDrawColor(&renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(&renderer, &m_rect);
}
