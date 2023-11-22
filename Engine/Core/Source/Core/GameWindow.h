#pragma once

#include <SFML/Graphics.hpp>

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

		GameWindow();
		GameWindow(const spe::Vector2& size, const std::string& name);

		bool IsOpen() const { return this->m_ptr_RenderGameWindow->isOpen(); }
		void Draw(spe::Sprite* ptr);
		void Display();
		void Clear();
	};
}