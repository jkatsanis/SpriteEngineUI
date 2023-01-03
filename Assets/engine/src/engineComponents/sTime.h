#pragma once

#include <SFML/Graphics.hpp>

namespace s2d
{
	class Time
	{
	private:
		static float m_secondCounter;
		static float m_tempFps;

	public:
		static float fps;

		static float deltaTime;
		static sf::Clock deltaClock;

		Time() = delete;

		static void update();
	};

}

