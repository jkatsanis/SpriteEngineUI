#pragma once

#include <data/gameData.h>
#include <physicalComponents/sprite.h>
#include <engineComponents/input.h>
#include <engineComponents/sTime.h>
#include <calculationComponents/physics.h>
#include <random>
#include <imgui.h>
#include <engineComponents/base.h>

class Testing : public s2d::Base
{
private:
	s2d::Camera* camera;
	s2d::Sprite* thisSprite;
	s2d::Sprite* up;
	s2d::Sprite* down;

	bool m_collided;
	int m_score;
	float m_spriteSpeed;
	float m_timeToPressAgain;
};

