#pragma once

#include "Math/Vector2.h"
#include "Math/Vector3.h"

namespace spe
{
	class LightSource
	{
	public:
		LightSource();
		LightSource(const spe::Vector2& pos, float radius, float intensiti, const sf::Vector3f& color);

		spe::Vector2 Position;
		float Radius;
		float LightIntensity;
		sf::Vector3f Color ;
	};
}

