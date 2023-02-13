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
	this->m_scale = s2d::Vector2(1.0f, 1.0f);
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

void s2d::Transform::setScale(const s2d::Vector2& scale)
{
	if (this->m_scale == scale)
	{
		return;
	}

	s2d::Vector2 multiply = scale;

	if (multiply.x < 0)
	{
		multiply.x = multiply.x * -1;
	}
	if (multiply.y < 0)
	{
		multiply.y = multiply.y * -1;
	}

	sf::IntRect textureRect = this->m_attachedSprite->getSprite().getTextureRect();
	this->textureSize = s2d::Vector2(textureRect.width * multiply.x, textureRect.height * multiply.y);

	this->m_scale = scale;
	this->m_attachedSprite->getSprite().setOrigin(100, 71);
	this->m_attachedSprite->getSprite().setScale(scale.x, scale.y);

	if (scale.x < 0 && scale.y < 0)
	{
		this->m_attachedSprite->getSprite().setOrigin(textureRect.width, textureRect.height);
	}
	else if (scale.x < 0)
	{
		this->m_attachedSprite->getSprite().setOrigin(textureRect.width, 0);
	}
	else if (scale.y < 0)
	{
		this->m_attachedSprite->getSprite().setOrigin(0, textureRect.height);
	}
	else
	{
		this->m_attachedSprite->getSprite().setOrigin(0, 0);
	}
}

//Public static functions

void s2d::Transform::onPositionChange(s2d::Sprite* sprite)
{
	ChildSystem::resetPositionWhenChildIsColliding(sprite);
	ChildSystem::updatePositionToParent(sprite);
	ChildSystem::updateChildPositionRecursivly(sprite);
}