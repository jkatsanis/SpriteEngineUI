#include "Time.h"

void spe::Time::Update()
{
	sf::Time dt = s_DeltaClock.restart();
	spe::Time::s_DeltaTime = dt.asSeconds();

	if (m_SecondCounter <= 1) {
		m_SecondCounter += s_DeltaTime;
		m_TempFPS++;
	}
	else
	{
		s_FPS = m_TempFPS;
		m_SecondCounter = 0;
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

float spe::Time::m_SecondCounter = 0;
float spe::Time::m_TempFPS = 0;

float spe::Time::s_FPS = 0;
float spe::Time::s_Ticks = 0;
float spe::Time::s_TimePassed = 0;

