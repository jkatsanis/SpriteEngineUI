#pragma once

#include <_header/SpriteEngine.h>

#define PLAYER_SPEED 800

class Game : public s2d::Base
{
private:
	s2d::Sprite* m_ptr_player;
public:
	s2d::EngineConfig config;

	void start() override;
	void update() override;
};

