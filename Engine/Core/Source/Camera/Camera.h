#pragma once

#include <Include/SFML/Graphics.hpp>

#include "Math/Vector2.h"
#include "Sprite/SpriteRepository.h"
#include "Sprite/Components/Transform.h"
#include "Sprite/Components/Light/LightRepository.h"

namespace spe
{
	class Camera
	{
	private:
		sf::RenderWindow* ptr_Window;;
		float m_camera_zoom;
		spe::SpriteRepository* m_ptr_sprite_repo;
		bool m_zoom_changed;
		spe::Vector2 getDefaultPosition();

	public:
		spe::Vector2 Position;
		sf::View camera_view;
		float camera_speed;

		Camera();
		Camera(sf::RenderWindow* ptr, spe::SpriteRepository& repository);

		bool hasZoomChanged() const { return this->m_zoom_changed; }
		void setZoomFlag() { this->m_zoom_changed = false; }

		float getZoom() const { return this->m_camera_zoom; }
		void setZoom(float zoom);
		void reset();
		void Update();
	};
}