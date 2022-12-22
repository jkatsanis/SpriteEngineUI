#include "childSystem.h"
#include <sprite.h>

void s2d::ChildSystem::updatePositionToParent(s2d::Sprite* child)
{
	if (child->parent != nullptr)
	{
		s2d::Vector2 distance = s2d::Vector2(child->parent->transform.position - child->transform.position);
		child->transform.positionToParent = distance;
	}
}

void s2d::ChildSystem::updateChildPositionToParentRecursivly(s2d::Sprite* parent)
{
	for (s2d::Sprite* child : parent->childs)
	{
		s2d::Vector2 distance = s2d::Vector2(parent->transform.position - child->transform.position);
		child->transform.positionToParent = distance;

		updateChildPositionToParentRecursivly(child);
	}
}

void s2d::ChildSystem::updateChildPositionRecursivly(s2d::Sprite* parent)
{
	for (s2d::Sprite* child : parent->childs)
	{
		child->transform.position =  parent->transform.position - child->transform.positionToParent;
		std::cout << child->transform.positionToParent.x << std::endl;
		updateChildPositionRecursivly(child);
	}
}
