#pragma once

#include <_header/SpriteEngine.h>

#define PLAYER_SPEED 800
#define SLIDE_TIME 0.3
#define WALL_SLIDE_TIME 0.5f
#define DOWN_ATTACK_SPEED -1000

class Game;
class PlayerController : public s2d::Base
{
private:
	Game* m_game;
	s2d::Sprite* m_ptr_player;
	s2d::Vector2 m_scale;
	bool m_grounded;
	bool m_walking;
	bool m_sliding;
	float m_time_slided;
	float m_left_default_box_size;

	float m_wall_velocity;
	float m_wall_timer;
	bool m_on_wall;


	bool m_down_attacking;

	void animationControll();
	void leftRight();
	void jump();
	void slide();
	void wallJump();
	void downAttack();

public:
	void start(Game& game);

	void update() override;
};

