#include "Raycaster.h"

Raycaster::Raycaster()
{
}

void Raycaster::setPos(int x, int y) {
	m_position = { x, y };
}

void Raycaster::setPos(SDL_Point point) {
	m_position = point;
}

void Raycaster::addVertex(int x, int y) {
	m_vertices.push_back({ x, y });
}

void Raycaster::addVertex(SDL_Point vertex) {
	m_vertices.push_back(vertex);
}

float Raycaster::calMinT(Ray ray)
{
	float result = std::numeric_limits<float>::max();
	for (int i = 0; i < m_vertices.size(); i++) {
		SDL_Point pos = { m_position.x + m_vertices[i].x, m_position.y + m_vertices[i].y };
		SDL_Point dir;
		if (i == m_vertices.size() - 1) {
			dir.x = m_vertices[0].x - m_vertices[i].x;
			dir.y = m_vertices[0].y - m_vertices[i].y;
		}
		else {
			dir.x = m_vertices[i + 1].x - m_vertices[i].x;
			dir.y = m_vertices[i + 1].y - m_vertices[i].y;
		}
		float u = getRayIntersect(ray, { pos, dir });
		float v = getRayIntersect({ pos, dir }, ray);
		if (u >= 0 && v >= 0 && v <= 1)
		{
			if (u < result) {
				result = u;
			}
		}
	}
	return result;
}

float Raycaster::getRayIntersect(Ray a, Ray b)
{
	if ((a.dir.x * b.dir.y) - (a.dir.y * b.dir.x) != 0) {
		return static_cast<float>((a.pos.y * b.dir.x) + (b.dir.y * b.pos.x) - (b.pos.y * b.dir.x) - (b.dir.y * a.pos.x)) / static_cast<float>((a.dir.x * b.dir.y) - (a.dir.y * b.dir.x));
	}
	return -1;
}
