#pragma once

#include "Property/UIBase.h"
#include "Sprite/Sprite.h"
#include "GUI/GUIRepository.h"
#include "GUI/UIUtility/UIUtility.h"
#include "Utility/Style.h"
#include "GUI/UIUtility/UIModels.h"
#include "Include/icons.h"
#include "GUI/Color.h"
#include "Core/SeceneHandler.h"
#include "Input/Input.h"
#include "UIUtility/UIModels.h"

#include "Savesystem.h"

#define TOOLS_AMOUNT 2
#define FILE_AMOUNT 6

#define WINDOW_SIZE_Y_TOOL_BUTTONS 20.0f

namespace spe
{
	class UITopbar : public IUIBase
	{
	private:
		spe::SceneHandler* m_ptr_SceneHandler;

		void Init() override;

		spe::Tool m_tools[TOOLS_AMOUNT];
		spe::Vector3 m_window_background_to_save;
		EditorTools m_editor_tools;

		char m_new_scene_name[150];

		bool m_display_engine_info;
		bool m_add_scene_mode;
		bool m_clicked_on_btn;
		bool m_update_event_to_set;
		std::string m_switch_scene_name;

		void createScene();
		void playGameButton();
		void toolSelector();
		void removeBackgroundFromButtons();
		void buildProjectIntoFolder();
		void hotkeys();
		void renderWindowSelecter();
		void renderMainMenuBar();
		void renderToolSelector();
		void renderSceneSelector();
		void removeScene(const std::string& scene);
		void renderSceneAddPopup();
		void switchScene(const std::string& scene);
		void displayEngineInfo();

	public:

		void Render() override;

		void SetSceneHandler(spe::SceneHandler& scene) { this->m_ptr_SceneHandler = &scene; }

	};
}