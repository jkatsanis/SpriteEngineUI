#include "game.h"

void Game::start()
{
	this->m_ptr_ship = this->config.ptr_sprites->getSpriteWithName("ship");
}

void Game::update()
{
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::A))
	{
		this->m_ptr_ship->transform.position.x -= 800 * s2d::Time::s_delta_time;
	}

	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::D))
	{
		this->m_ptr_ship->transform.position.x += 800 * s2d::Time::s_delta_time;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::S))
	{
		this->m_ptr_ship->transform.position.y -= 800 * s2d::Time::s_delta_time;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::W))
	{
		this->m_ptr_ship->transform.position.y += 800 * s2d::Time::s_delta_time;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::G))
	{
		this->m_ptr_ship->transform.setRotation(this->m_ptr_ship->transform.getRotation() + 10);
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::V))
	{
		this->m_ptr_ship2->transform.position.x += 200 * s2d::Time::s_delta_time;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::H))
	{
		this->m_ptr_ship->transform.setRotation(0);
	}

	if (s2d::Input::onKeyPress(s2d::KeyBoardCode::O))
	{
		this->m_ptr_ship2 = s2d::PrefabRepositor::getPrefabByName("Hello");
		this->config.ptr_sprites->add(this->m_ptr_ship2);
	}
}
