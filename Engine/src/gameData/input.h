#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <event.h>
#include <keyBoard.h>

namespace s2d
{
	class Input
	{
	private:
		static s2d::Event* event;

	public:
		Input() = delete;

		
		static bool onKeyRelease(s2d::KeyBoardCode key);
		static bool onKeyPress(s2d::KeyBoardCode key);
		static bool onKeyHold(s2d::KeyBoardCode key);

		static void setEvent(s2d::Event* event);
	};
}