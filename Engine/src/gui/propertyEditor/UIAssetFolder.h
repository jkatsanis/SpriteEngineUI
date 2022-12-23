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
		sf::Texture texture;
		ImTextureID textureId;

		std::string currentPath;
		std::string currentName;
		int m_iconSize;
		int m_padding;

		std::vector<s2d::FileData*> m_filesInAssets;
		bool m_hoveredOverItem;
		bool m_draggingItem;
		bool m_interacted;

		void render();
		void getAllFilesInDir(const char* path, const char* name, std::vector<s2d::FileData*>& vec);
		void setDragAndDrop(std::string path);
		void beginColumns();
		void goBackToBeforeFolder();

	public:
		bool isHovered;

		UIAssetFolder();
		~UIAssetFolder();

		void createAssetLinkerWindow();

		//THIS IS THE PATH FROM THE .SLN file in the engine!
		static std::string dragAndDropPath;
	};
}

