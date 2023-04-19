#pragma once

#include <SFML/Graphics.hpp>
#include <ImGui.h>
#include <data/UIInfo.h>

namespace s2d
{
	class UIIconData
	{
	private:
		std::vector<sf::Texture> m_textures;
		std::vector<uint32_t> m_ids;

	public:
		// Init all textures and get the paths from UIInfo
		UIIconData();

		uint32_t getId(std::string fileExtension);
	};
}
