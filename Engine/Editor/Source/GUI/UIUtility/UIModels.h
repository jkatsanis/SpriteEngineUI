#pragma once

#include <ImGui/ImGUISFML.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <icons.h>

#include "Components/Animator/KeyFrame.h"
#include "Components/Animator/Animation.h"

#include "Utility/Style.h"

#define DEFAULT_FLAGS ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar

namespace spe
{
	struct KeyFrameSelected
	{
		int position;
		const spe::KeyFrame* keyFrameSelected;
		bool isClicked;
	};
	enum class EditorTools
	{
		None = -1,
		PositionTool = 0,
		ScaleTool = 1,
	};
	struct Tool
	{
		EditorTools tool;
		bool background;
		std::string icon;
		std::string tool_name;

		Tool(const EditorTools tool, const std::string& icon, const std::string& name)
		{
			this->tool_name = name;
			this->background = false;
			this->tool = tool;
			this->icon = icon;
		}

		Tool()
		{
			this->tool_name = "";
			this->background = false;
			this->tool = spe::EditorTools::PositionTool;
			this->icon = "";
		}
	};
	struct UIWindowData 
	{
		bool Reload;
		bool IsOpen;
		ImVec2* ptr_Size;

		UIWindowData() : ptr_Size(nullptr), Reload(true), IsOpen(true) { }
		UIWindowData(bool reload, bool open, ImVec2* size) : Reload(reload), IsOpen(open), ptr_Size(size) { }

		void SetOpen()
		{
			Reload = true;
			IsOpen = true;
		}
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

		Rectangle(const sf::Vector2f& pos,
			const sf::Vector2f& size, const sf::Color& outline_color,
			float outline_thickness, const std::string& path_to_texture, const std::string& name)
		{
			this->SortingLayerIdx = 0;
			this->Render = true;
			this->ID = 0;
			this->Name = name;

			this->Texture.loadFromFile(path_to_texture);

			this->Shape.setTexture(&this->Texture);
			this->Shape.setPosition(pos);
			this->Shape.setSize(size);
			this->Shape.setOutlineColor(outline_color);
			this->Shape.setOutlineThickness(outline_thickness);
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
	struct ScaleDott
	{
		spe::Rectangle* ptr_scaling_rectangle;
		bool clicked;
	};
}