#pragma once

#include "GUI/Property/UIBase.h"
#include "GUI/UIUtility/UIModels.h"
#include "Sprite/Sprite.h"
#include "Utility/Style.h"
#include "icons.h"
#include "GUI/UIUtility/UIUtility.h"


#define SCALE_DOTTS_COLLIDER 4
#define INVALID_AERA 1
#define DEFAULT_DOTT_SCALE 35

namespace spe
{
	class UIInspectorBoxCollider
	{
	private:	
		spe::ScaleDott m_ScaleDotts[SCALE_DOTTS_COLLIDER];
		bool m_EditMode;

		/// <summary>
		///  Hardcoded to much shit
		/// </summary>
		void RenderScaleDotts(spe::Sprite* sprite);
		void RenderDotts();
		void UnrenderDotts();
		void Reset();
	public:
		UIInspectorBoxCollider();

		void LeaveEditMode();
		void Edit(float& x, float& y);
		void Solid(float& x, float& y, spe::Sprite* sprite);
		void Width(float x, float y, spe::Sprite* sprite);
		void Height(spe::Sprite* sprite);
		void DrawBoxCollider(spe::Sprite* sprite, spe::Rectangle* ptr_rectangle);

		void InitScaleDottsUI(spe::GUIRepository& repo);
	};
}