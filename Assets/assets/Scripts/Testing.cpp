#include "Testing.h"

//User classes

void Testing::start()
{
	this->m_spriteSpeed = 700;

	//Crash because we tried to acces a nullptr
	thisSprite = s2d::Sprite::getSpriteByName("Rick Astley");
	camera = &s2d::GameObject::camera;

	//User classes
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

	//Only 1 static camera
	///camera->setPosition(thisSprite->transform.position);
	//Almost like in unity 
}
 