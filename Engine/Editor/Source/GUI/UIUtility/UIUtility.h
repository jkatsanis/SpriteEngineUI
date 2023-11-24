#pragma once

#include <ImGui/ImGUISFML.h>
#include <iostream>

#include "Math/Vector2.h"
#include "GUI/UIUtility/UIModels.h"
#include "Utility/Style.h"

namespace spe
{
	class UIUtility
	{
	public:
		static spe::Vector2 GUICursor;

		static void UpdateCursor();

		UIUtility() = delete;

		static bool RenderCloseRectangle(float padding_left, const char* icon, const std::string& id,
			const std::string& content, float cursor_pos);
		static void DrawRectangleInGUIWIndow(const ImVec2& size, const ImVec2& top_left, const ImColor& color);
		static bool IsHovered(const ImVec2& windowPos, const ImVec2& windowSize);
		static bool HandleCloseAndReloadWindow(spe::UIWindowData& data, bool& hovered, const ImVec2& original_size);
	};
}