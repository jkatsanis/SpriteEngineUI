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

// Public functions

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
	//Pushing the sprites from a collider if 1 exists && we collided && IF everyting is unknown (no sprite collidng) then why a check
	if (!this->m_attachedSprite->collider.positionData.isEverythingUnknown())
		this->pushSetup();

	float x = 960 + this->position.x - this->textureSize.x / 2;
	float y = 540 - this->position.y - this->textureSize.y / 2;

	this->m_attachedSprite->getSprite().setPosition(sf::Vector2f(x, y));

	this->m_attachedSprite->collider.positionData.resetPosition();
	this->m_attachedSprite->collider.collisionCnt = 0;

	this->setLastPosition();
}

void s2d::Transform::pushSetup()
{
	this->pushSpriteFromCollider(s2d::BoxColliderPositionData::Right, false, this->position.x, this->nextPos.x, this->lastPos.x);
	this->pushSpriteFromCollider(s2d::BoxColliderPositionData::Left, true, this->position.x, this->nextPos.x, this->lastPos.x);
	this->pushSpriteFromCollider(s2d::BoxColliderPositionData::Down, true, this->position.y, this->nextPos.y, this->lastPos.y);
	this->pushSpriteFromCollider(s2d::BoxColliderPositionData::Up, false, this->position.y, this->nextPos.y, this->lastPos.y);
}

void s2d::Transform::pushSpriteFromCollider(s2d::BoxColliderPositionData::Position p, bool smaller, float& tXY, float& lXY, float& nXY)
{
	if (!smaller)
	{
		if (this->m_attachedSprite->collider.positionData.isEqual(p))
		{
			//We cant go into the gameobject when its right from us so we cant increment our x pos
			if (tXY > lXY)
			{
				//Swap lol
				tXY = lXY;
				lXY = nXY;
				nXY = tXY;
			}
		}
		return;
	}
	if (this->m_attachedSprite->collider.positionData.isEqual(p))
	{
		if (tXY < lXY)
		{
			tXY = lXY;
			lXY = nXY;
			nXY = tXY;
		}
	}
}
void s2d::Transform::setScale(const s2d::Vector2& scale, bool b)
{
	if (this->m_scale == scale && !b)
	{
		return;
	}

	this->setTextureSize(scale);
	sf::IntRect textureRect = this->m_attachedSprite->getSprite().getTextureRect();

	this->m_scale = scale;
	this->m_attachedSprite->getSprite().setOrigin(100, 71);
	this->m_attachedSprite->getSprite().setScale(scale.x, scale.y);

	if (scale.x < 0 && scale.y < 0)
	{
		this->m_attachedSprite->getSprite().setOrigin((float)textureRect.width, (float)textureRect.height);
	}
	else if (scale.x < 0)
	{
		this->m_attachedSprite->getSprite().setOrigin((float)textureRect.width, 0);
	}
	else if (scale.y < 0)
	{
		this->m_attachedSprite->getSprite().setOrigin(0, (float)textureRect.height);
	}
	else
	{
		this->m_attachedSprite->getSprite().setOrigin(0, 0);
	}
}

// Private functions

void s2d::Transform::setTextureSize(const s2d::Vector2& scale)
{
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
}


//Public static functions

void s2d::Transform::onPositionChange(s2d::Sprite* sprite)
{
	ChildSystem::resetPositionWhenChildIsColliding(sprite);
	ChildSystem::updatePositionToParent(sprite);
	ChildSystem::updateChildPositionRecursivly(sprite);
}
