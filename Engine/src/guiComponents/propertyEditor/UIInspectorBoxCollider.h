#pragma once

#include <physicalComponents/sprite.h>
#include <ImGui.h>
#include <icons.h>
#include <UIAssetFolder.h>
#include <manager/fontManager.h>
#include <UIRealTimeEditorTransfsormScale.h> 

namespace s2d
{
	class UIInspectorBoxCollider
	{
	private:	
		s2d::ScaleDott m_box_collider_scale_dotts[SCALE_DOTTS];
		bool m_edit_mode;

		void renderScaleDotts(s2d::Sprite* sprite, s2d::Rectangle* ptr_box_collider_rec);

		void getPos(const s2d::Sprite* focusedSprite, sf::Vector2f pos[]);
		void setPos(const sf::Vector2f pos[]);
	public:
		UIInspectorBoxCollider();

		void edit(float& x, float& y);
		void solid(float& x, float& y, s2d::Sprite* sprite);
		void width(float x, float y, s2d::Sprite* sprite);
		void height(s2d::Sprite* sprite);
		void drawBoxCollider(s2d::Sprite* sprite, s2d::Rectangle* ptr_rectangle);
		void initScaleDottsUI(s2d::GUIRepository& repo);
	};
}