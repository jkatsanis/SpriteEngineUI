#pragma once

#include <ImGui.h>
#include <UIInfo.h>

namespace s2d
{
	class FontManager
	{
	public:
		FontManager() = delete;

		static ImFont* imGuiDefaultFont;
		static ImFont* defaultFont;
		static ImFont* symbolFont;
		static float fontSize;

		static void displaySmybolAsText(const char* symbol);
		static bool displaySmybolAsButton(const char* symbol, ImFont* font = nullptr);
		static bool displaySmybolAsButton(const char* symbol, float defaultFontSize);
	};

}

