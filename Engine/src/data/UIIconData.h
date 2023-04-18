#pragma once

#include <SFML/Graphics.hpp>
#include <ImGui.h>
#include <data/UIInfo.h>

namespace s2d
{
	class UIIconData
	{
	public:
		sf::Texture textureFolder;
		uint32_t textureIDFodler;

		sf::Texture textureFile;
		uint32_t textureIDFile;

		// Init all textures and get the paths from UIInfo
		UIIconData();

		uint32_t getId(std::string fileExtension);
	};
}
