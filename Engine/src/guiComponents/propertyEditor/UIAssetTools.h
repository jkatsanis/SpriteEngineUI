#pragma once

#include <iostream>
#include <ImGui.h>

namespace s2d
{
	class UIAssetTools
	{
	private:
		const std::string m_POPUP_NAME = "popUp";


	public:
		UIAssetTools();

		void update(bool& hovered);
	};
}

