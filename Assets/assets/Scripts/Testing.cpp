#include "Testing.h"

//User classes

void Testing::start()
{
	this->m_spriteSpeed = 200;

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

	camera->transform.position = thisSprite->transform.position;
}
 