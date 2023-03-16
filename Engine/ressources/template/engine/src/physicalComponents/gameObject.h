#pragma once

#include <spriteComponents/stransform.h>
#include <calculationComponents/vector2.h>
#include <physicalComponents/camera.h>
#include "SFML/Graphics.hpp"

namespace s2d
{
	class GameObject
	{
	public:
		GameObject() = delete;

	public:
		static float ssizeMultipliyer;
		static s2d::Camera camera;
		static std::vector <sf::RectangleShape> rects;
	};
}
