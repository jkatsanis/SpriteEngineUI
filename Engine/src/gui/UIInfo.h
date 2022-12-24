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

		static std::string sfolderIconPath;
		static std::string sdefaultIcon;

		static sf::Texture textureFolder;
		static ImTextureID textureIdFolder;

		static sf::Texture file;
		static ImTextureID fileId;

		static void Init();
		static void Ini();
	};
}

