#include "Physics.h"

void spe::Physics::AddForce(spe::Sprite* sprite, const spe::Vector2& direction, float force)
{
	float mass = sprite->Physicsbody.Mass;
	float speed = force / mass;
	const spe::Vector2 velocity = spe::Vector2(direction.X * speed, direction.Y * speed);
	sprite->Physicsbody.Velocity += velocity;
}
