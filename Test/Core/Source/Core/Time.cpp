#include "Time.h"

void spe::Time::update()
{
	sf::Time dt = s_delta_clock.restart();
	spe::Time::s_delta_time = dt.asSeconds();

	if (s_second_counter <= 1) {
		s_second_counter += s_delta_time;
		m_tempFps++;
	}
	else
	{
		s_fps = m_tempFps;
		s_second_counter = 0;
		m_tempFps = 0;
	}

	s_ticks++;

	s_time_passed += s_delta_time;	
}

void spe::Time::reset()
{
	spe::Time::s_fps = 0;
	spe::Time::s_delta_time = 0;
	spe::Time::s_second_counter = 0;
	spe::Time::m_tempFps = 0;
}

float spe::Time::s_delta_time = 0;
sf::Clock spe::Time::s_delta_clock;

float spe::Time::s_second_counter = 0;
float spe::Time::m_tempFps = 0;

float spe::Time::s_fps = 0;
float spe::Time::s_ticks = 0;
float spe::Time::s_time_passed = 0;

