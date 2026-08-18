#include "Time.h"

#include "Input/Keyboard.h"

void spe::Time::Update()
{
	auto now = std::chrono::high_resolution_clock::now();

	// delta time in seconds
	std::chrono::duration<float> delta = now - m_LastTime;
	s_DeltaTime = delta.count();
	m_LastTime = now; // just update, no "restart" overhead



	// FPS calculation
	if (m_SecondCounter <= 1.0f) {
		m_SecondCounter += s_DeltaTime;
		m_TempFPS++;
	} else {
		s_FPS = m_TempFPS;
		m_SecondCounter = 0.0f;
		m_TempFPS = 0;
	}

	s_Ticks++;
	s_TimePassed += s_DeltaTime;
}

void spe::Time::Reset() noexcept
{
	spe::Time::s_FPS = 0;
	spe::Time::s_DeltaTime = 0;
	spe::Time::m_SecondCounter = 0;
	spe::Time::m_TempFPS = 0;
}

float spe::Time::s_DeltaTime = 0;
sf::Clock spe::Time::s_DeltaClock;

float spe::Time::s_Accumulator = 0;
float spe::Time::s_FixedDeltaTime = FIXED_DELTA_TIME;

float spe::Time::m_SecondCounter = 0;
float spe::Time::m_TempFPS = 0;

float spe::Time::s_FPS = 0;
float spe::Time::s_Ticks = 0;
float spe::Time::s_TimePassed = 0;

std::chrono::high_resolution_clock::time_point spe::Time::m_LastTime = std::chrono::high_resolution_clock::now();

