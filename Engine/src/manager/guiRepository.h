#pragma once

#include <ImGui.h>

namespace s2d
{
	struct GUIRepository
	{
		const ImVec2* ptr_asset_window_size;
		const ImVec2* ptr_inspector_window_size;
		const ImVec2* ptr_hierarchy_window_size;
	};
}

