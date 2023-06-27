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
			KeyReleased = 0,
			KeyPressed,
			MousePressedLeft,
			MouseReleasedLeft,
		};

		s2d::KeyBoardCode key;
		Type type;

		Event();

	public:
		static bool s_handle_other_events;

	};	
}

