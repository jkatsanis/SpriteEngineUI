#pragma once

#include <SFML/Graphics.hpp>
#include <calculationComponents/vector2.h>
#include <spriteComponents/stransform.h>
#include <manager/lightRepository.h>

namespace s2d
{
	class Camera
	{
	private:
		sf::RenderWindow* ptr_renderWindow_to_engine;
		float m_camera_zoom;

		s2d::Vector2 getDefaultPosition();

	public:
		s2d::Transform transform;
		sf::View cameraView;

		float getZoom() const { return this->m_camera_zoom; }
		void setZoom(float zoom);

		Camera();
		Camera(sf::RenderWindow* ptr);

		void reset();
		void update();
	};
}