#pragma once

#include <iostream>

namespace s2d
{
	class Sprite;
	class ChildSystem
	{
	public:
		ChildSystem() = delete;

		static void updatePositionToParent(s2d::Sprite* parent);
		static void updateChildPositionToParentRecursivly(s2d::Sprite* parent);
		static void updateChildPositionRecursivly(s2d::Sprite* ptr_parent);
	};
}

