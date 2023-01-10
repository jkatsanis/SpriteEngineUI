#pragma once

#include <data/gameData.h>
#include <physicalComponents/sprite.h>
#include <engineComponents/input.h>
#include <engineComponents/sTime.h>

class Testing : s2d::GameData
{
private:

	s2d::Camera* camera;
	s2d::Sprite* thisSprite;

	float m_spriteSpeed;
public:

	void start();

	void update();
};