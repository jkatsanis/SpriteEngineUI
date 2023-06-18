#include <engineComponents/sTime.h>

void s2d::Time::update()
{
	sf::Time dt = deltaClock.restart();

	Time::s_delta_time = dt.asSeconds();	


	if (m_secondCounter <= 1) {
		m_secondCounter += s_delta_time;
		m_tempFps++;
	}
	else 
	{
		fps = m_tempFps;
		m_secondCounter = 0;
		m_tempFps = 0;
	}
	timePassed += s_delta_time;
	//std::cout << fps << std::endl;
}

float s2d::Time::s_delta_time = 0;
sf::Clock s2d::Time::deltaClock;

float s2d::Time::m_secondCounter = 0;
float s2d::Time::m_tempFps = 0;
float s2d::Time::timePassed = 0;

float s2d::Time::fps = 0;
