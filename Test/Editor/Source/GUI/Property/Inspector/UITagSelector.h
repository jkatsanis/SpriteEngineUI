#pragma once

#include "GUI/UIUtility/UIModels.h"
#include "GUI/UIUtility/UIUtility.h"
#include "GUI/Property/UIBase.h"

#include "Sprite/Sprite.h"

#include <ImGui.h>

namespace spe
{
	class UITagSelector : public spe::IUIBase
	{
	private:
		bool m_open_name_popup;
		char m_tag_name[150];

		void renderSelector();
		void renderPopup();

		void Init() override;

	public:
		UITagSelector();

		void Render() override;
	};
}

