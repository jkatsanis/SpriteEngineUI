#pragma once

#include <_header/SpriteEngine.h>

class Game : public s2d::Base
{
private:
	s2d::Sprite* m_ptr_ship;
public:
	void start() override;
	void update() override;
};

