#pragma once

#include <calculationComponents/vector2.h>
#include <calculationComponents/vector3.h>

namespace s2d
{
	class LightSource
	{
	public:
		LightSource();
		LightSource(const s2d::Vector2& pos, float radius, float intensiti, const s2d::Vector3& color);

		s2d::Vector2 position;
		float radius;
		float light_intensities;
		s2d::Vector3 color;
	};
}

