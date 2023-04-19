#pragma once

#include <iostream>
#include <calculationComponents/vector2.h>
#include <ImGui.h>
#include <guiComponents/UI.h>
#include <data/UIInfo.h>
#include <string>

#define CPP_FILE_NAME_SIZE 80
#define INPUT_POPUP_NAME "inputPopup"

namespace s2d
{
	class UIAssetTools
	{
	private:	
		const std::string* m_ptr_currentAssetPath;

	    /// <summary>
	    /// Need to change value on delete
	    /// </summary>
	    std::string* m_ptr_hoveredIconName;
		char m_classFileName[CPP_FILE_NAME_SIZE];

		void getFileName();
		void createFileContent();

		bool m_isPopUpOpen;
		bool m_openFileInput;
		float m_windoFontSize;
	public:
		UIAssetTools();
		UIAssetTools(const std::string* currentAssetPath, std::string* hoveredIconName);

		void update();
		void includeInUserProject(const std::string& fileName);

	public:
		bool isPopUpOpen() const { return this->m_isPopUpOpen; }
	};
}

