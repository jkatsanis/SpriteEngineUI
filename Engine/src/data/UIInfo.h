#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <ImGui.h>
#include <manager/initializer.h>

#define ICONS_AMOUNT 5

namespace s2d
{
	class UIInfo
	{
		UIInfo() = delete;

	public:
		static float s_defaultFontSize;
		static bool s_renderAssetFolder;
		static bool s_isAssetFolderActive;

		const static std::string s_icons[ICONS_AMOUNT];
	};
}

