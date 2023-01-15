#pragma once

#include <imgui-SFML.h>
#include <imgui.h>

namespace s2d
{
	class GUIManager
	{
	public:
		GUIManager() = delete;

		static void update();
		static float s_fontSize;
	};
}

