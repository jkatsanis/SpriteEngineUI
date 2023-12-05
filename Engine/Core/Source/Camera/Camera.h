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
		sf::RenderWindow* m_ptr_Window;;
		float m_CameraZoom;
		spe::SpriteRepository* m_ptr_Repo;
		bool m_ZoomChanged;
		spe::Vector2 GetDefaultPosition() noexcept;

	public:
		spe::Vector2 Position;
		sf::View CameraView;
		float CameraSpeed;

		Camera();
		Camera(sf::RenderWindow* ptr, spe::SpriteRepository& repository);

		[[nodiscard]] bool HasZoomChanged() const noexcept { return this->m_ZoomChanged; }
		void SetZoomFlag() noexcept { this->m_ZoomChanged = false; }

		float GetZoom() const noexcept { return this->m_CameraZoom; }
		void SetZoom(float zoom) noexcept;
		void Reset() noexcept;
		void Update(spe::LightRepository* lightrepo);
	};
}