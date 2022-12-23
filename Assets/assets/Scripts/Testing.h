#pragma once

#include <gameData.h>
#include <sprite.h>
#include <input.h>
#include <sTime.h>

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

