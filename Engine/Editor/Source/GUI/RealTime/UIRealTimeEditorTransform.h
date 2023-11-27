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
		bool m_realeasedCursorOnSprite;
		spe::GameWindow* m_ptr_Window;
		spe::Vector2 m_cursorWorldPos;
		ScaleDott m_scale_dotts[SCALE_DOTTS];
		spe::Vector2 m_currentCursorPos;

		spe::Sprite* m_clickedSprite;
		int m_clickedSpriteId;

		void moveComponent();

		bool checkClick(spe::Sprite* const sprite);
		spe::Sprite* checkIfMouseClickedOnSprite();

		void scaleChanger(spe::Sprite* focusedSprite);

		void reset();
		void setPos(const sf::Vector2f pos[]);
		void getPos(const spe::Sprite* focusedSprite, sf::Vector2f pos[]);

		void renderDolls();
		void unrenderDolls();

		void Init() override;
	public: 
		void SetGameWindow(spe::GameWindow* ptr) { this->m_ptr_Window = ptr; }
		
		void Render() override;	
	};
}
