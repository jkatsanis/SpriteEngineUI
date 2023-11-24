#include "Transform.h"
#include "Sprite.h"

//Contructor

spe::Transform::Transform()
{
	this->init();
}

spe::Transform::Transform(spe::Sprite* attached)
{
	this->init();
	this->m_attached_sprite = attached;
}

spe::Transform::Transform(spe::Sprite* attachedSprite, spe::Transform& transform)
{
	this->init();
	this->m_attached_sprite = attachedSprite;
	this->m_Position = transform.m_Position;
	this->m_rotation = transform.m_rotation;
	this->m_scale = transform.m_scale;
	this->texture_size = transform.texture_size;
	this->position_to_parent = transform.position_to_parent;

	this->setScale(this->m_scale, true);
	this->setOrigin();
	this->setTextureSize(this->m_scale);
}

void spe::Transform::init()
{
	this->base_component = true;
	this->m_attached_sprite = nullptr;
	this->m_rotation = 0;
}

// Private functions

void spe::Transform::UpdateSpritePositionToParent(const spe::Vector2& position)
{
	if (this->m_attached_sprite != nullptr && this->m_attached_sprite->parent != nullptr)
	{
	    const spe::Vector2 distance = this->m_attached_sprite->parent->transform.GetPosition() - position;
		if (distance != this->position_to_parent)
		{
			this->position_to_parent = distance;
		}
	}
}

// Public functions

void spe::Transform::SetPosition(const spe::Vector2& position)
{
	if (this->m_Position == position)
	{
		this->position_changed = false;
	}
	this->UpdateSpritePositionToParent(position);
	this->m_Position = position;
	this->position_changed = true;

	if (this->m_attached_sprite != nullptr)
	{
		this->m_attached_sprite->getSprite().setPosition(sf::Vector2f(position.x + 960, 540 - position.y));
	}
}

void spe::Transform::setTextureSize(const spe::Vector2& scale)
{
	spe::Vector2 multiply = scale;

	if (multiply.x < 0)
	{
		multiply.x = multiply.x * -1;
	}
	if (multiply.y < 0)
	{
		multiply.y = multiply.y * -1;
	}

	sf::IntRect textureRect = this->m_attached_sprite->getSprite().getTextureRect();
	this->texture_size = spe::Vector2(textureRect.width * multiply.x, textureRect.height * multiply.y);
	this->setOrigin();
}

void spe::Transform::calculateScaleXByWorldPosition(const float posX)
{
	float scaleX = posX / this->texture_size.x;

	this->setScale(spe::Vector2(scaleX, this->m_scale.y));
}

spe::Vector2 spe::Transform::getDefaultTextureSize() const
{
	spe::Vector2 scale = this->m_scale;

	if (this->m_scale.x < 0)
	{
		scale.x = scale.x * -1;
	}
	if (this->m_scale.y < 0)
	{
		scale.y = scale.y * -1;
	}
	return spe::Vector2(this->texture_size.x / scale.x, this->texture_size.y / scale.y);
}

void spe::Transform::setRotation(uint32_t angle)
{
	this->m_rotation = angle % 360;
	this->m_attached_sprite->getSprite().setRotation((float)this->m_rotation);
}

void spe::Transform::setOrigin()
{
	sf::Sprite& spr = this->m_attached_sprite->getSprite();
	spr.setOrigin(sf::Vector2f(this->getDefaultTextureSize().x / 2, this->getDefaultTextureSize().y / 2));
}

void spe::Transform::setScale(const spe::Vector2& scale, bool b)
{
	if (this->m_scale == scale && !b)
	{
		return;
	}

	this->setTextureSize(scale);

	this->m_scale = scale;
	this->m_attached_sprite->getSprite().setScale(scale.x, scale.y);
	this->setOrigin();
}

void spe::Transform::reset()
{
	this->m_Position = spe::Vector2(0, 0);
	this->setScale(spe::Vector2(1, 1));
}

