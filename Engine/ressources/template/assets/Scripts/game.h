#pragma once

#include <_header/SpriteEngine.h>

class Game : public s2d::Base
{
private:
	s2d::Sprite* m_ptr_ship;
	s2d::Sprite* m_ptr_ship2;
public:
	s2d::EngineConfig config;

	void start() override;
	void update() override;
};

