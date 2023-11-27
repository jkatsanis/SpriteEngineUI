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
		spe::ResizeWindowData m_resize_window_data;
		spe::UIAssetTools m_tools;
		spe::UIIconData m_data;

		std::string m_to_hover_item_name;
		bool m_is_item_hovered;
		bool m_is_asset_folder_tree_node_open;
		std::string m_current_path;
		std::string m_current_name;
		float m_file_content_padding;
		ImGuiTextFilter m_file_filter;
		bool m_hovered_over_item;
		bool m_dragging_item;
		bool m_interacted;

		std::unordered_map<std::string, DIR*> m_AlreadyOpenedPaths;

		void getAllFilesInDir(const char* path, const char* name);
		void setDragAndDrop(std::string path, std::string name);
		void beginColumns();
		void goBackToBeforeFolder();
		void addPrefab();
		void renderFolderHierarchy();
		void renderCloseRectangle();
		void renderFolderHierarchyRecursiv(const char* path, const char* name, bool open_next_tree_node);
		void renderFilesWithChildWindow(const std::string& name, const std::string& new_path, const std::string& entry_path
			, bool is_folder, uint32_t texture_id, uint8_t colum_cnt);

		/// <summary>
		/// Renders the selected path (NOT RECURSIV)
		/// </summary>
		void renderContentBrowser();
		void setCurrentPath(const std::string& path, const std::string& name);
		void resizeWindow();
		void Init() override;

	public:
		void Render() override;
	};
}
