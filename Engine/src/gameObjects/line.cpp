#include "line.h"

s2d::Line::Line() { }

s2d::Line::Line(Vector2 pointA, Vector2 pointB)
{
	this->m_pointA = pointA;
	this->m_pointB = pointB;

	/*int pointAX = 250 + 640 + (int)pointA.x;
	int pointAY = 100 + 360 - int(pointA.y);
	
	int pointBX = 250 + 640 + (int)pointB.x;
	int pointBY = 100 + 360 - int(pointB.y);*/

	line[0] = sf::Vertex(sf::Vector2f(m_pointA.x, m_pointA.y));
	line[1] = sf::Vertex(sf::Vector2f(m_pointB.x, m_pointB.y));
}
