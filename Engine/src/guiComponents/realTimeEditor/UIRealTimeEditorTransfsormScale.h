#pragma once

#include <SFML/Graphics.hpp>
#include <physicalComponents/gameObject.h>
#include <UIHirachy.h>
#include <guiComponents/UI.h>

#define SCALE_DOTTS 4
#define DEFAULT_DOLL_SCALE 35

namespace s2d
{
	struct ScaleDott 
	{
		sf::RectangleShape shape;
		short vecpos;
		bool clicked;
	};
	class UIRealTimeEditorTransfsormScale
	{
	private:
		ScaleDott m_scaleDotts[SCALE_DOTTS];

		s2d::Vector2 m_scale;
		s2d::Vector2 m_currentCursorPos;
		s2d::Event* m_event;

		void scaleChanger(s2d::Sprite* focusedSprite);
		void xScaleChanger(s2d::Sprite* focusedSprite, ScaleDott& dott);
		void yScaleChanger(s2d::Sprite* focusedSprite, ScaleDott& dott);

		void reset();
		void setPos(const sf::Vector2f pos[]);
		void getPos(const s2d::Sprite* focusedSprite, sf::Vector2f pos[]);

	public:
		UIRealTimeEditorTransfsormScale();
		UIRealTimeEditorTransfsormScale(s2d::Event* event);

		void renderDolls();
		void unrenderDolls();

		void update();
	};
}

