#include "Transform.h"
#include "Sprite.h"

//Contructor

spe::Transform::Transform()
{
	this->Init();
}

spe::Transform::Transform(spe::Sprite* attached)
{
	this->Init();
	this->ptr_Sprite = attached;
}

spe::Transform::Transform(spe::Sprite* attachedSprite, spe::Transform& transform)
{
	this->Init();
	this->ptr_Sprite = attachedSprite;
	this->m_Position = transform.m_Position;
	this->m_Rotation = transform.m_Rotation;
	this->m_Scale = transform.m_Scale;
	this->TextureSize = transform.TextureSize;
	this->PositionToParent = transform.PositionToParent;

	this->SetScale(this->m_Scale, true);
	this->SetOrigin();
	this->SetTextureSize(this->m_Scale);
}

void spe::Transform::Init()
{
	this->BaseComponent = true;
	this->ptr_Sprite = nullptr;
	this->m_Rotation = 0;
	this->m_Scale = spe::Vector2(1, 1);
}

// Private functions

void spe::Transform::UpdateSpritePositionToParent(const spe::Vector2& position)
{
	if (this->ptr_Sprite == nullptr)
	{
		return;
	}
	for (spe::Sprite* spr : this->ptr_Sprite->ptr_Childs)
	{
		spe::Vector2 distance = this->m_Position - position;

		distance.X *= -1;
		distance.Y *= -1;

		spe::Vector2 newpos = spr->Transform.GetPosition() +distance;
		spr->Transform.SetPosition(newpos);
	}

}

spe::Vector2 spe::Transform::HandleCollisions(const spe::Vector2& position)
{
	spe::Vector2 new_position(position);

	if (this->ptr_Sprite == nullptr || !this->ptr_Sprite->Collider.Exist || !this->ptr_Sprite->Collider.Collided)
	{
		// No collision check
		return new_position;
	}

	const float current_y = this->ptr_Sprite->Transform.m_Position.Y;
	const float current_x = this->ptr_Sprite->Transform.m_Position.X;

	// Down	
	if (this->ptr_Sprite->Collider.Down)
	{
		if (position.Y < this->m_Position.Y
			&& this->ptr_Sprite->Physicsbody.Velocity.Y <= 0)
		{
			new_position.Y = current_y;
		}
	}

	// Up	
	if (this->ptr_Sprite->Collider.Up)
	{
		if (position.Y > this->m_Position.Y
			&& this->ptr_Sprite->Physicsbody.Velocity.Y >= 0)
		{
			new_position.Y = current_y;
		}
	}

	// Left	
	if (this->ptr_Sprite->Collider.Left)
	{
		if (position.X < this->m_Position.X
			&& this->ptr_Sprite->Physicsbody.Velocity.X <= 0)
		{
			new_position.X = current_x;
		}
	}

	// Right	
	if (this->ptr_Sprite->Collider.Right)
	{
		if (position.X > this->m_Position.X
			&& this->ptr_Sprite->Physicsbody.Velocity.X >= 0)
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
		this->PositionChanged = false;
		return;
	}

	const spe::Vector2 new_pos = this->HandleCollisions(position);

	this->UpdateSpritePositionToParent(new_pos);
	this->m_Position = new_pos;
	this->PositionChanged = true;

	if (this->ptr_Sprite != nullptr)
	{
		this->ptr_Sprite->GetSprite().setPosition(sf::Vector2f(new_pos.X + 960, 540 - new_pos.Y));
	}
}

void spe::Transform::SetTextureSize(const spe::Vector2& scale)
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

	sf::IntRect textureRect = this->ptr_Sprite->GetSprite().getTextureRect();
	this->TextureSize = spe::Vector2(textureRect.width * multiply.X, textureRect.height * multiply.Y);
	this->SetOrigin();
}

void spe::Transform::CalculateScaleXByWorldPosition(const float posX)
{
	float scaleX = posX / this->TextureSize.X;

	this->SetScale(spe::Vector2(scaleX, this->m_Scale.Y));
}

spe::Vector2 spe::Transform::GetDefaultTextureSize() const noexcept
{
	spe::Vector2 scale = this->m_Scale;

	if (this->m_Scale.X < 0)
	{
		scale.X = scale.X * -1;
	}
	if (this->m_Scale.Y < 0)
	{
		scale.Y = scale.Y * -1;
	}
	return spe::Vector2(this->TextureSize.X / scale.X, this->TextureSize.Y / scale.Y);
}

void spe::Transform::SetRotation(uint32_t angle)
{
	this->m_Rotation = angle % 360;
	this->ptr_Sprite->GetSprite().setRotation((float)this->m_Rotation);
}

void spe::Transform::Teleport(const spe::Vector2& position)
{
	if (this->m_Position == position)
	{
		this->PositionChanged = false;
		return;
	}

	this->UpdateSpritePositionToParent(position);
	this->m_Position = position;
	this->PositionChanged = true;

	if (this->ptr_Sprite != nullptr)
	{
		this->ptr_Sprite->GetSprite().setPosition(sf::Vector2f(position.X + 960, 540 - position.Y));
	}

}

void spe::Transform::SetOrigin()
{
	sf::Sprite& spr = this->ptr_Sprite->GetSprite();
	spr.setOrigin(sf::Vector2f(this->GetDefaultTextureSize().X / 2, this->GetDefaultTextureSize().Y / 2));
}

void spe::Transform::SetScale(const spe::Vector2& scale, bool b)
{
	if (this->m_Scale == scale && !b)
	{
		return;
	}

	this->SetTextureSize(scale);

	this->m_Scale = scale;
	this->ptr_Sprite->GetSprite().setScale(scale.X, scale.Y);
	this->SetOrigin();
}

spe::Vector2 spe::Transform::GetOrigininalPosition() const
{
	float x = this->ptr_Sprite->GetSprite().getPosition().x - this->TextureSize.X / 2;
	float y = this->ptr_Sprite->GetSprite().getPosition().y - this->TextureSize.Y / 2;

	return spe::Vector2(x, y);
}

void spe::Transform::Reset()
{
	this->m_Position = spe::Vector2(0, 0);
	this->SetScale(spe::Vector2(1, 1));
}

