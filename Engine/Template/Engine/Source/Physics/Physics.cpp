#include "Physics.h"

void spe::Physics::AddForce(spe::Sprite* sprite, const spe::Vector2& direction, float force)
{
	float mass = sprite->physicsBody.mass;
	float speed = force / mass;
	const spe::Vector2 velocity = spe::Vector2(direction.x * speed, direction.y * speed);
	sprite->physicsBody.velocity += velocity;
}
