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
	this->m_scale = scale;
	this->textureSize *= scale;
	this->m_attachedSprite->getSprite().setScale(sf::Vector2f(this->m_scale.x, this->m_scale.y));
	s2d::Vector2 size = s2d::Vector2(this->m_attachedSprite->getTexture().getSize().x, this->m_attachedSprite->getTexture().getSize().y);
	this->textureSize = size * this->getScale();
}

//Public static functions

void s2d::Transform::onPositionChange(s2d::Sprite* sprite)
{
	s2d::BoxCollider::checkCollisions();

	ChildSystem::resetPositionWhenChildIsColliding(sprite);
	ChildSystem::updatePositionToParent(sprite);
	ChildSystem::updateChildPositionRecursivly(sprite);
}
