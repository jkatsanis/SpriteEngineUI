#pragma once

#include "Property/UIBase.h"
#include "UIUtility/UIUtility.h"

namespace spe
{
	class UIConsole : public spe::IUIBase
	{
	private:

		void Init() override;
	public:
		std::string Buffer;
		
		UIConsole();

		void Render() override;
	};
}