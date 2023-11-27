#pragma once

#include <ImGui/ImGUISFML.h>
#include "RessourceHandler/FileDataMacros.h"

namespace spe
{
	class Style
	{
	public:
		Style() = delete;

		static void Init();
		static void RenderStyle();

		static bool DisplaySymbolAsButtonWithWidthAndCursorPos(const char* symbol, const ImVec2& cursor, const ImVec2& width, const std::string& identy);
		static bool DisplaySymbolInMenuItem(const char* symbol);
		static void DisplaySmybolAsText(const char* symbol);
		static bool DisplaySmybolAsButton(const char* symbol, ImFont* font = nullptr);
		static bool DisplaySmybolAsButton(const char* symbol, float defaultFontSize);
		static bool DisplaySmybolAsButton(const char* symbol, ImVec2 cursorPos);
		static bool DisplaySymbolInTreeNode(const char* symbol, std::string name, bool openNode);
		static bool DisplaySymbolInMenuItemWithText(const char* symbol, std::string name);

		static ImFont* s_DefaultFont;
		static ImFont* s_SymbolFont;
		static float s_FontSize;
		static float s_DefaultFontSize;
	};
}