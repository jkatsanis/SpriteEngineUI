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
			Released = 0,
			Pressed
		};

		spe::KeyBoardCode key;
		Type type;

		Event();

	};
}
