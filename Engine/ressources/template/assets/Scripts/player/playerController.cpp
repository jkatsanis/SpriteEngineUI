#include "playerController.h"
#include <game.h>

void PlayerController::start(Game& game)
{
	this->m_walking = false;
	this->m_grounded = false;
	this->m_game = &game;
	this->m_ptr_player = game.config.ptr_sprites->getSpriteWithName("Player");
	this->m_scale = this->m_ptr_player->transform.getScale();
	this->m_ptr_player->animator.play("idle");
	
	/////////////////////
	// PLAYER INITIALIZED
	/////////////////////

	this->m_left_default_box_size = this->m_ptr_player->collider.box_collider_width.x;

	this->m_time_slided = 0.0f;
	this->m_wall_velocity = 0.0f;
	this->m_down_attacking = true;
}

void PlayerController::update()
{
	this->animationControll();
	this->leftRight();
	this->jump();
	this->slide();
	this->wallJump();
	this->downAttack();

	s2d::Vector2 pos = s2d::Vector2(this->m_ptr_player->transform.getPosition().x, this->m_ptr_player->transform.getPosition().y + 170);
	s2d::GameObject::camera.transform.setPosition(pos);
}

void PlayerController::animationControll()
{
	if (s2d::Input::onKeyPress(s2d::KeyBoardCode::A) || s2d::Input::onKeyPress(s2d::KeyBoardCode::D))
	{
		this->m_walking = true;
		this->m_ptr_player->animator.stop("idle");
		this->m_ptr_player->animator.play("runv2");
	}
	else if ((s2d::Input::onKeyRelease(s2d::KeyBoardCode::A) || s2d::Input::onKeyRelease(s2d::KeyBoardCode::D))
		&& !s2d::Input::onKeyHold(s2d::KeyBoardCode::A) && !s2d::Input::onKeyHold(s2d::KeyBoardCode::D))
	{
		this->m_walking = false;
		this->m_ptr_player->animator.stop("runv2");
		this->m_ptr_player->animator.play("idle");
	}
}

void PlayerController::leftRight()
{
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::A))
	{
		this->m_ptr_player->collider.box_collider_width.x = 24;

		this->m_ptr_player->transform.setScale(s2d::Vector2(-this->m_scale.x, this->m_scale.y));
		const s2d::Vector2 pos = s2d::Vector2(this->m_ptr_player->transform.getPosition().x - PLAYER_SPEED * s2d::Time::s_delta_time,
			this->m_ptr_player->transform.getPosition().y);


		this->m_ptr_player->transform.setPosition(pos);
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::D))
	{
		this->m_ptr_player->collider.box_collider_width.x = this->m_left_default_box_size;

		this->m_ptr_player->transform.setScale(s2d::Vector2(this->m_scale.x, this->m_scale.y));
		const s2d::Vector2 pos = s2d::Vector2(this->m_ptr_player->transform.getPosition().x + PLAYER_SPEED * s2d::Time::s_delta_time,
			this->m_ptr_player->transform.getPosition().y);

		this->m_ptr_player->transform.setPosition(pos);	
	}
}

void PlayerController::jump()
{
	this->m_grounded = this->m_ptr_player->collider.collidedWithTag("Floor") != nullptr;

	if (s2d::Input::onKeyPress(s2d::KeyBoardCode::Space) && this->m_grounded)
	{
		this->m_grounded = false;
		this->m_ptr_player->physicsBody.velocity.y = 0;
		s2d::Physics::addForce(this->m_ptr_player, s2d::Vector2(0, 1), 1000.0f);
	}
}

void PlayerController::slide()
{
	if (s2d::Input::onKeyPress(s2d::KeyBoardCode::LShift) && this->m_walking)
	{
		this->m_ptr_player->physicsBody.velocity.x = 0;
		const float direction = (this->m_ptr_player->transform.getScale().x < 0.0f) ? -1.0f : 1.0f;
		s2d::Physics::addForce(this->m_ptr_player, s2d::Vector2(direction, 0), 1000.0f);
		this->m_sliding = true;

		this->m_ptr_player->animator.play("dash");
	}
	if (this->m_sliding)
	{
		this->m_ptr_player->physicsBody.velocity.y = 0.0f;
		this->m_time_slided += s2d::Time::s_delta_time;
		if (this->m_time_slided >= SLIDE_TIME)
		{
			this->m_ptr_player->animator.play("idle");
			this->m_ptr_player->physicsBody.velocity.x = 0.0f;
			this->m_sliding = false;
			this->m_time_slided = 0.0f;
		}
	}
}

void PlayerController::wallJump()
{
	if (this->m_ptr_player->collider.collidedWithTag("wall") != nullptr && s2d::Input::onKeyHold(s2d::KeyBoardCode::A))
	{
		this->m_ptr_player->physicsBody.velocity.y = this->m_wall_velocity;
		this->m_on_wall = true;

	}

	
	if (!this->m_on_wall)
	{
		return;
	}

	this->m_wall_timer += s2d::Time::s_delta_time;

	if (this->m_wall_timer >= WALL_SLIDE_TIME)
	{
		this->m_wall_velocity--;
	}

	if (this->m_grounded)
	{
		this->m_wall_velocity = 0;
		this->m_wall_timer = 0;
		this->m_on_wall = false;
	}
}

void PlayerController::downAttack()
{
	if (s2d::Input::onKeyPress(s2d::KeyBoardCode::S))
	{
		this->m_down_attacking = true;
		this->m_ptr_player->physicsBody.velocity.y = DOWN_ATTACK_SPEED;
	}
	if (this->m_grounded)
	{
		this->m_down_attacking = false;
	}
}

