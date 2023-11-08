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
		s2d::SpriteRepository* m_ptr_sprite_repo;
		bool m_zoom_changed;
		s2d::Vector2 getDefaultPosition();

	public:
		s2d::Transform transform;
		sf::View camera_view;
		float camera_speed;

		Camera();
		Camera(sf::RenderWindow* ptr, s2d::SpriteRepository& repository);

		bool hasZoomChanged() const { return this->m_zoom_changed; }
		void setZoomFlag() { this->m_zoom_changed = false; }

		float getZoom() const { return this->m_camera_zoom; }
		void setZoom(float zoom);
		void reset();
		void update();
	};
}