#include "physicsBody.h"

// Constructor / Destructor

s2d::PhsysicsBody::PhsysicsBody()
{
	this->init();
}

void s2d::PhsysicsBody::init()
{
	this->base_component = false;
	this->exist = false;
	this->velocity = s2d::Vector2(0, 0);
	this->gravity = 0.0f;
	this->mass = 0.0f;
}

// Public functions

void s2d::PhsysicsBody::reset()
{
	this->exist = false;
	this->mass = 0.0f;
	this->velocity = s2d::Vector2(0.0f, 0.0f);
	this->gravity = 0.0f;
}

