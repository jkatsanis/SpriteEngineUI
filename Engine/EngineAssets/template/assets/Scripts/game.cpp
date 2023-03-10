#include "game.h"

void Game::start()
{
	this->m_ptr_ship = s2d::Sprite::getSpriteByName("ship");
}

void Game::update()
{
	if (s2d::Input::onKeyPress(s2d::KeyBoardCode::A))
	{
		//std::cout << this->m_ptr_ship->animator.exists
		this->m_ptr_ship->animator.play("Cof");
	}

	if (s2d::Input::onKeyPress(s2d::KeyBoardCode::B))
	{

		this->m_ptr_ship->animator.stop("Cof");

	}
}
