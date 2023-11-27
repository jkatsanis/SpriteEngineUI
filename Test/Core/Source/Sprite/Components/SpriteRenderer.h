#pragma once

#include <iostream>

#include "Sprite/Components/Component.h"

namespace spe
{
	class Sprite;
	class SpriteRenderer : public spe::Component
	{
	private:
		void init() override;
	public:
		std::string path;
		uint32_t sorting_layer_index;
		bool effected_by_light;
		bool render;
	
		SpriteRenderer();
		SpriteRenderer(const spe::SpriteRenderer& rhs);

		void reset();
	};
}

