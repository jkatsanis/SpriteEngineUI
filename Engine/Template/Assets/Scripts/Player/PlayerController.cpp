#include "PlayerController.h"
#include "Scripts/Game.h"

void PlayerController::Start(spe::EngineConfig& game)
{
	this->m_walking = false;
	this->m_grounded = false;
	this->m_ptr_player = game.ptr_Sprites->GetByName("Player");
	this->m_scale = this->m_ptr_player->transform.getScale();
	this->m_ptr_player->animator.play("idle");

	this->m_ptr_camera = game.ptr_Camera;
	/////////////////////
	// PLAYER INITIALIZED
	/////////////////////

	this->m_left_default_box_size = this->m_ptr_player->collider.box_collider_width.x;

	this->m_down_attacking = true;

	this->m_slide.start(this->m_ptr_player);
	this->m_wall_jump.start(this->m_ptr_player);

	this->m_ptr_camera->setZoom(0.7f);
}

void PlayerController::Update()
{
	this->animationControll();
	this->leftRight();
	this->jump();

	// this->m_slide.update(this->m_walking);
	// this->m_wall_jump.update(this->m_grounded);
	

	spe::Vector2 pos = spe::Vector2(this->m_ptr_player->transform.GetPosition().x, this->m_ptr_player->transform.GetPosition().y + 170);
	pos.y *= -1;
	this->m_ptr_camera->Position = pos;
}

void PlayerController::animationControll()
{
	// Checking when we first press the key so we play the run and stop the idle animation
	if (spe::Input::onKeyPress(spe::KeyBoardCode::A) || spe::Input::onKeyPress(spe::KeyBoardCode::D)
		&& !this->m_slide.isSliding() && !this->m_walking)
	{
		this->m_walking = true;
		this->m_ptr_player->animator.stop("idle");
		this->m_ptr_player->animator.play("runv2");
	}
	else if ((spe::Input::onKeyRelease(spe::KeyBoardCode::A) || spe::Input::onKeyRelease(spe::KeyBoardCode::D))
		&& !spe::Input::onKeyHold(spe::KeyBoardCode::A) && !spe::Input::onKeyHold(spe::KeyBoardCode::D))
	{
		this->m_walking = false;
		this->m_ptr_player->animator.stop("runv2");
		this->m_ptr_player->animator.play("idle");
	}
}

void PlayerController::leftRight()
{
	if (spe::Input::onKeyHold(spe::KeyBoardCode::A))
	{
		if (this->m_slide.controlWalkCondition(spe::KeyBoardCode::A))
		{
			return;
		}

		this->m_ptr_player->transform.setScale(spe::Vector2(-this->m_scale.x, this->m_scale.y));

		this->m_ptr_player->collider.box_collider_width.x = 24;
		const spe::Vector2 pos = spe::Vector2(this->m_ptr_player->transform.GetPosition().x - PLAYER_SPEED * spe::Time::s_delta_time,
			this->m_ptr_player->transform.GetPosition().y);
		this->m_ptr_player->transform.SetPosition(pos);
	}
	if (spe::Input::onKeyHold(spe::KeyBoardCode::D))
	{
		if (this->m_slide.controlWalkCondition(spe::KeyBoardCode::D))
		{
			return;
		}

		this->m_ptr_player->transform.setScale(spe::Vector2(this->m_scale.x, this->m_scale.y));

		this->m_ptr_player->collider.box_collider_width.x = this->m_left_default_box_size;
		const spe::Vector2 pos = spe::Vector2(this->m_ptr_player->transform.GetPosition().x + PLAYER_SPEED * spe::Time::s_delta_time,
			this->m_ptr_player->transform.GetPosition().y);
		this->m_ptr_player->transform.SetPosition(pos);
	}
}

void PlayerController::jump()
{
	this->m_grounded = this->m_ptr_player->collider.CollidedWithTag("Floor") != nullptr;

	if (spe::Input::onKeyPress(spe::KeyBoardCode::Space) && this->m_grounded)
	{
		this->m_grounded = false;
		this->m_ptr_player->physicsBody.velocity.y = 0;
		spe::Physics::AddForce(this->m_ptr_player, spe::Vector2(0, 1), 750);
	}
}
void PlayerController::downAttack()
{
	if (spe::Input::onKeyPress(spe::KeyBoardCode::S))
	{
		this->m_down_attacking = true;
		this->m_ptr_player->physicsBody.velocity.y = DOWN_ATTACK_SPEED;
	}
	if (this->m_grounded)
	{
		this->m_down_attacking = false;
	}
}