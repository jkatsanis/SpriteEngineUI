#include "childSystem.h"
#include <physicalComponents/sprite.h>

void s2d::ChildSystem::updatePositionToParent(s2d::Sprite* child)
{
	if (child->parent != nullptr)
	{
		s2d::Vector2 distance = s2d::Vector2(child->parent->transform.position - child->transform.position);
		child->transform.position_to_parent = distance;
	}
}

void s2d::ChildSystem::updateChildPositionToParentRecursivly(s2d::Sprite* parent)
{
	for (int i = 0; i < parent->ptr_childs.size(); i++)
	{
		s2d::Sprite* child = parent->ptr_childs[i];
		s2d::Vector2 distance = s2d::Vector2(parent->transform.position - child->transform.position);
		child->transform.position_to_parent = distance;

		updateChildPositionToParentRecursivly(child);
	}
}

void s2d::ChildSystem::updateChildPositionRecursivly(s2d::Sprite* parent)
{
	for(int i = 0; i < parent->ptr_childs.size(); i++)
	{
		s2d::Sprite* child = parent->ptr_childs[i];
		child->transform.position =  parent->transform.position - child->transform.position_to_parent;
		updateChildPositionRecursivly(child);
	}
}
