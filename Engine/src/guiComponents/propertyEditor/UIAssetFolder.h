#pragma once

#include <ImGui.h>
#include <dirent.h> 
#include <physicalComponents/sprite.h>
#include <data/fileData.h>
#include <data/UIInfo.h>
#include <array>
#include <data/UIIconData.h>
#include <UIAssetTools.h>
#include <manager/saveSystem.h>
#include <manager/fontManager.h>
#include <icons.h>
#include <manager/fileDialogs.h>

#define CLOSE_RECTANGLE_PADDIND_X 50
#define FOLDER_HIERACHY_PADDING 32
#define UIASSET_FOLDER_WIDTH 200
#define FILE_DISPLAYER_COLOR 30.0f
#define CLOSE_RECTANGLE_SIZE ImVec2(150, FOLDER_HIERACHY_PADDING)

#define CLOSE_RECTANGLE_INNER_PADDING 7
#define MAX_COLUMNS 9
#define PADDING_BETWEEN_ROWS 20
#define PADDING_BETWEEN_COLUMS 130
#define ICONS_SIZE 70

namespace s2d
{		
	// The amount of icons the Engine has
	const int amountOfIcons = 3;

	class UIAssetFolder
	{
	private:
		std::string m_toHoverItemName;
		bool m_isItemHovered;

		bool m_isAssetFolderTreeNodeOpen;

		s2d::SpriteRepository* m_ptr_repo;
		s2d::UIAssetTools m_tools;
		s2d::UIIconData m_data;
		std::string currentPath;
		std::string currentName;
		ImVec2 m_windowSize;
		float m_fileContentPadding;

		ImGuiTextFilter m_fileFilter;

		bool m_hoveredOverItem;
		bool m_draggingItem;
		bool m_interacted;
		bool m_clickedOnResizeButton;
		void render();
		void getAllFilesInDir(const char* path, const char* name);
		void setDragAndDrop(std::string path, std::string name);
		void beginColumns();
		void goBackToBeforeFolder();
		void addPrefab();
		void renderFolderHierarchy();
		void renderCloseRectangle();

		void renderFolderHierarchyRecursiv(const char* path, const char* name, bool openNextTreeNode);

		void renderFilesWithChildWindow(const std::string& name, const std::string& newPath, const std::string& entryPath, bool isFolder, uint32_t textureId, uint8_t columCnt);

		/// <summary>
		/// Renders the selected path (NOT RECURSIV)
		/// </summary>
		void renderContentBrowser();

		void setCurrentPath(const std::string& path, const std::string& name);

		void resizeWindow();
		void init();

	public:
		bool isHovered;

		UIAssetFolder();

		void createAssetLinkerWindow();

		void setSpriteRepository(s2d::SpriteRepository& repo) { this->m_ptr_repo = &repo; }
		const ImVec2* getSizePtr() { return &this->m_windowSize; }
	};
}

