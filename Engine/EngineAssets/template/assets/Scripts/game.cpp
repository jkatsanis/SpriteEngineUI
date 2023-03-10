#include "game.h"

void Game::start()
{
	this->m_ptr_ship = s2d::Sprite::getSpriteByName("ship");
}

void Game::update()
{
	if (s2d::Input::onKeyPress(s2d::KeyBoardCode::A))
	{
		this->m_ptr_ship->animator.play("Doe");
	}

	if (s2d::Input::onKeyPress(s2d::KeyBoardCode::B))
	{

		this->m_ptr_ship->animator.stop("Doe");

	}
}
