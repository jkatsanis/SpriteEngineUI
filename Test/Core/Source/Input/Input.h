#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Core/Event.h"
#include "Input/Keyboard.h"

namespace spe
{
	class Input
	{
	private:
		static spe::Event* event;

	public:
		Input() = delete;

		
		static bool onKeyRelease(spe::KeyBoardCode key);
		static bool onKeyPress(spe::KeyBoardCode key);
		static bool onKeyHold(spe::KeyBoardCode key);

		static void setEvent(spe::Event* event);
	};
}