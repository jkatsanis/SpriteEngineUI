#include <iostream>

namespace s2d
{
	enum class EditorTools
	{
		PositionTool,
		ScaleTool
	};
	struct Tool
	{
		EditorTools tool;
		bool background;
		std::string icon;

		Tool(const EditorTools tool, std::string icon)
		{
			this->background = false;
			this->tool = tool;
			this->icon = icon;
		}

		Tool()
		{
			this->background = false;
			this->tool = s2d::EditorTools::PositionTool;
			this->icon = "";
		}
	};
}