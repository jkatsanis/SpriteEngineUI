#pragma once

#include <iostream>
#include <ImGui.h>
#include <string>

#include "UtilityFunctions.h"
#include "GUI/UIUtility/UIUtility.h"
#include "RessourceHandler/FileDataMacros.h"

#define CPP_FILE_NAME_SIZE 80
#define INPUT_POPUP_NAME "inputPopup"

namespace spe
{
	class UIAssetTools
	{
	private:	
		const std::string* m_ptr_currentAssetPath;

	    /// <summary>
	    /// Need to change value on delete
	    /// </summary>
	    std::string* m_ptr_hovered_icon_name;
		char m_classFileName[CPP_FILE_NAME_SIZE];

		void getFileName();
		void createFileContent();

		bool m_is_popup_open;
		bool m_open_file_input;
		float m_window_font_size;
	public:
		UIAssetTools();
		UIAssetTools(const std::string* current_asset_path, std::string* hovered_icon_name);

		void update();

	public:
		bool isPopUpOpen() const { return this->m_is_popup_open; }
	};
}

