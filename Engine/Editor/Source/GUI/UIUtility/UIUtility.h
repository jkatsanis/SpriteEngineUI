#pragma once

#include <ImGui/ImGUISFML.h>
#include <iostream>

#include "Math/Vector2.h"

namespace spe
{
	class UIUtility
	{
	public:
		static spe::Vector2 GUICursor;

		static void UpdateCursor();

		UIUtility() = delete;

		static void DrawRectangleInGUIWIndow(const ImVec2& size, const ImVec2& top_left, const ImColor& color);
		static bool IsHovered(const ImVec2& windowPos, const ImVec2& windowSize);
		static bool HandleCloseAndReloadWindow(bool open, bool& reload, bool& hovered, ImVec2& window_size, const ImVec2& original_size);
	};
}