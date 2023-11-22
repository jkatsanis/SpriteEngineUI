#pragma once

#include "Component.h"

#include <iostream>

namespace spe
{
	class Sprite;
	class SpriteRenderer : public spe::Component
	{
	private:
		void init() override;
	public:
		std::string path;
		int32_t sorting_layer_index;
		bool effected_by_light;
	
		SpriteRenderer();
		SpriteRenderer(const spe::SpriteRenderer& rhs);

		void reset();
	};
}

