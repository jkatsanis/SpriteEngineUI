#pragma once

#include <SFML/Graphics.hpp>
#include <vector2.h>
#include <stransform.h>

namespace s2d
{
	class Camera
	{
	private:

		sf::RenderWindow* ptr_renderWindow_to_engine;

		s2d::Vector2 getDefaultPosition();

	public:
		s2d::Transform transform;
		sf::View cameraView;
		float cameraZoom;

		Camera();
		Camera(sf::RenderWindow* ptr);

		void reset();
		void update();
	};
}