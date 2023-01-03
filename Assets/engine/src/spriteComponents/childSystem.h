#pragma once

#include <iostream>

namespace s2d
{
	class Sprite;
	class ChildSystem
	{
	private:
		static void setBoxColliderPosition(s2d::Sprite* child, s2d::Sprite* node);

	public:
		ChildSystem() = delete;

		static void resetPositionWhenChildIsColliding(s2d::Sprite* child);
		static void updatePositionToParent(s2d::Sprite* parent);
		static void updateChildPositionRecursivly(s2d::Sprite* ptr_parent);
		static void updatePositionRevursivWhenChildIsColliding(s2d::Sprite* node);
	
	};
}

