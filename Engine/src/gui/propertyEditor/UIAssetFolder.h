#pragma once

#include <ImGui.h>
#include <dirent.h> 
#include <sprite.h>
#include <fileData.h>
#include <UIInfo.h>
#include <array>
#include <UIIconData.h>

namespace s2d
{		
	// The amount of icons the Engine has
	const int amountOfIcons = 3;

	class UIAssetFolder
	{
	private:
		s2d::UIIconData data;

		std::string currentPath;
		std::string currentName;
		int m_iconSize;
		int m_padding;

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

