#include "stransform.h"
#include <physicalComponents/sprite.h>

//Contructor

s2d::Transform::Transform()
{
	this->m_attachedSprite = nullptr;
	this->keepOpenInHirachy = false;
	this->position = s2d::Vector2(0.0f, 0.0f);
	this->posiitonChanged = false;	
}

s2d::Transform::Transform(s2d::Sprite* attachedSprite)
{
	this->scale = s2d::Vector2(1.0f, 1.0f);
	this->m_attachedSprite = nullptr;
	this->keepOpenInHirachy = false;
	this->position = s2d::Vector2(0.0f, 0.0f);
	this->posiitonChanged = false;		
	this->m_attachedSprite = attachedSprite;
}

void s2d::Transform::setLastPosition()
{
	if (this->nextPos != this->position)
	{
		this->lastPos = this->nextPos;
		this->nextPos = this->position;
		this->posiitonChanged = true;
	}
	else
	{
		this->posiitonChanged = false;
	}
}

void s2d::Transform::updateTransformPosition()
{
	//Setting it centered 
	float x = 960 + this->position.x - this->textureSize.x / 2;
	float y = 540 - this->position.y - this->textureSize.y / 2;
	this->m_attachedSprite->getSprite().setPosition(sf::Vector2f(x, y));

	this->setLastPosition();
}

//Public static functions

void s2d::Transform::onPositionChange(s2d::Sprite* sprite)
{
	ChildSystem::updatePositionToParent(sprite);
	ChildSystem::updateChildPositionRecursivly(sprite);
	ChildSystem::updateChildPositionToParentRecursivly(sprite);
}
