#include "physicsBody.h"

s2d::PhsysicsBody::PhsysicsBody()
	: exists(false), gravity(0.0f), mass(0.0f)
{
	this->velocity = s2d::Vector2(0, 0);
}

void s2d::PhsysicsBody::resetPhysicsBody()
{
	this->exists = false;
	this->mass = 0.0f;
	this->velocity = s2d::Vector2(0.0f, 0.0f);
	this->gravity = 0.0f;
}

