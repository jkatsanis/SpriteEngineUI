#include "spriteComponents/childSystem.h"
#include <physicalComponents/sprite.h>

void s2d::ChildSystem::updateChildPositionRecursivly(s2d::Sprite* parent)
{
	for (s2d::Sprite* child : parent->ptr_childs)
	{
		s2d::Vector2 p = parent->transform.getPosition() - child->transform.position_to_parent;
		child->transform.setPositionForce(p);
		updateChildPositionRecursivly(child);
	}
}

// RESET POSITION WHEN TREE MEMBER IS COLLIDING

void s2d::ChildSystem::updatePositionRevursivWhenChildIsColliding(s2d::Sprite* node, s2d::Sprite* copy)
{
	//Iterating over all sprites so set the position enum
	for (s2d::Sprite* child : node->ptr_childs)
	{
		setBoxColliderPosition(child, copy);
		updatePositionRevursivWhenChildIsColliding(child, copy);
	}
}


void s2d::ChildSystem::resetPositionWhenChildIsColliding(s2d::Sprite* child)
{
	if (!child->collider.collided)
	{
		return;
	}

	s2d::Sprite* node = child->getNode();

	setBoxColliderPosition(node, child);

	s2d::ChildSystem::updatePositionRevursivWhenChildIsColliding(node, child);
}


void s2d::ChildSystem::setBoxColliderPosition(s2d::Sprite* child, s2d::Sprite* copy)
{
	if (child->collider.left)
	{
		copy->collider.left = true;
	}
	if (child->collider.right)
	{
		copy->collider.right = true;
	}	
	if (child->collider.up)
	{
		copy->collider.up = true;
	}
	if (child->collider.down)
	{
		copy->collider.down = true;
	}

	child->collider.left = copy->collider.left;
	child->collider.right = copy->collider.right;
	child->collider.up = copy->collider.up;
	child->collider.down = copy->collider.down;
}
