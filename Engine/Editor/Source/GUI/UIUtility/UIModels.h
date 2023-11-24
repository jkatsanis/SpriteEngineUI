#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

namespace spe
{
	struct UIWindowData {
		bool Reload;
		bool IsOpen;
		ImVec2* ptr_Size;

		UIWindowData() : ptr_Size(nullptr) { }
		UIWindowData(bool reload, bool open, ImVec2* size) : Reload(reload), IsOpen(open), ptr_Size(size) { }
	};
	struct ResizeWindowData
	{
		float additinal_add;
		bool clicked_on_resize_button;
	};
	struct Rectangle
	{
		uint32_t SortingLayerIdx;
		uint32_t ID;
		std::string Name;
		sf::RectangleShape Shape;
		sf::Texture Texture;
		bool Render;

		Rectangle(const std::string& path_to_texture)
		{
			this->SortingLayerIdx = 0;
			this->Render = true;
			this->ID = 0;
			this->Texture.loadFromFile(path_to_texture);
			this->Shape.setTexture(&this->Texture);
		}
	};

	struct UserProjectInfo
	{
		std::string AbsulutePath;
		std::string relativePath;
		std::string name;
		std::string lastOpened;

		UserProjectInfo()
		{
			this->lastOpened = "none";
			this->AbsulutePath = "none";
			this->name = "none";
		}
		UserProjectInfo(std::string name, std::string abspath, std::string lastOpened, std::string relativePath)
		{
			this->lastOpened = lastOpened;
			this->name = name;
			this->AbsulutePath = abspath;
			this->relativePath = relativePath;

		}
	};
}