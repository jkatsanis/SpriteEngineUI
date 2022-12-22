#pragma once

#include <SFML/Graphics.hpp>
#include <vector2.h>
#include <stime.h>

namespace s2d
{
	class Camera
	{
	private:
		sf::RenderWindow* m_ptr_renderWindow_to_engine;

		s2d::Vector2 getDefaultPosition();

	public:
		sf::View cameraView;
		float cameraZoom;

		Camera();
		Camera(sf::RenderWindow& ptr);

		void setPosition(const s2d::Vector2& position);

		void reset();
	};
}