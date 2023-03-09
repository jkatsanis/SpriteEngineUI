#include "game.h"

void Game::start()
{
	this->m_ptr_ship = s2d::Sprite::getSpriteByName("ship");
	this->m_ptr_ship->animator.exists = true;
	
	std::vector<s2d::KeyFrame> frames =
	{
		s2d::KeyFrame("assets\\laser.png", 0),
		s2d::KeyFrame("assets\\asteroid.png", 50),
		s2d::KeyFrame("assets\\asteroid.png", 100),
	};

	this->m_ptr_ship->animator.createAnimation("Hello", "assets", frames);
}

void Game::update()
{
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::A))
	{
		this->m_ptr_ship->transform.position.x -= 800 * s2d::Time::deltaTime;
	}

	if (s2d::Input::onKeyPress(s2d::KeyBoardCode::D))
	{
		this->m_ptr_ship->animator.play("Hello");
	}

	if (s2d::Input::onKeyPress(s2d::KeyBoardCode::F))
	{
		this->m_ptr_ship->animator.stop("Hello");
	}
}
