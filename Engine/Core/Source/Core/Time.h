#pragma once

#include <Include/SFML/Graphics.hpp>
#include <iostream>

namespace spe
{
	class Time
	{
	private:
		static float m_SecondCounter;
		static float m_TempFPS;

	public:
		static float s_FPS;
		static float s_TimePassed;
		static float s_Ticks;

		static float s_DeltaTime;
		static sf::Clock s_DeltaClock;

		Time() = delete;

		static void Update();
		static void Reset() noexcept;
	};

}

