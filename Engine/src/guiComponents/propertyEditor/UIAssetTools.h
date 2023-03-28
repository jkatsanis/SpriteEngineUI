#pragma once

#include <iostream>
#include <calculationComponents/vector2.h>
#include <ImGui.h>
#include <guiComponents/UI.h>
#include <string>

#define CPP_FILE_NAME_SIZE 80
#define INPUT_POPUP_NAME "inputPopup"

namespace s2d
{
	class UIAssetTools
	{
	private:	
		const std::string* m_ptr_currentAssetPath;
		char m_classFileName[CPP_FILE_NAME_SIZE];

		void getFileName();
		void createFileContent();

		bool m_isPopUpHoverd;
		bool m_openFileInput;
	public:
		UIAssetTools();
		UIAssetTools(const std::string* currentAssetPath);

		void update(bool& hovered);
		void includeInUserProject(const std::string& fileName);

	public:
		bool isPopUpHovered() const { return this->m_isPopUpHoverd; }
	};
}

