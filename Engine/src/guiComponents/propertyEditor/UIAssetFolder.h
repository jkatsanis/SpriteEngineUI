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
#define FOLDER_HIERARCHY_PADDING 32
#define UIASSET_FOLDER_WIDTH 200
#define FILE_DISPLAYER_COLOR 30.0f

#define MAX_COLUMNS 9
#define PADDING_BETWEEN_ROWS 20
#define PADDING_BETWEEN_COLUMS 130
#define ICONS_SIZE 70
#define ASSET_FOLDER_DEFAULT_WINDOW_SIZE ImVec2(1280 + 250, 400)

namespace s2d
{		
	class UIAssetFolder
	{
	private:
		std::string m_to_hover_item_name;
		bool m_is_item_hovered;

		bool m_is_asset_folder_tree_node_open;

		s2d::SpriteRepository* m_ptr_repo;
		s2d::UIAssetTools m_tools;
		s2d::UIIconData m_data;
		std::string m_current_path;
		std::string m_current_name;
		ImVec2 m_window_size;
		float m_file_content_padding;

		ImGuiTextFilter m_file_filter;

		bool m_hovered_over_item;
		bool m_dragging_item;
		bool m_interacted;
		bool m_clicked_resize_button;

		void render();
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
		void init();

	public:
		bool is_hovered;

		UIAssetFolder();

		void createAssetLinkerWindow();

		void setSpriteRepository(s2d::SpriteRepository& repo) { this->m_ptr_repo = &repo; }
		const ImVec2* getSizePtr() { return &this->m_window_size; }
	};
}

