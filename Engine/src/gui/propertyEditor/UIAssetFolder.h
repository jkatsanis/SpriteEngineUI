#pragma once

#include <ImGui.h>
#include <dirent.h> 
#include <sprite.h>
#include <fileData.h>
#include <UIInfo.h>

namespace s2d
{
	class UIAssetFolder
	{
	private:
		std::vector<s2d::FileData*> m_filesInAssets;
		bool m_hoveredOverItem;
		bool m_draggingItem;
		bool m_interacted;

		void render();
		void getAllFilesInDir(const char* path, const char* name, std::vector<s2d::FileData*>& vec);
		void setDragAndDrop(std::string path);

	public:
		bool isHovered;

		UIAssetFolder();
		~UIAssetFolder();

		void createAssetLinkerWindow();

		//THIS IS THE PATH FROM THE .SLN file in the engine!
		static std::string dragAndDropPath;
	};
}

