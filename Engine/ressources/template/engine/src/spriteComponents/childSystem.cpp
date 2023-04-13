#include "spriteComponents/childSystem.h"
#include <physicalComponents/sprite.h>

void s2d::ChildSystem::updatePositionToParent(s2d::Sprite* child)
{
	if (child->parent != nullptr)
	{
		s2d::Vector2 distance = s2d::Vector2(child->parent->transform.position - child->transform.position);
		child->transform.positionToParent = distance;
	}
}

void s2d::ChildSystem::updateChildPositionRecursivly(s2d::Sprite* parent)
{
	for (s2d::Sprite* child : parent->childs)
	{
		child->transform.position = parent->transform.position - child->transform.positionToParent;
		s2d::Vector2 distance = s2d::Vector2(parent->transform.position - child->transform.position);
		child->transform.positionToParent = distance;

		updateChildPositionRecursivly(child);
	}
}

// RESET POSITION WHEN TREE MEMBER IS COLLIDING

void s2d::ChildSystem::updatePositionRevursivWhenChildIsColliding(s2d::Sprite* child)
{
	//Iterating over all sprites so set the position enum
	for (s2d::Sprite* node : child->childs)
	{
		setBoxColliderPosition(node, child);
	}
}


void s2d::ChildSystem::resetPositionWhenChildIsColliding(s2d::Sprite* child)
{
	if (!child->collider.isInCollision())
	{
		return;
	}
	s2d::Sprite* node = child->getNode();

	setBoxColliderPosition(child, node);

	s2d::ChildSystem::updatePositionRevursivWhenChildIsColliding(node);
}


void s2d::ChildSystem::setBoxColliderPosition(s2d::Sprite* child, s2d::Sprite* node)
{
	for (int i = 0; i < s2d::BoxColliderPositionData::s_canCollideSpritesAround; i++)
	{
		s2d::BoxColliderPositionData::Position data = static_cast<s2d::BoxColliderPositionData::Position>(static_cast<int>(i));
		if (child->collider.positionData.isEqual(data))
		{
			//A sprite can collidde from left and one from up, we need to not go RIGHT AND DOWN 
			node->collider.positionData.position[node->collider.collisionCnt] = data;
			node->collider.collisionCnt++;
		}
	}
}
