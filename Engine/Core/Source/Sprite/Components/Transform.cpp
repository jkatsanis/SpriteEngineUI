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
	this->m_scale = spe::Vector2(1, 1);
}

// Private functions

void spe::Transform::UpdateSpritePositionToParent(const spe::Vector2& position)
{
	if (this->m_attached_sprite == nullptr)
	{
		return;
	}
	for (spe::Sprite* spr : this->m_attached_sprite->ptr_childs)
	{
		spe::Vector2 distance = this->m_Position - position;

		distance.X *= -1;
		distance.Y *= -1;

		spe::Vector2 newpos = spr->transform.GetPosition() +distance;
		spr->transform.SetPosition(newpos);
	}

}

spe::Vector2 spe::Transform::HandleCollisions(const spe::Vector2& position)
{
	spe::Vector2 new_position(position);

	if (this->m_attached_sprite == nullptr || !this->m_attached_sprite->collider.exist || !this->m_attached_sprite->collider.collided)
	{
		// No collision check
		return new_position;
	}

	const float current_y = this->m_attached_sprite->transform.m_Position.Y;
	const float current_x = this->m_attached_sprite->transform.m_Position.X;

	// Down	
	if (this->m_attached_sprite->collider.down)
	{
		if (position.Y < this->m_Position.Y
			&& this->m_attached_sprite->physicsBody.velocity.Y <= 0)
		{
			new_position.Y = current_y;
		}
	}

	// Up	
	if (this->m_attached_sprite->collider.up)
	{
		if (position.Y > this->m_Position.Y
			&& this->m_attached_sprite->physicsBody.velocity.Y >= 0)
		{
			new_position.Y = current_y;
		}
	}

	// Left	
	if (this->m_attached_sprite->collider.left)
	{
		if (position.X < this->m_Position.X
			&& this->m_attached_sprite->physicsBody.velocity.X <= 0)
		{
			new_position.X = current_x;
		}
	}

	// Right	
	if (this->m_attached_sprite->collider.right)
	{
		if (position.X > this->m_Position.X
			&& this->m_attached_sprite->physicsBody.velocity.X >= 0)
		{
			new_position.X = current_x;
		}
	}
	return new_position;
	//this->m_attached_sprite->collider.resetPositions();
}

// Public functions

void spe::Transform::SetPosition(const spe::Vector2& position)
{
	if (this->m_Position == position)
	{
		this->position_changed = false;
		return;
	}

	const spe::Vector2 new_pos = this->HandleCollisions(position);

	this->UpdateSpritePositionToParent(new_pos);
	this->m_Position = new_pos;
	this->position_changed = true;

	if (this->m_attached_sprite != nullptr)
	{
		this->m_attached_sprite->getSprite().setPosition(sf::Vector2f(new_pos.X + 960, 540 - new_pos.Y));
	}
}

void spe::Transform::setTextureSize(const spe::Vector2& scale)
{
	spe::Vector2 multiply = scale;

	if (multiply.X < 0)
	{
		multiply.X = multiply.X * -1;
	}
	if (multiply.Y < 0)
	{
		multiply.Y = multiply.Y * -1;
	}

	sf::IntRect textureRect = this->m_attached_sprite->getSprite().getTextureRect();
	this->texture_size = spe::Vector2(textureRect.width * multiply.X, textureRect.height * multiply.Y);
	this->setOrigin();
}

void spe::Transform::calculateScaleXByWorldPosition(const float posX)
{
	float scaleX = posX / this->texture_size.X;

	this->setScale(spe::Vector2(scaleX, this->m_scale.Y));
}

spe::Vector2 spe::Transform::getDefaultTextureSize() const
{
	spe::Vector2 scale = this->m_scale;

	if (this->m_scale.X < 0)
	{
		scale.X = scale.X * -1;
	}
	if (this->m_scale.Y < 0)
	{
		scale.Y = scale.Y * -1;
	}
	return spe::Vector2(this->texture_size.X / scale.X, this->texture_size.Y / scale.Y);
}

void spe::Transform::setRotation(uint32_t angle)
{
	this->m_rotation = angle % 360;
	this->m_attached_sprite->getSprite().setRotation((float)this->m_rotation);
}

void spe::Transform::setOrigin()
{
	sf::Sprite& spr = this->m_attached_sprite->getSprite();
	spr.setOrigin(sf::Vector2f(this->getDefaultTextureSize().X / 2, this->getDefaultTextureSize().Y / 2));
}

void spe::Transform::setScale(const spe::Vector2& scale, bool b)
{
	if (this->m_scale == scale && !b)
	{
		return;
	}

	this->setTextureSize(scale);

	this->m_scale = scale;
	this->m_attached_sprite->getSprite().setScale(scale.X, scale.Y);
	this->setOrigin();
}

spe::Vector2 spe::Transform::getOrigininalPosition() const
{
	float x = this->m_attached_sprite->getSprite().getPosition().x - this->texture_size.X / 2;
	float y = this->m_attached_sprite->getSprite().getPosition().y - this->texture_size.Y / 2;

	return spe::Vector2(x, y);
}

void spe::Transform::reset()
{
	this->m_Position = spe::Vector2(0, 0);
	this->setScale(spe::Vector2(1, 1));
}

