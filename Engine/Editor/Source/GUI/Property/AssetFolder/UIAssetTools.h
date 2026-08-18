#pragma once

#include <iostream>
#include <string>
#include "thread"

#include "UtilityFunctions.h"
#include "GUI/UIUtility/UIUtility.h"
#include "RessourceHandler/FileDataMacros.h"
#include "Input/Input.h"

#define CPP_FILE_NAME_SIZE 80
#define INPUT_POPUP_NAME "inputPopup"

namespace spe
{
	class UIAssetTools
	{
	private:	
		const std::string* m_ptr_CurrentAssetPath;

	    /// <summary>
	    /// Need to change value on delete
	    /// </summary>
	    std::string* m_ptr_HoveredIconName;
		char m_ClassFileName[CPP_FILE_NAME_SIZE];

		void GetFileName();
		void CreateFileContent();

		void GetFolderFileName();

		bool m_IsPopupOpen;
		bool m_OpenFileInput;
		float m_WindowFontSize;

		bool m_OpenFolderInput;
		char m_FolderFileName[CPP_FILE_NAME_SIZE];

	public:
		UIAssetTools();
		UIAssetTools(const std::string* current_asset_path, std::string* hovered_icon_name);

		void Update(bool& hovered);

		bool IsPopUpOpen() const { return this->m_IsPopupOpen; }
	};
}

