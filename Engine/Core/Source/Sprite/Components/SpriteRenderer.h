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
		std::string Path;
		uint32_t SortinLayerIdx;
		bool EffectedByLight;
		bool Render;
	
		SpriteRenderer();
		SpriteRenderer(const spe::SpriteRenderer& rhs);

		void reset() override;
	};
}

