#pragma once

#include <iostream>

namespace s2d
{
	enum class EditorTools
	{
		None = -1,
		PositionTool = 0,
		ScaleTool = 1,
	};

	struct Tool
	{
		EditorTools tool;
		bool background;
		std::string icon;
		std::string tool_name;

		Tool(const EditorTools tool, const std::string& icon, const std::string& name);
		Tool();
	};
}