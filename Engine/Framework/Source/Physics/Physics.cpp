#include "Physics.h"

void spe::Physics::AddForce(spe::Sprite* sprite, const spe::Vector2& direction, float force)
{
	float mass = sprite->Physicsbody.Mass;
	float speed = force / mass;
	const spe::Vector2 velocity = spe::Vector2(direction.X * speed, direction.Y * speed);
	sprite->Physicsbody.Velocity += velocity;
}

float spe::Physics::Lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

spe::Vector2 spe::Physics::Lerp(const spe::Vector2& a, const spe::Vector2& b, float t)
{
	float x = a.X + (b.X - a.X) * t;
	float y = a.Y + (b.Y - a.Y) * t;
	return spe::Vector2(x, y);
}
