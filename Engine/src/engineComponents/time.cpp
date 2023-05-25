#include <engineComponents\stime.h>
#include <iostream>

void s2d::Time::update()
{
	sf::Time dt = s_delta_clock.restart();
	s2d::Time::s_delta_time = dt.asSeconds();

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

void s2d::Time::reset()
{
	s2d::Time::s_fps = 0;
	s2d::Time::s_delta_time = 0;
	s2d::Time::s_second_counter = 0;
	s2d::Time::m_tempFps = 0;
}

float s2d::Time::s_delta_time = 0;
sf::Clock s2d::Time::s_delta_clock;

float s2d::Time::s_second_counter = 0;
float s2d::Time::m_tempFps = 0;

float s2d::Time::s_fps = 0;
float s2d::Time::s_ticks = 0;
float s2d::Time::s_time_passed = 0;

