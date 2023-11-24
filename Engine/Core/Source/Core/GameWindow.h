#pragma once

#include <Include/SFML/Graphics.hpp>
#include <Include/ImGui/ImGUISFML.h>

#include "Camera/Camera.h"
#include "Core/Event.h"
#include "Sprite/Components/Light/LightRepository.h"
#include "Sprite/Sprite.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"

namespace spe
{
	class GameWindow
	{
	private:
		sf::RenderWindow* m_ptr_Window;
		spe::Camera* m_Camera;
		spe::Vector2 m_Size;
		spe::Vector3* m_BackgroundColor;
		bool m_IsOpen;

		void UpdateCamera();

	public:
		sf::Event WindowEvent;
		spe::Event Event;

		GameWindow();
		GameWindow(const spe::Vector2& size, const std::string& name);

		void PollEvents();
		bool IsOpen() const { return this->m_IsOpen; }
		void Draw(spe::Sprite* ptr);
		void Display();
		void Clear();
		void Shutdown();

		void SetCamera(spe::Camera* camera) { this->m_Camera = camera; }
		void SetBackgroundColor(spe::Vector3* bck) { this->m_BackgroundColor = bck; }
	};
}