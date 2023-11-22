#pragma once

#include <SFML/Graphics.hpp>

#include "Math/Vector2.h"

namespace spe
{
	class Window
	{
	private:
		sf::RenderWindow* m_ptr_RenderWindow;

	public:
		

		Window();
		Window(const spe::Vector2& size, const std::string& name);

		bool IsOpen() const { return this->m_ptr_RenderWindow->isOpen(); }

		void Clear();
		void Draw();
	};
}