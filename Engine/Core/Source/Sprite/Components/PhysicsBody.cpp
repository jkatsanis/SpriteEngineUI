#include "physicsBody.h"

#include "Sprite/Sprite.h"

// Constructor / Destructor

spe::PhsysicsBody::PhsysicsBody()
{
	this->init();
}

spe::PhsysicsBody::PhsysicsBody(spe::Sprite* spr)
{
	this->init();
	this->ptr_Sprite = spr;
}

spe::PhsysicsBody::PhsysicsBody(spe::Sprite* spr, const spe::PhsysicsBody& rhs)
{
	this->ptr_Sprite = spr;
	this->Velocity = rhs.Velocity;
	this->Mass = rhs.Mass;
	this->exist = rhs.exist;
	this->Gravity = rhs.Gravity;
}

void spe::PhsysicsBody::init()
{
	this->base_component = false;
	this->exist = false;
	this->Velocity = spe::Vector2(0, 0);
	this->Gravity = 0.0f;
	this->Mass = 0.0f;
}

// Public functions

void spe::PhsysicsBody::reset()
{
	this->exist = false;
	this->Mass = 0.0f;
	this->Velocity = spe::Vector2(0.0f, 0.0f);
	this->Gravity = 0.0f;
}

void spe::PhsysicsBody::Update()
{
	if (!this->exist || this->ptr_Sprite == nullptr) return;

	//ALl Physic calcutions will happen here!
	spe::Vector2 dir = this->Velocity * spe::Time::s_DeltaTime;
	dir += this->ptr_Sprite->Transform.GetPosition();

	this->ptr_Sprite->Transform.SetPosition(dir);


	if (this->ptr_Sprite->Collider.Down && this->Velocity.Y <= 0)
	{
		this->Velocity.Y = 0.0f;
	}

	this->Velocity.Y -= this->Gravity;

}

