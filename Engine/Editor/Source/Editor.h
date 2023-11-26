#pragma once

#include "Core/GameWindow.h"
#include "UtilityFunctions.h"
#include "RessourceHandler/FileDataMacros.h"
#include "Input/Input.h"


#include "GUI/GUIRepository.h"
#include "SpriteRepository.h"
#include "RessourceHandler/Initializer.h"
#include "Core/IApplication.h"
#include "Core/SeceneHandler.h"
#include "GUI/UIUtility/UIUtility.h"
#include "GUI/RealTime/UIRealTimeEditor.h"

#include "GUI/UIWindow.h"

#include <ImGUISFML.h>
#include <iostream>

namespace spe
{
	class Editor : public spe::IAppliaction
	{
	private:
		spe::GameWindow m_Window;
		spe::GUIRepository m_GUIRepository;
		spe::SceneHandler m_SceneHandler;
		spe::UIWindow m_UIWindow;
		spe::UIRealTimeEditor m_UIRealTimeEditor;

		void UpdateUI();
		void UpdateComponents() override;
		void Init() override;
	public:
		Editor();
		~Editor();

		void Update() override;
		bool IsOpen() { return m_Window.IsOpen(); }
	};
}