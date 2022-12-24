#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <ImGui.h>

namespace s2d
{
	class UIInfo
	{
		UIInfo() = delete;

	public:
		static float sdefaultFontSize;
		static bool srenderAssetFolder;

		static std::string sfolderIcon;
		static std::string sdefaultIcon;
	};
}

