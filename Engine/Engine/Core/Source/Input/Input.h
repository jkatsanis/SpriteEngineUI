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
		static spe::Event* Event;

	public:
		Input() = delete;

		
		static bool OnKeyRelease(spe::KeyBoardCode key);
		static bool OnKeyPress(spe::KeyBoardCode key);
		static bool OnKeyHold(spe::KeyBoardCode key);

		static void SetEvent(spe::Event* event) noexcept;
	};
}