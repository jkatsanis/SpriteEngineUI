#pragma once

#include "Slide.h"
#include "WallJump.h"

#define PLAYER_SPEED 800
#define DOWN_ATTACK_SPEED -1000

class PlayerController : public spe::IScript
{
private:
	spe::Camera* m_ptr_camera;
	spe::Sprite* m_ptr_player;
	spe::Vector2 m_scale;
	bool m_grounded;
	bool m_walking;
	float m_left_default_box_size;

	Slide m_slide;
	WallJump m_wall_jump;

	bool m_down_attacking;

	void animationControll();
	void leftRight();
	void jump();
	void downAttack();


public:
	void Start(spe::EngineConfig& game);

	void Update() override;

	spe::Sprite* getPlayer() { return this->m_ptr_player; }
};
