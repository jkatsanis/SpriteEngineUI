#pragma once

#include <ImGui.h>
#include <dirent.h> 
#include <physicalComponents/sprite.h>
#include <data/fileData.h>
#include <data/UIInfo.h>
#include <array>
#include <data/UIIconData.h>
#include <UIAssetTools.h>

namespace s2d
{		
	// The amount of icons the Engine has
	const int amountOfIcons = 3;

	class UIAssetFolder
	{
	private:
		s2d::UIAssetTools m_tools;
		s2d::UIIconData m_data;
		std::string currentPath;
		std::string currentName;
		int m_iconSize;
		int m_padding;

		std::string m_currentDeletePath;

		bool m_hoveredOverItem;
		bool m_draggingItem;
		bool m_interacted;

		void render();
		void getAllFilesInDir(const char* path, const char* name);
		void setDragAndDrop(std::string path, std::string name);
		void beginColumns();
		void goBackToBeforeFolder();

	public:
		bool isHovered;

		UIAssetFolder();

		void createAssetLinkerWindow();

		//THIS IS THE PATH FROM THE .SLN file in the engine!
		static std::string dragAndDropPath;
		static std::string dragAndDropName;
	};
}

