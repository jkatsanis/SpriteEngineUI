#include "physicsBody.h"

// Constructor / Destructor

spe::PhsysicsBody::PhsysicsBody()
{
	this->init();
}

spe::PhsysicsBody::PhsysicsBody(const spe::PhsysicsBody& rhs)
{
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

