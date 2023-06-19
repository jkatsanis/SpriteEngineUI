#include "stransform.h"
#include <physicalComponents/sprite.h>

//Contructor

s2d::Transform::Transform()
{
	this->init();
}

s2d::Transform::Transform(s2d::Sprite* attachedSprite)
{
	this->init();
	this->m_attached_sprite = attachedSprite;
}

s2d::Transform::Transform(s2d::Sprite* attachedSprite, s2d::Transform& transform)
{
	this->init();
	this->m_attached_sprite = attachedSprite;
	this->position = transform.position;
	this->m_rotation = transform.m_rotation;
	this->m_scale = transform.m_scale;
	this->texture_size = transform.texture_size;
}

void s2d::Transform::init()
{
	this->exist = true;
	this->reset();
}


// Public functions

void s2d::Transform::reset()
{
	this->exist = true;
	this->m_scale = s2d::Vector2(1.0f, 1.0f);
	this->position = s2d::Vector2(0.0f, 0.0f);
	this->position_changed = false;
	this->m_attached_sprite = nullptr;
	this->m_rotation = 0;
}

s2d::Vector2 s2d::Transform::getDefaultTextureSize() const
{
	s2d::Vector2 scale = this->m_scale;

	if (this->m_scale.x < 0)
	{
		scale.x = scale.x * -1;
	}
	if (this->m_scale.y < 0)
	{
		scale.y = scale.y * -1;
	}
	return s2d::Vector2(this->texture_size.x / scale.x, this->texture_size.y / scale.y);
}

void s2d::Transform::setRotation(uint32_t angle)
{
	this->m_rotation = angle % 360;
	this->m_attached_sprite->getSprite().setRotation((float)this->m_rotation);
}

void s2d::Transform::setLastPosition()
{
	if (this->next_pos != this->position)
	{
		this->last_pos = this->next_pos;
		this->next_pos = this->position;
		this->position_changed = true;
	}
	else
	{
		this->position_changed = false;
	}
}

void s2d::Transform::updateTransformPosition()
{
	//Pushing the sprites from a collider if 1 exists && we collided && IF everyting is unknown (no sprite collidng) then why a check
	if (!this->m_attached_sprite->collider.position_data.isEverythingUnknown())
		this->pushSetup();

	float x = 960 + this->position.x;
	float y = 540 - this->position.y;

	this->m_attached_sprite->getSprite().setPosition(sf::Vector2f(x, y));

	this->m_attached_sprite->collider.position_data.resetPosition();
	this->m_attached_sprite->collider.collision_cnt = 0;

	this->setLastPosition();
}

void s2d::Transform::setScale(const s2d::Vector2& scale, bool b)
{
	if (this->m_scale == scale && !b)
	{
		return;
	}

	this->setTextureSize(scale);
	sf::IntRect textureRect = this->m_attached_sprite->getSprite().getTextureRect();

	this->m_scale = scale;
	this->m_attached_sprite->getSprite().setScale(scale.x, scale.y);
}

void s2d::Transform::setOrigin()
{
	sf::Sprite& spr = this->m_attached_sprite->getSprite();
	spr.setOrigin(sf::Vector2f(this->getDefaultTextureSize().x / 2, this->getDefaultTextureSize().y / 2));
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

	sf::IntRect textureRect = this->m_attached_sprite->getSprite().getTextureRect();
	this->texture_size = s2d::Vector2(textureRect.width * multiply.x, textureRect.height * multiply.y);
}

void s2d::Transform::pushSetup()
{
	this->pushSpriteFromCollider(s2d::BoxColliderPositionData::Right, false, this->position.x, this->next_pos.x, this->last_pos.x);
	this->pushSpriteFromCollider(s2d::BoxColliderPositionData::Left, true, this->position.x, this->next_pos.x, this->last_pos.x);
	this->pushSpriteFromCollider(s2d::BoxColliderPositionData::Down, true, this->position.y, this->next_pos.y, this->last_pos.y);
	this->pushSpriteFromCollider(s2d::BoxColliderPositionData::Up, false, this->position.y, this->next_pos.y, this->last_pos.y);
}

void s2d::Transform::pushSpriteFromCollider(s2d::BoxColliderPositionData::Position p, bool smaller, float& tXY, float& lXY, float& nXY)
{
	if (!smaller)
	{
		if (this->m_attached_sprite->collider.position_data.isEqual(p))
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
	if (this->m_attached_sprite->collider.position_data.isEqual(p))
	{
		if (tXY < lXY)
		{
			tXY = lXY;
			lXY = nXY;
			nXY = tXY;
		}
	}
}

//Public static functions

void s2d::Transform::onPositionChange(s2d::Sprite* sprite)
{
	ChildSystem::resetPositionWhenChildIsColliding(sprite);
	ChildSystem::updatePositionToParent(sprite);
	ChildSystem::updateChildPositionRecursivly(sprite);
}
