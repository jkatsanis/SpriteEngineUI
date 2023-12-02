#include "WallJump.h"

void WallJump::start(spe::Sprite* player)
{
	this->m_ptr_player = player;
}

void WallJump::update(const bool& is_grounded)
{
	if (this->m_ptr_player->collider.CollidedWithTag("wall") != nullptr
		&& spe::Input::onKeyHold(spe::KeyBoardCode::A))
	{
		this->m_ptr_player->physicsBody.velocity.y = this->m_wall_velocity;
		this->m_on_wall = true;

	}

	if (!this->m_on_wall)
	{
		return;
	}

	this->m_wall_timer += spe::Time::s_delta_time;

	if (this->m_wall_timer >= WALL_SLIDE_TIME)
	{
		this->m_wall_velocity--;
	}

	if (is_grounded)
	{
		this->m_wall_velocity = 0;
		this->m_wall_timer = 0;
		this->m_on_wall = false;
	}
}