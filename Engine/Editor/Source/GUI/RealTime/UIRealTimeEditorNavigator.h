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
		spe::UITransform m_cursor;

		float m_scroll_speed;
		spe::GameWindow* m_ptr_Window;

		void navigateRightClick();
		void navigateScrollWheel();
		void navigateArrows();
		void navigateKeys();

		void calculateScrollWheelSpeed();

		void setChangedPosition();

		void Init() override;
	public:	

		void SetGameWindow(spe::GameWindow* window) { this->m_ptr_Window = window; }
		void Render() override;
	};
}

