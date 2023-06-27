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

		Tool(const EditorTools tool, const std::string& icon, const std::string& name)
		{
			this->tool_name = name;
			this->background = false;
			this->tool = tool;
			this->icon = icon;
		}

		Tool()
		{
			this->tool_name = "";
			this->background = false;
			this->tool = s2d::EditorTools::PositionTool;
			this->icon = "";
		}
	};
}