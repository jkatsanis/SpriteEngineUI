#pragma once

#include <_header/SpriteEngine.h>

class Game : public s2d::Base
{
private:
	s2d::Sprite* m_ptr_ship;
public:
	s2d::EngineConfig config;

	void start() override;
	void update() override;
};

