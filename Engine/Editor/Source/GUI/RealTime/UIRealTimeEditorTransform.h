#pragma once

#include "GUI/Property/UIBase.h"
#include "Core/GameWindow.h"
#include "GUI/UIUtility/UIUtility.h"
#include "GUI/UIUtility/UIModels.h"

#define SCALE_DOTTS 2
#define DEFAULT_DOTT_SCALE 35

namespace spe
{
	class UIRealTimeEditorTransform : public spe::IUIBase
	{
	private:
		bool m_RealeasedCursorOnSprite;
		spe::GameWindow* m_ptr_Window;
		spe::Vector2 m_CursorWorldPos;
		ScaleDott m_ScaleDotts[SCALE_DOTTS];
		spe::Vector2 m_CurrentCursorPos;

		spe::Sprite* m_ptr_ClickedSprite;
		int m_ClickedSpriteId;

		void MoveComponent();

		bool CheckClick(spe::Sprite* const sprite);
		spe::Sprite* CheckIfMouseClickedOnSprite();

		void ScaleChanger(spe::Sprite* focusedSprite);

		void Reset();
		void SetPos(const sf::Vector2f pos[]);
		void GetPos(const spe::Sprite* focusedSprite, sf::Vector2f pos[]);

		void RenderDolls();
		void RnrenderDolls();

		void Init() override;
	public: 
		void SetGameWindow(spe::GameWindow* ptr) { this->m_ptr_Window = ptr; }
		
		void Render() override;	
	};
}
