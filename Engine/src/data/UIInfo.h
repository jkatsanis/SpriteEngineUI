#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <ImGui.h>
#include <manager/initializer.h>

#define ICONS_AMOUNT 5

#define DEFAULT_WINDOW_FLAGS ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar


namespace s2d
{
	struct ResizeWindowData 
	{
		float additinal_add;
		bool clicked_on_resize_button;
	};
	struct OpenWindow
	{
		bool reload;
		bool is_open;

		OpenWindow()
		{
			this->reload = true;
			this->is_open = true;
		}

		void setOpen()
		{
			reload = true;
			is_open = true;
		}
	};
	class UIInfo
	{
		UIInfo() = delete;

	public:
		static float s_default_font_size;
		static bool s_render_asset_folder;

		static OpenWindow s_is_asset_folder_open;
		static OpenWindow s_is_hierarchy_open;
		static OpenWindow s_is_inspector_open;

		const static std::string s_icons[ICONS_AMOUNT];
	};
}

