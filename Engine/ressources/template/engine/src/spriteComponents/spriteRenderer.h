#pragma once

#include <spriteComponents/component.h>

#include <iostream>

namespace s2d
{
	class SpriteRenderer : public s2d::Component
	{
	private:
		void init() override;
	public:
		std::string path;
		int32_t sorting_layer_index;
		bool effected_by_light;

		SpriteRenderer();

		void reset();
	};
}

