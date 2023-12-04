#pragma once

#include "GUI/Property/UIBase.h"
#include "GUI/UIUtility/UIModels.h"
#include "Sprite/Sprite.h"
#include "Utility/Style.h"
#include "Include/icons.h"
#include "GUI/UIUtility/UIUtility.h"


#define SCALE_DOTTS_COLLIDER 4
#define INVALID_AERA 1
#define DEFAULT_DOTT_SCALE 35

namespace spe
{
	class UIInspectorBoxCollider
	{
	private:	
		spe::ScaleDott m_box_collider_scale_dotts[SCALE_DOTTS_COLLIDER];
		bool m_edit_mode;

		/// <summary>
		///  Hardcoded to much shit
		/// </summary>
		void renderScaleDotts(spe::Sprite* sprite);
		void renderDotts();
		void unrenderDotts();
		void reset();
	public:
		UIInspectorBoxCollider();

		void leaveEditMode();
		void edit(float& x, float& y);
		void solid(float& x, float& y, spe::Sprite* sprite);
		void width(float x, float y, spe::Sprite* sprite);
		void height(spe::Sprite* sprite);
		void drawBoxCollider(spe::Sprite* sprite, spe::Rectangle* ptr_rectangle);

		void initScaleDottsUI(spe::GUIRepository& repo);
	};
}