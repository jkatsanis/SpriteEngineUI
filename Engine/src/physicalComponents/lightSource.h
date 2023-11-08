#pragma once

#include <calculationComponents/vector2.h>
#include <calculationComponents/vector3.h>

namespace s2d
{
	class LightSource
	{
	public:
		LightSource();
		LightSource(const s2d::Vector2& pos, float radius, float intensiti, const sf::Vector3f& color);

		s2d::Vector2 position;
		float radius;
		float light_intensities;
		sf::Vector3f color;
	};
}

