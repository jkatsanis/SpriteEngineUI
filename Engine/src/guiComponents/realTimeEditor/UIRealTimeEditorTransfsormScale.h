#pragma once

#include <SFML/Graphics.hpp>
#include <guiComponents/UI.h>
#include <guiComponents/UIModels.h>

#define SCALE_DOTTS 2
#define DEFAULT_DOTT_SCALE 35

namespace s2d
{
	class UIRealTimeEditorTransfsormScale
	{
	private:
		ScaleDott m_scale_dotts[SCALE_DOTTS];

		s2d::SpriteRepository* m_ptr_repo;
		s2d::GUIRepository* m_ptr_gui_repo;
		s2d::Vector2 m_scale;
		s2d::Vector2 m_currentCursorPos;
		s2d::Event* m_event;

		void scaleChanger(s2d::Sprite* focusedSprite);

		void reset();
		void setPos(const sf::Vector2f pos[]);
		void getPos(const s2d::Sprite* focusedSprite, sf::Vector2f pos[]);

	public:
		UIRealTimeEditorTransfsormScale();
		UIRealTimeEditorTransfsormScale(s2d::Event* event, s2d::SpriteRepository& repo, s2d::GUIRepository& gui_repo);

		void renderDolls();
		void unrenderDolls();

		void update();
	};
}

