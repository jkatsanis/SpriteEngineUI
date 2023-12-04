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
	this->velocity = rhs.velocity;
	this->mass = rhs.mass;
	this->exist = rhs.exist;
	this->gravity = rhs.gravity;
}

void spe::PhsysicsBody::init()
{
	this->base_component = false;
	this->exist = false;
	this->velocity = spe::Vector2(0, 0);
	this->gravity = 0.0f;
	this->mass = 0.0f;
}

// Public functions

void spe::PhsysicsBody::reset()
{
	this->exist = false;
	this->mass = 0.0f;
	this->velocity = spe::Vector2(0.0f, 0.0f);
	this->gravity = 0.0f;
}

void spe::PhsysicsBody::Update()
{
	if (!this->exist || this->ptr_Sprite == nullptr) return;

	//ALl Physic calcutions will happen here!
	spe::Vector2 dir = this->velocity * spe::Time::s_DeltaTime;
	dir += this->ptr_Sprite->transform.GetPosition();

	this->ptr_Sprite->transform.SetPosition(dir);


	if (this->ptr_Sprite->collider.down && this->velocity.Y <= 0)
	{
		this->velocity.Y = 0.0f;
	}

	this->velocity.Y -= this->gravity;

}

