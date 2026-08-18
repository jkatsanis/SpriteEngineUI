#pragma once

#include <SFML/Graphics.hpp>


#include "Math/Vector2.h"
#include "Sprite/SpriteRepository.h"
#include "Sprite/Components/Transform.h"
#include "Sprite/Components/Light/LightRepository.h"

namespace spe
{
	class Camera
	{
	private:
		float m_CameraZoom;
		bool m_ZoomChanged;
		spe::Vector2 m_WindowSize; // Added window size
		spe::Vector2 GetDefaultPosition() noexcept;

	public:
		spe::Vector2 Position;
		sf::View CameraView;
		float CameraSpeed;

		Camera();

		[[nodiscard]] bool HasZoomChanged() const noexcept { return this->m_ZoomChanged; }
		void SetZoomFlag() noexcept { this->m_ZoomChanged = false; }

		float GetZoom() const noexcept { return this->m_CameraZoom; }
		void SetZoom(float zoom) noexcept;
		void Reset() noexcept;
		void Update(spe::LightRepository* lightrepo);

		// New method to set window size
		void SetWindowSize(const spe::Vector2& size) noexcept { this->m_WindowSize = size; }
		spe::Vector2 GetWindowSize() const noexcept { return this->m_WindowSize; }
	};
}
