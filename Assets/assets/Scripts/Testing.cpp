#include "Testing.h"

//User classes

void Testing::start()
{
	this->m_spriteSpeed = 800;

	thisSprite = s2d::Sprite::getSpriteByName("Rick");
	camera = &s2d::GameObject::camera;
}

void Testing::update()
{
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::A))
	{
		thisSprite->transform.position.x -= this->m_spriteSpeed * s2d::Time::deltaTime;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::W))
	{
		thisSprite->transform.position.y += this->m_spriteSpeed * s2d::Time::deltaTime ;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::S))
	{
		thisSprite->transform.position.y -= this->m_spriteSpeed * s2d::Time::deltaTime;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::D))
	{
		thisSprite->transform.position.x += this->m_spriteSpeed * s2d::Time::deltaTime;
	}
	if (s2d::Input::onKeyPress(s2d::KeyBoardCode::P))
	{
		s2d::Sprite* spr = new s2d::Sprite("name", s2d::Vector2(0, 0), "assets/Sprites/big.png", true);
		std::cout << s2d::Sprite::activeSprites[0]->getId() << std::endl;
		std::cout << s2d::Sprite::activeSprites[1]->getId() << std::endl;
	}
	if (s2d::Input::onKeyPress(s2d::KeyBoardCode::O))
	{
		std::cout << s2d::Sprite::getSpriteByName("name")->getId() << std::endl;
	}
	camera->transform.position = thisSprite->transform.position;
}
 