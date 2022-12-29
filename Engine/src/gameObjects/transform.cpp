#include "stransform.h"
#include <sprite.h>

//Contructor

s2d::Transform::Transform()
{
	this->position = s2d::Vector2(0.0f, 0.0f);
}

bool s2d::Transform::setLastPosition()
{
	if (this->nextPos != this->position)
	{
		this->lastPos = this->nextPos;
		this->nextPos = this->position;
		return true;
	}
}

//Public static functions

void s2d::Transform::onPositionChange(s2d::Sprite* sprite)
{
	ChildSystem::updatePositionToParent(sprite);
	ChildSystem::updateChildPositionRecursivly(sprite);
	ChildSystem::updateChildPositionToParentRecursivly(sprite);
}
