#include "physicsBody.h"
#include <physicalComponents/sprite.h>

//Constructor

s2d::PhsysicsBody::PhsysicsBody()
	: exists(false), gravity(0.0f), mass(0.0f)
{
	this->velocity = s2d::Vector2(0, 0);
}

s2d::PhsysicsBody::PhsysicsBody(s2d::Sprite* ptr_attachedSprite)
    : ptr_attachedSprite(ptr_attachedSprite), exists(false), gravity(0.0f), mass(0.0f)
{

}

//Public functions

void s2d::PhsysicsBody::resetPhysicsBody()
{
	this->exists = false;
	this->mass = 0.0f;
	this->velocity = s2d::Vector2(0.0f, 0.0f);
	this->gravity = 0.0f;
}

void s2d::PhsysicsBody::fixedUpdate()
{
    if (!this->exists || this->ptr_attachedSprite == nullptr || this->gravity == 0) return;

	//ALl Physic calcutions will happen here! -> calle from s2d::physics::update();

    this->ptr_attachedSprite->transform.position += this->velocity * s2d::Time::deltaTime;

    if (this->exists)
    {
        if (this->ptr_attachedSprite->collider.isInCollision() && this->ptr_attachedSprite->collider.positionData.isEqual(s2d::BoxColliderPositionData::Down))
        {
            this->velocity.y = 0.0f;
        }
        else
        {
            this->velocity.y -= this->gravity * s2d::Time::deltaTime;
        }
    }
}

