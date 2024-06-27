#pragma once

#include "Property/UIBase.h"
#include "Sprite/Sprite.h"
#include "GUI/GUIRepository.h"
#include "GUI/UIUtility/UIUtility.h"
#include "Utility/Style.h"
#include "GUI/UIUtility/UIModels.h"
#include "icons.h"
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

		spe::Tool m_Tools[TOOLS_AMOUNT];
		spe::Vector3 m_WindowBackground;
		EditorTools m_EditorTools;

		char m_NewSceneName[150];

		bool m_DisplayInfo;
		bool m_AddSceneMode;
		bool m_ClickedOnBtn;
		bool m_UpdateEventToSet;
		std::string m_SwitchSceneName;

		void CreateScene();
		void PlayGameButton();
		void ToolSelector();
		void RemoveBackgroundFromButtons();
		void BuildProjectIntoFolder();
		void Hotkeys();
		void RenderWindowSelecter();
		void RenderMainMenuBar();
		void RenderToolSelector();
		void RenderSceneSelector();
		void RemoveScene(const std::string& scene);
		void RenderSceneAddPopup();
		void SwitchScene(const std::string& scene);
		void DisplayEngineInfo();

		void ProjectSettings();
		void SimulateButton();

	public:
		UITopbar();

		void Render() override;

		void SetSceneHandler(spe::SceneHandler& scene) { this->m_ptr_SceneHandler = &scene; }

	};
}