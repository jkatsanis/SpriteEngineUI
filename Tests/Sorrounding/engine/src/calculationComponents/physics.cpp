#include "physics.h"

//Public static functions

void s2d::Physics::update()
{
	for (s2d::Sprite* const ptr_activeSprite : s2d::Sprite::activeSprites)
	{
		ptr_activeSprite->physicsBody.fixedUpdate();
	}	
}

void s2d::Physics::addForce(s2d::Sprite* sprite, const s2d::Vector2& direction, float force)
{
	float mass = sprite->physicsBody.mass;
	float speed = force / mass;
	s2d::Vector2 velocity = s2d::Vector2(direction.x * speed, direction.y * speed);
	sprite->physicsBody.velocity += velocity;
}
