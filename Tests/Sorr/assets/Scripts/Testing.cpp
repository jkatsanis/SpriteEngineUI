#include "Testing.h"

//User classes

void Testing::start()
{
	this->m_spriteSpeed = 800;
	this->m_score = 0;
	thisSprite = s2d::Sprite::getSpriteByName("Rick");
	up = s2d::Sprite::getSpriteByName("up");
	down = s2d::Sprite::getSpriteByName("down");
	camera = &s2d::GameObject::camera;
}

void Testing::update()
{
	
	if (s2d::Input::onKeyPress(s2d::KeyBoardCode::Space))
	{
		thisSprite->physicsBody.velocity.y = 0;
		s2d::Physics::addForce(thisSprite, s2d::Vector2(0, 1), 0.2f);
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::A))
	{
		thisSprite->transform.position.x -= this->m_spriteSpeed * s2d::Time::deltaTime;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::W))
	{
		thisSprite->transform.position.y += this->m_spriteSpeed * s2d::Time::deltaTime;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::S))
	{
		thisSprite->transform.position.y -= this->m_spriteSpeed * s2d::Time::deltaTime;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::D))
	{
		thisSprite->transform.position.x += this->m_spriteSpeed * s2d::Time::deltaTime;
	}


}
 