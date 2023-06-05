#include "physics.h"

//Public static functions

void s2d::Physics::update(s2d::SpriteRepository& repo)
{
	for (int i = 0; i < repo.amount(); i++)
	{
		s2d::Sprite* const sprite = repo.readAt(i);
		sprite->physicsBody.fixedUpdate();
	}	
}

void s2d::Physics::addForce(s2d::Sprite* sprite, const s2d::Vector2& direction, float force)
{
	float mass = sprite->physicsBody.mass;
	float speed = force / mass;
	s2d::Vector2 velocity = s2d::Vector2(direction.x * speed, direction.y * speed);
	sprite->physicsBody.velocity += velocity;
}
