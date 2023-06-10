#include "game.h"

void Game::start()
{
	this->m_ptr_ship = this->config.ptr_sprites->getSpriteWithName("ship");
}

void Game::update()
{
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::A))
	{
		this->m_ptr_ship->transform.position.x -= 800 * s2d::Time::deltaTime;
	}

	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::D))
	{
		this->m_ptr_ship->transform.position.x += 800 * s2d::Time::deltaTime;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::S))
	{
		this->m_ptr_ship->transform.position.y -= 800 * s2d::Time::deltaTime;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::W))
	{
		this->m_ptr_ship->transform.position.y += 800 * s2d::Time::deltaTime;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::G))
	{
		this->m_ptr_ship->animator.play("Im");
	}

	s2d::GameObject::camera.transform.position = this->m_ptr_ship->transform.position;
}
