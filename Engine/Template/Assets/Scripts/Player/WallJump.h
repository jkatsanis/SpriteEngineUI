#pragma once

#include "Source/SpriteEngine.h"

#define WALL_SLIDE_TIME 0.5f

class WallJump : public spe::IScript
{
private:
	spe::Sprite* m_ptr_player;
	float m_wall_velocity;
	float m_wall_timer;
	bool m_on_wall;

public:
	void start(spe::Sprite* player);

	void update(const bool& is_grounded);
};
