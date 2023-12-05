#pragma once

#include "Core/GameWindow.h"
#include "RessourceHandler/Initializer.h"
#include "Core/IApplication.h"
#include "Core/SeceneHandler.h"
#include "Utility/Style.h"

#include "Scripts/Game.h"

namespace spe
{
	class Engine : public spe::IAppliaction
	{
	private:
		// 1 Instance of the user game class
		Game m_Game;

		spe::GameWindow m_Window;
		spe::Camera m_Camera;
		spe::Vector3 m_BackgroundColor;

		spe::SceneHandler m_SceneHandler;

		void UpdateComponents() override;

		void Init() override;

	public:
		Engine();
		~Engine();

		void Update() override;
		bool IsOpen() const { return this->m_Window.IsOpen(); }
	};
}