#pragma once

#include "GUI/Property/UIBase.h"
#include "Core/GameWindow.h"
#include "GUI/UIUtility/UIUtility.h"

namespace spe
{
	class UIRealTimeEditorTransform : public spe::IUIBase
	{
	private:
		bool m_realeasedCursorOnSprite;
		spe::GameWindow* m_ptr_Window;
		spe::Vector2 m_cursorWorldPos;

		spe::Sprite* m_clickedSprite;
		int m_clickedSpriteId;

		void moveComponent();

		bool checkClick(spe::Sprite* const sprite);
		spe::Sprite* checkIfMouseClickedOnSprite();

		void Init() override;

	public: 
		void SetGameWindow(spe::GameWindow* ptr) { this->m_ptr_Window = ptr; }
		
		void Render() override;	
	};
}
