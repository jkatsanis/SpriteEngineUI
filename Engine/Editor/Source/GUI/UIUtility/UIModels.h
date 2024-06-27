#pragma once

#include <imgui.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <icons.h>

#include "Sprite/Components/Animator/KeyFrame.h"	
#include "Sprite/Components/Animator/Animation.h"

#include "Utility/Style.h"

#define DEFAULT_FLAGS ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar

namespace spe
{
	struct KeyFrameSelected
	{
		int Position;
		const spe::KeyFrame* KeyFrameSelected;
		bool IsClicked;
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
		bool Background;
		std::string Icon;
		std::string Name;

		Tool(const EditorTools tool, const std::string& icon, const std::string& name)
		{
			this->Name = name;
			this->Background = false;
			this->tool = tool;
			this->Icon = icon;
		}

		Tool()
		{
			this->Name = "";
			this->Background = false;
			this->tool = spe::EditorTools::PositionTool;
			this->Icon = "";
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
		float AdditionalAdd;
		bool ClickedOnResizeButton;

		ResizeWindowData()
			: AdditionalAdd(0), ClickedOnResizeButton(false) { }
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
		std::string Name;
		std::string LastOpened;

		UserProjectInfo()
		{
			this->LastOpened = "none";
			this->AbsulutePath = "none";
			this->Name = "none";
		}
		UserProjectInfo(std::string name, std::string abspath, std::string lastOpened, std::string relativePath)
		{
			this->LastOpened = lastOpened;
			this->Name = name;
			this->AbsulutePath = abspath;
			this->relativePath = relativePath;

		}
	};
	struct ScaleDott
	{
		spe::Rectangle* ptr_ScalingRec;
		bool Clicked;
	};
}