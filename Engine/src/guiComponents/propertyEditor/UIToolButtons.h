#pragma once

#include <engineComponents/input.h>
#include <ImGui.h>
#include <icons.h>
#include <SFML/Graphics.hpp>
#include <manager/saveSystem.h>
#include <data/UIInfo.h>
#include <manager/fontManager.h>
#include <data/engineData.h>
#include <Windows.h>
#include <guiComponents/colors.h>
#include <UIToolButtonsModels.h>
#include <filesystem>
#include <manager/saveSystem.h>
#include <manager/spriteRepository.h>
#include <manager/guiRepository.h>
#include <UIHierarchy.h>

#define TOOLS_AMOUNT 2
#define FILE_AMOUNT 6

#define WINDOW_SIZE_Y_TOOL_BUTTONS 15.0f

namespace s2d
{
	class UIToolButtons
	{
	private:
		s2d::Tool m_tools[TOOLS_AMOUNT];
		s2d::Vector3 m_window_background_to_save;
		EditorTools m_editor_tools;
		s2d::GUIRepository* m_ptr_gui_repo;
		s2d::SpriteRepository* m_ptr_repo;
		std::vector<std::string>* m_ptr_scene_names;
		
		char m_new_scene_name[150];

		s2d::EditorTools m_tool_to_set;

		bool m_add_scene_mode;
		bool m_clicked_on_btn;
		bool m_update_event_to_set;
		std::string m_switch_scene_name;

		void playGameButton();
		void toolSelector();
		void removeBackgroundFromButtons();
		void buildProjectIntoFolder();
		void hotkeys();
		void build();
		void init();
		void renderWindowSelecter();
		void renderMainMenuBar();
		void renderToolSelector();
		void renderSceneSelector();
		void removeScene(const std::string& scene);
		void renderSceneAddPopup(); 
		void switchScene(const std::string& scene);

	public:
		bool is_hovered;

		UIToolButtons();
		UIToolButtons(s2d::SpriteRepository& sprite_repo, std::vector<std::string>& scene_names);
		~UIToolButtons();

		void createToolsAndButtons();
		void setBackgroundColorToSave(const s2d::Vector3& color);
		void setGUIRepo(s2d::GUIRepository* repo) { this->m_ptr_gui_repo = repo; }
	};
}

