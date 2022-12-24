#pragma once

#include <SFML/Graphics.hpp>
#include <ImGui.h>
#include <UIInfo.h>

namespace s2d
{
	class UIIconData
	{
	public:
		sf::Texture textureFolder;
		ImTextureID textureIDFodler;

		sf::Texture textureFile;
		ImTextureID textureIDFile;

		// Init all textures and get the paths from UIInfo
		UIIconData();

		ImTextureID getId(std::string fileExtension);
	};
}
