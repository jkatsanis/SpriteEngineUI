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
		bool m_OpenNamePopup;
		char m_TagName[150];

		void RenderSelector();
		void RenderPopup();

		void Init() override;

	public:
		UITagSelector();

		void Render() override;
	};
}

