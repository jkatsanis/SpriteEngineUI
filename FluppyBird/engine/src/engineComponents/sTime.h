#pragma once

#include <iostream>
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
		static float timePassed;

		Time() = delete;

		static void update();
	};

}

