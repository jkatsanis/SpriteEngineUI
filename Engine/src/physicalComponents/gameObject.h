#pragma once

#include <spriteComponents/stransform.h>
#include <calculationComponents/vector2.h>
#include <physicalComponents/line.h>
#include "SFML/Graphics.hpp"
#include <physicalComponents/camera.h>	

namespace s2d
{
	class GameObject
	{
	public:
		GameObject() = delete;

	public:
		static std::vector<s2d::Line> lines;

		//Used for box collider, and game window
		static std::vector <sf::RectangleShape> rects;

		//Pointer to the camera in "UIRealTimeEditor"
		static s2d::Camera* ptr_camera_tRealTimeEditor;
	};
}
