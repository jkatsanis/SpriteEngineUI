#pragma once

#include <iostream>
#include <ImGui.h>
#include <Include/UtilityFunctions.h>
#include <Include/SFML/Graphics.hpp>
#include "dirent.h"

#include "RessourceHandler/FileDataMacros.h"
#include "GUI/UIUtility/UIModels.h"
#include "GUI/Property/UIBase.h"
#include "Utility/Style.h"
#include "GUI/UIUtility/UIUtility.h"
#include "Core/Time.h"
#include "GUI/Color.h"
#include "Utility/FileDialoge.h"

#include "Sprite/Sprite.h"
#include "GUI/Property/AssetFolder/UIAssetTools.h"
#include "GUI/Property/AssetFolder/UIIconData.h"
#include "GUI/Property/AssetFolder/UIAssetFolder.h"

#include "Savesystem.h"

#define CLOSE_RECTANGLE_PADDIND_X 50
#define FOLDER_HIERARCHY_PADDING 32
#define UIASSET_FOLDER_WIDTH 200
#define FILE_DISPLAYER_COLOR 30.0f

#define MAX_COLUMNS 9
#define PADDING_BETWEEN_ROWS 20
#define PADDING_BETWEEN_COLUMS 130
#define ICONS_SIZE 70
#define ASSET_FOLDER_DEFAULT_WINDOW_SIZE ImVec2(1280 + 250, 400)

namespace spe
{
	class UIAssetFolder : public spe::IUIBase
	{
	private:
		spe::ResizeWindowData m_ResizeWindow;
		spe::UIAssetTools m_Tools;
		spe::UIIconData m_IconData;

		std::string m_HoverItemName;
		bool m_IsItemHovered;
		bool m_IsAssetFolderOpen;
		std::string m_CurrentPath;
		std::string m_CurrentName;
		float m_FileContentPadding;
		ImGuiTextFilter m_FileFilter;
		bool m_HoveredOverItem;
		bool m_DraggingItem;
		bool m_Interacted;

		std::unordered_map<std::string, DIR*> m_AlreadyOpenedPaths;

		void GetAllFilesInDir(const char* path);
		void SetDragAndDrop(std::string path, std::string name);
		void BeginColumns();
		void GoBackToBeforeFolder();
		void AddPrefab();
		void RenderFolderHierarchy();
		void RenderCloseRectangle();
		void RenderFolderHierarchyRecursiv(const char* path, const char* name, bool open_next_tree_node);
		void RenderFilesWithChildWindow(const std::string& name, const std::string& new_path, const std::string& entry_path
			, bool is_folder, uint32_t texture_id, uint8_t colum_cnt);

		/// <summary>
		/// Renders the selected path (NOT RECURSIV)
		/// </summary>
		void RenderContentBrowser();
		void SetCurrentPath(const std::string& path, const std::string& name);
		void ResizeWindow();
		void Init() override;

	public:
		UIAssetFolder();

		void Render() override;
	};
}
