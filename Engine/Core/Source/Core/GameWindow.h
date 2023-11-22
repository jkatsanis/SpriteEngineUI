#pragma once

#include <SFML/Graphics.hpp>
#include "ImGui/ImGUISFML.h"

#include "Core/Event.h"
#include "Components/Light/LightRepository.h"
#include "Sprite.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"

namespace spe
{
	class GameWindow
	{
	private:
		sf::RenderWindow* m_ptr_RenderGameWindow;

	public:
		spe::Vector3 BackgroundColor;
		sf::Event WindowEvent;
		spe::Event Event;

		GameWindow();
		GameWindow(const spe::Vector2& size, const std::string& name);

		void PollEvents();
		bool IsOpen() const { return this->m_ptr_RenderGameWindow->isOpen(); }
		void Draw(spe::Sprite* ptr);
		void Display();
		void Clear();
	};
}