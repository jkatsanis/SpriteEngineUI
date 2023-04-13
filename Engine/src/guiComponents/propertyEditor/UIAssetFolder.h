#pragma once

#include <ImGui.h>
#include <dirent.h> 
#include <physicalComponents/sprite.h>
#include <data/fileData.h>
#include <data/UIInfo.h>
#include <array>
#include <data/UIIconData.h>
#include <UIAssetTools.h>
#include <UIHirachy.h>
#include <manager/saveSystem.h>

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

		bool m_hoveredOverItem;
		bool m_draggingItem;
		bool m_interacted;

	    s2d::Sprite* m_ptr_rightClickedSprite;

		void render();
		void getAllFilesInDir(const char* path, const char* name);
		void setDragAndDrop(std::string path, std::string name);
		void beginColumns();
		void goBackToBeforeFolder();
		void addPrefab();

		void init();

	public:
		bool isHovered;

		UIAssetFolder();

		void createAssetLinkerWindow();

		void setRightClickedSprite(s2d::Sprite* clicked) { this->m_ptr_rightClickedSprite = clicked; }

		//THIS IS THE PATH FROM THE .SLN file in the engine!
		static std::string dragAndDropPath;
		static std::string dragAndDropName;
	};
}

