#pragma once

#include "Input/Keyboard.h"

namespace spe
{
	class Event
	{
	public:
		enum Type
		{
			None = -1,
			KeyReleased = 0,
			KeyPressed,
			MousePressedLeft,
			MouseReleasedLeft,
		};

		spe::KeyBoardCode Key;
		Type Type;

		Event();

	};
}
