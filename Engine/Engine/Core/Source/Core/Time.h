#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>

namespace spe
{
	class Time
	{
	private:
		static float m_SecondCounter;
		static float m_TempFPS;
		static  std::chrono::high_resolution_clock::time_point m_LastTime;
 ;		static sf::Time m_Time;

	public:
		static float s_FPS;
		static float s_TimePassed;
		static float s_Ticks;

		static float s_DeltaTime;
		static sf::Clock s_DeltaClock;

		// fixed timestep for physics (60 Hz)
		static constexpr float FIXED_DELTA_TIME = 1.0f / 60.0f;
		static constexpr float MAX_FRAME_TIME = 0.25f;
		static float s_Accumulator;
		static float s_FixedDeltaTime; // current fixeddelta time being used

		Time() = delete;

		static void Update();
		static void Reset() noexcept;
	};

}

