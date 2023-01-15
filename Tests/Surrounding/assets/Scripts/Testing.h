#pragma once

#include <data/gameData.h>
#include <physicalComponents/sprite.h>
#include <engineComponents/input.h>
#include <engineComponents/sTime.h>
#include <calculationComponents/physics.h>
#include <random>
#include <imgui.h>

class Testing : s2d::GameData
{
private:

	s2d::Camera* camera;
	s2d::Sprite* thisSprite;
	s2d::Sprite* up;
	s2d::Sprite* down;

	int m_score;
	float m_spriteSpeed;
	float m_timeToPressAgain;
public:

	void start();

	void update();
};

