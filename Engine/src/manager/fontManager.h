#pragma once

#include <ImGui.h>
#include <data/UIInfo.h>
#include <imgui-SFML.h>
#include <manager/initializer.h>

namespace s2d
{
	class FontManager
	{
	public:
		FontManager() = delete;

		static ImFont* imGuiDefaultFont;
		static ImFont* s_default_font;
		static ImFont* s_symbol_font;
		static float fontSize;

		static bool displaySymbolInMenuItem(const char* symbol);
		static void displaySmybolAsText(const char* symbol);
		static bool displaySmybolAsButton(const char* symbol, ImFont* font = nullptr);
		static bool displaySmybolAsButton(const char* symbol, float defaultFontSize);
		static bool displaySmybolAsButton(const char* symbol, ImVec2 cursorPos);
		static bool displaySymbolInTreeNode(const char* symbol, std::string name, bool openNode);
		static bool displaySymbolInMenuItemWithText(const char* symbol, std::string name);
		static void InitFonts(ImGuiIO& io);
	};

}

