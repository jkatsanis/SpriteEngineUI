#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

namespace spe
{
	class Time
	{
	private:
		static float s_second_counter;
		static float m_tempFps;

	public:
		static float s_fps;
		static float s_time_passed;
		static float s_ticks;

		static float s_delta_time;
		static sf::Clock s_delta_clock;

		Time() = delete;

		static void update();
		static void reset();
	};

}

