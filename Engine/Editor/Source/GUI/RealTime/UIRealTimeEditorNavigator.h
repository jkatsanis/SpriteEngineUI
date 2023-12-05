#pragma once

#include <SFML/Graphics.hpp>

#include "Core/GameWindow.h"
#include "GUI/Property/UIBase.h"
#include "Input/Input.h"
#include "GUI/UIUtility/UITransform.h"
#include "RessourceHandler/FileDataMacros.h"
#include "GUI/UIUtility/UIUtility.h"

namespace spe
{
	class UIRealTimeEditorNavigator : public spe::IUIBase
	{
	private:
		spe::UITransform m_Cursor;

		float m_ScrollSpeed;
		spe::GameWindow* m_ptr_Window;

		void NavigateRightClick();
		void NavigateScrollWheel();
		void NavigateArrows();
		void NavigateKeys();

		void CalculateScrollWheelSpeed();

		void SetChangedPosition();

		void Init() override;
	public:	

		void SetGameWindow(spe::GameWindow* window) { this->m_ptr_Window = window; }
		void Render() override;
	};
}

