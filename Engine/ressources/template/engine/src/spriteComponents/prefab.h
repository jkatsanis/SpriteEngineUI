#pragma once

#include <iostream>
#include <spriteComponents/component.h>

namespace s2d
{
	class Sprite;
	class Prefab : public s2d::Component
	{
	private:
		s2d::Sprite* m_ptr_attachedSprite;
		void init() override;
	public:
		bool load_in_memory;

		Prefab();
		Prefab(s2d::Sprite* m_attached);

		void reset() override;
	};
}

