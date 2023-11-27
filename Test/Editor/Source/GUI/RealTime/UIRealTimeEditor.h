#pragma once

#include "GUI/RealTime/UIRealTimeEditorNavigator.h"
#include "GUI/RealTime/UIRealTimeEditorTransform.h"
#include "Core/GameWindow.h"

namespace spe
{
	class UIRealTimeEditor
	{
	private:
		spe::UIRealTimeEditorNavigator m_navigator;
		spe::UIRealTimeEditorTransform m_transformChanger;

	public: 

		UIRealTimeEditor();
		UIRealTimeEditor(spe::GameWindow* windowm, spe::SpriteRepository& spriteRepo, spe::GUIRepository& gui_rep);

		void update();	
	};
}
