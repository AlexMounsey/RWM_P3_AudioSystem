#pragma once
#include "SDL.h"
#include <vector>

struct Ray {
	SDL_Point pos;
	SDL_Point dir;
};
class Raycaster {
	friend class AudioManager;
private:
	std::vector<SDL_Point> m_vertices;
	SDL_Point m_position;
public:
	Raycaster();
	void setPos(int x, int y);
	void addVertex(SDL_Point vertex);
	void setPos(SDL_Point point);
	void addVertex(int x, int y);
	float calMinT(Ray ray);
	float getRayIntersect(Ray a, Ray b);






};