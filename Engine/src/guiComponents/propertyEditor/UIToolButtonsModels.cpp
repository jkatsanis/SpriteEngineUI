#include "UIToolButtonsModels.h"

// Tool constructor

s2d::Tool::Tool(const EditorTools tool, const std::string& icon, const std::string& name)
{
	this->tool_name = name;
	this->background = false;
	this->tool = tool;
	this->icon = icon;
}

s2d::Tool::Tool()
{
	this->tool_name = "";
	this->background = false;
	this->tool = s2d::EditorTools::PositionTool;
	this->icon = "";
}

