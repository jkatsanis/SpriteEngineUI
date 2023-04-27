#pragma once

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

		OpenWindow(bool open)
		{
			this->reload = true;
			this->is_open = open;
		}

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
}