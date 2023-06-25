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
	this->m_position = transform.m_position;
	this->m_rotation = transform.m_rotation;
	this->m_scale = transform.m_scale;
	this->texture_size = transform.texture_size;
	this->position_to_parent = transform.position_to_parent;

	this->setScale(this->m_scale, true);
	this->setOrigin();
	this->setTextureSize(this->m_scale);
}

void s2d::Transform::init()
{
	this->exist = true;
	this->reset();
}


// Public functions

void s2d::Transform::setPosition(const s2d::Vector2& positon)
{
	if (!this->validatePositionInput(positon))
	{
		return;
	}
	if (this->m_attached_sprite != nullptr && this->m_attached_sprite->parent != nullptr)
	{
		s2d::Vector2 distance = this->m_attached_sprite->parent->transform.getPosition() - positon;
		if (distance != this->position_to_parent)
		{
			this->position_to_parent = distance;
		}
	}
	this->m_position = positon;
}

void s2d::Transform::reset()
{
	this->exist = true;
	this->m_scale = s2d::Vector2(1.0f, 1.0f);
	this->m_position = s2d::Vector2(0.0f, 0.0f);
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

void s2d::Transform::updateTransformPosition()
{
	float x = 960 + this->m_position.x;
	float y = 540 - this->m_position.y;

	this->m_attached_sprite->getSprite().setPosition(sf::Vector2f(x, y));
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

bool s2d::Transform::validatePositionInput(const s2d::Vector2& position)
{
	if (this->m_attached_sprite == nullptr || !this->m_attached_sprite->collider.exist)
	{
		return true;
	}

	// Down	
	if (this->m_attached_sprite->collider.down)
	{
		if (position.y < this->m_position.y
			&& this->m_attached_sprite->physicsBody.velocity.y <= 0)
		{
			return false;
		}
	}

	// Up	
	if (this->m_attached_sprite->collider.up)
	{
		if (position.y > this->m_position.y
			&& this->m_attached_sprite->physicsBody.velocity.y >= 0)
		{
			return false;
		}
	}

	// Left	
	if (this->m_attached_sprite->collider.left)
	{
		if (position.x < this->m_position.x
			&& this->m_attached_sprite->physicsBody.velocity.x <= 0)
		{
			return false;
		}
	}

	// Right	
	if (this->m_attached_sprite->collider.right)
	{
		if (position.x > this->m_position.x
			&& this->m_attached_sprite->physicsBody.velocity.x >= 0)
		{
			return false;
		}
	}

	//this->m_attached_sprite->collider.resetPositions();
	
	return true;
}

//Public static functions

void s2d::Transform::onPositionChange(s2d::Sprite* sprite)
{
	ChildSystem::updateChildPositionRecursivly(sprite);
	ChildSystem::resetPositionWhenChildIsColliding(sprite);
}
