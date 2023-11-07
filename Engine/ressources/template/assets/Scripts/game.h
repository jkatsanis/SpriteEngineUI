#pragma once

#include <_header/SpriteEngine.h>

#include <player/playerController.h>

class Game : public s2d::Base
{
private:
	PlayerController m_controller;
public:
	s2d::EngineConfig config;

	void start() override;
	void update() override;
};

