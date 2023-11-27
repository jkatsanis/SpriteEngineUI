#pragma once

#include "Math/Vector2.h"

namespace spe
{
	class UITransform
	{
	public:
		spe::Vector2 Position;
		spe::Vector2 LastPosition;
		spe::Vector2 NextPosition;
		bool PositionChanged;

		void SetLastPosition();
	};
}