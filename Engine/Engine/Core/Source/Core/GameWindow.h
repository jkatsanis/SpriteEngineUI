#pragma once

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

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
		sf::IntRect m_WindowBounds;

		void UpdateCamera();
		void Draw(spe::Sprite* ptr, const sf::Shader* shader, bool ignoreLight);

	public:
		sf::Event WindowEvent;
		spe::Event Event;

		GameWindow();
		GameWindow(const spe::Vector2& size, const std::string& name);

		void PollEvents();
		bool IsOpen() const noexcept { return this->m_IsOpen; }
		void DrawEngine(spe::Sprite* ptr, const sf::Shader* shader, bool ignoreLight);
		void DrawGame(spe::Sprite* ptr, const sf::Shader* shader, bool ignoreLight);

		void Display();
		void Clear();
		void Shutdown();

		bool ContainsCursor();

		sf::RenderWindow* GetRenderWindow() noexcept { return this->m_ptr_Window; }

		void SetCamera(spe::Camera* camera) noexcept { this->m_Camera = camera; }
		void SetBackgroundColor(spe::Vector3* bck) noexcept { this->m_BackgroundColor = bck; }
	};
}