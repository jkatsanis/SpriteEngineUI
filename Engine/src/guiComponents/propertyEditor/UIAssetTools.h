#pragma once

#include <iostream>
#include <calculationComponents/vector2.h>
#include <ImGui.h>
#include <guiComponents/UI.h>

#define CPP_FILE_NAME_SIZE 80
#define INPUT_POPUP_NAME "inputPopup"

namespace s2d
{
	class UIAssetTools
	{
	private:	
		const std::string* m_ptr_currentAssetPath;
		const std::string* m_ptr_deleteFilePath;
		char m_cppFileName[CPP_FILE_NAME_SIZE];

		void getFileName();
		void createFileContent();

		bool m_isPopUpHoverd;
		bool m_openFileInput;
	public:
		UIAssetTools();
		UIAssetTools(const std::string* currentAssetPath, const std::string* deleteItem);

		void update(bool& hovered);

	public:
		bool isPopUpHovered() const { return this->m_isPopUpHoverd; }
	};
}

