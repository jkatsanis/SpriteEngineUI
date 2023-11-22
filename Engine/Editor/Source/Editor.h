#pragma once

#include "Window.h"
#include "UtilityFunctions.h"
#include "FileDataMacros.h"


#include <iostream>

namespace spe
{
	class Editor
	{
	private:
		spe::Window m_Window;

	public:
		Editor();

		void Update();
		bool IsOpen() { return true; }
	};
}