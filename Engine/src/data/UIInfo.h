#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <ImGui.h>
#include <manager/initializer.h>

#define ICONS_AMOUNT 5

namespace s2d
{
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

		const static std::string s_icons[ICONS_AMOUNT];
	};
}

