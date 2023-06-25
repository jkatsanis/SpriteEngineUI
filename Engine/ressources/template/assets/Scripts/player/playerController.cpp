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
	this->m_time_slided = 0.0f;
}

void PlayerController::update()
{
	this->animationControll();
	this->leftRight();
	this->jump();
	this->slide();

	s2d::Vector2 pos =s2d::Vector2(this->m_ptr_player->transform.getPosition().x, this->m_ptr_player->transform.getPosition().y + 170);
	s2d::GameObject::camera.transform.setPosition(pos);
}

void PlayerController::animationControll()
{
	if (s2d::Input::onKeyPress(s2d::KeyBoardCode::A) || s2d::Input::onKeyPress(s2d::KeyBoardCode::D))
	{
		this->m_walking = true;
		this->m_ptr_player->animator.stop("idle");
		this->m_ptr_player->animator.play("run");
	}
	else if ((s2d::Input::onKeyRelease(s2d::KeyBoardCode::A) || s2d::Input::onKeyRelease(s2d::KeyBoardCode::D))
		&& !s2d::Input::onKeyHold(s2d::KeyBoardCode::A) && !s2d::Input::onKeyHold(s2d::KeyBoardCode::D))
	{
		this->m_walking = false;
		this->m_ptr_player->animator.stop("run");
		this->m_ptr_player->animator.play("idle");
	}
}

void PlayerController::leftRight()
{
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::A))
	{
		this->m_ptr_player->transform.setScale(s2d::Vector2(-this->m_scale.x, this->m_scale.y));
		const s2d::Vector2 pos = s2d::Vector2(this->m_ptr_player->transform.getPosition().x - PLAYER_SPEED * s2d::Time::s_delta_time,
			this->m_ptr_player->transform.getPosition().y);

		this->m_ptr_player->transform.setPosition(pos);
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::D))
	{
		this->m_ptr_player->transform.setScale(s2d::Vector2(this->m_scale.x, this->m_scale.y));
		const s2d::Vector2 pos = s2d::Vector2(this->m_ptr_player->transform.getPosition().x + PLAYER_SPEED * s2d::Time::s_delta_time,
			this->m_ptr_player->transform.getPosition().y);

		this->m_ptr_player->transform.setPosition(pos);	
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::W))
	{
		const s2d::Vector2 pos = s2d::Vector2(this->m_ptr_player->transform.getPosition().x,
			this->m_ptr_player->transform.getPosition().y + PLAYER_SPEED * s2d::Time::s_delta_time);
		this->m_ptr_player->transform.setPosition(pos);
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::S))
	{
		const s2d::Vector2 pos = s2d::Vector2(this->m_ptr_player->transform.getPosition().x,
			this->m_ptr_player->transform.getPosition().y - PLAYER_SPEED * s2d::Time::s_delta_time);
		this->m_ptr_player->transform.setPosition(pos);
	}
}

void PlayerController::jump()
{
	if (this->m_ptr_player->collider.colliding_sprite != nullptr)
	{
		if(this->m_ptr_player->collider.colliding_sprite->tag == "Floor")
		{
			this->m_grounded = true;
		}
	}
	else
	{
		this->m_grounded = false;
	}
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
		const float direction = (this->m_ptr_player->transform.getScale().x < 0.0f) ? -1.0f : 1.0f;
		s2d::Physics::addForce(this->m_ptr_player, s2d::Vector2(direction, 0), 1000.0f);
		this->m_sliding = true;
	}
	if (this->m_sliding)
	{
		this->m_time_slided += s2d::Time::s_delta_time;
		if (this->m_time_slided >= SLIDE_TIME)
		{
			this->m_ptr_player->physicsBody.velocity.x = 0.0f;
			this->m_sliding = false;
			this->m_time_slided = 0.0f;
		}
	}
}

