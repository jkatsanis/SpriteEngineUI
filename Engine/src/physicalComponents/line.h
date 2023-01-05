#pragma once

#include <SFML/Graphics.hpp>
#include <calculationComponents/vector2.h>

namespace s2d
{
	class Line
	{		
	private:
	   Vector2 m_pointA;
	   Vector2 m_pointB;

	public:
		sf::Vertex line[2];

		Line();
		Line(Vector2 pointA, Vector2 pointB);
	};
}
