#pragma once

#include <engineComponents/keyBoard.h>

namespace s2d
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

		s2d::KeyBoardCode key;
		Type type;

		Event();

	};
}

