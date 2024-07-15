#include "physicsBody.h"

#include "Sprite/Sprite.h"

// Constructor / Destructor

spe::PhsysicsBody::PhsysicsBody()
{
	this->Init();
}

spe::PhsysicsBody::PhsysicsBody(spe::Sprite* spr)
{
	this->Init();
	this->ptr_Sprite = spr;
}

spe::PhsysicsBody::PhsysicsBody(spe::Sprite* spr, const spe::PhsysicsBody& rhs)
{
	this->ptr_Sprite = spr;
	this->Velocity = rhs.Velocity;
	this->Mass = rhs.Mass;
	this->Exist = rhs.Exist;
	this->Gravity = rhs.Gravity;
	this->Friction = rhs.Friction;
}

void spe::PhsysicsBody::Init()
{
	this->BaseComponent = false;
	this->Exist = false;
	this->Velocity = spe::Vector2(0, 0);
	this->Gravity = 0.0f;
	this->Mass = 0.0f;
	this->Friction = 0.0f;
}

void spe::PhsysicsBody::UpdateFriction()
{
	if (this->ptr_Sprite->Collider.Down)
	{
		// Apply friction based on deltaTime
		this->Velocity.X *= (1 - this->Friction * spe::Time::s_DeltaTime);

		// If the X velocity is very small, set it to 0 to stop the sliding
		if (std::abs(this->Velocity.X) < 0.01f)
		{
			this->Velocity.X = 0.0f;
		}
	}
}

// Public functions

void spe::PhsysicsBody::Reset()
{
	this->Exist = false;
	this->Mass = 0.0f;
	this->Velocity = spe::Vector2(0.0f, 0.0f);
	this->Gravity = 0.0f;
	this->Friction = 0.0f;
}

void spe::PhsysicsBody::Update()
{
	if (!this->Exist || this->ptr_Sprite == nullptr) return;

	//ALl Physic calcutions will happen here!
	spe::Vector2 dir = this->Velocity * spe::Time::s_DeltaTime;
	dir += this->ptr_Sprite->Transform.GetPosition();

	this->ptr_Sprite->Transform.SetPosition(dir);


	if (this->ptr_Sprite->Collider.Down && this->Velocity.Y <= 0)
	{
		this->Velocity.Y = 0.0f;
	}

	this->Velocity.Y -= this->Gravity * spe::Time::s_DeltaTime;

	if (this->Friction != 0)
	{
		this->UpdateFriction();
	}
}

