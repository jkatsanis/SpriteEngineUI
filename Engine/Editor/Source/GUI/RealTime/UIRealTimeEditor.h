#pragma once

#include "GUI/RealTime/UIRealTimeEditorNavigator.h"
#include "GUI/RealTime/UIRealTimeEditorTransform.h"
#include "Core/GameWindow.h"

namespace spe
{
	class UIRealTimeEditor
	{
	private:
		spe::UIRealTimeEditorNavigator m_Navigator;
		spe::UIRealTimeEditorTransform m_TransformChanger;

	public: 
		UIRealTimeEditor();
		UIRealTimeEditor(spe::GameWindow* windowm, spe::SpriteRepository& spriteRepo, spe::GUIRepository& gui_rep);

		void Update();	
	};
}
