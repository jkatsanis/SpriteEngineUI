#include "Slide.h"

// Private

void Slide::animationControl(const bool& is_walking)
{
	if (this->slideCondition(is_walking))
	{
		this->m_ptr_player->animator.stop("runv2");
		this->m_ptr_player->animator.play("dash");
	}
}

bool Slide::slideCondition(const bool& is_walking)
{
	bool a = spe::Input::onKeyHold(spe::KeyBoardCode::A) && !spe::Input::onKeyHold(spe::KeyBoardCode::D);
	bool b = !spe::Input::onKeyHold(spe::KeyBoardCode::A) && spe::Input::onKeyHold(spe::KeyBoardCode::D);

	bool ad = spe::Input::onKeyHold(spe::KeyBoardCode::A);
	bool da = spe::Input::onKeyHold(spe::KeyBoardCode::D);

	return (spe::Input::onKeyPress(spe::KeyBoardCode::LShift) && is_walking
		&& (a || b));
}

bool Slide::controlWalkCondition(spe::KeyBoardCode direction)
{
	DashKey right = (direction == spe::KeyBoardCode::A) ? DashKey::D : DashKey::A;

	DashKey left = (direction == spe::KeyBoardCode::A) ? DashKey::A : DashKey::D;

	if (this->m_dash_key == left)
	{
		return true;
	}
	if (this->m_dash_key == right)
	{
		this->stop();
	}
	return false;
}


// Public 

void Slide::start(spe::Sprite* player)
{
	this->m_ptr_player = player;

	this->m_slide_time = this->m_ptr_player->animator.animations["dash"].GetAnimationTime() / 100;

	this->m_time_slided = 0.0f;
	this->m_sliding = false;
}

void Slide::update(const bool& is_walking)
{
	this->animationControl(is_walking);

	bool ad = spe::Input::onKeyHold(spe::KeyBoardCode::A);

	if (this->slideCondition(is_walking))
	{
		this->m_ptr_player->physicsBody.velocity.x = 0;
		const float direction = (this->m_ptr_player->transform.getScale().x < 0.0f) ? -1.0f : 1.0f;
		spe::Physics::AddForce(this->m_ptr_player, spe::Vector2(direction, 0), SLIDE_FORCE);
		this->m_sliding = true;

		this->m_dash_key = (ad) ? DashKey::A : DashKey::D;

	}
	if (this->m_sliding)
	{
		this->m_ptr_player->physicsBody.velocity.y = 0;
		this->m_time_slided += spe::Time::s_delta_time;
		if (this->m_time_slided >= this->m_slide_time)
		{
			this->stop();
		}
	}
}

void Slide::stop()
{
	this->m_ptr_player->physicsBody.velocity.x = 0.0f;
	this->m_sliding = false;
	this->m_time_slided = 0.0f;

	this->m_ptr_player->animator.stop("dash");

	if (spe::Input::onKeyHold(spe::KeyBoardCode::A) || spe::Input::onKeyHold(spe::KeyBoardCode::D))
	{
		this->m_ptr_player->animator.play("runv2");
	}
	else
	{
		this->m_ptr_player->animator.play("idle");
	}


	// Walkk true
	this->m_dash_key = DashKey::None;
}