#pragma once

#include <_header/SpriteEngine.h>

class CameraController : s2d::Base
{
private:
	const s2d::Sprite* m_ptr_player;
	s2d::Camera* m_ptr_cam;
public:
	void start(const s2d::Sprite* ptr_player);
	void update() override;
};

