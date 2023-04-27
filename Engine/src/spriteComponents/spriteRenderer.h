#pragma once

#include <spriteComponents/component.h>
#include <data/spriteData.h>

#include <iostream>

namespace s2d
{
	class SpriteRenderer : s2d::Component
	{
	private:
		void init() override;
	public:
		std::string path;
		int32_t sorting_layer_index;

		SpriteRenderer();

		void reset();
	};
}

