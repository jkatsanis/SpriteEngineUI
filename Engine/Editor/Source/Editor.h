#pragma once

#include "Core/GameWindow.h"
#include "UtilityFunctions.h"
#include "RessourceHandler/FileDataMacros.h"


#include "SpriteRepository.h"
#include "RessourceHandler/Initializer.h"
#include "Core/IApplication.h"

#include <iostream>

namespace spe
{
	class Editor : public spe::IAppliaction
	{
	private:
		spe::GameWindow m_Window;
		spe::SpriteRepository m_SpriteRepository;

		void UpdateComponents() override;

	public:
		Editor();

		void Update() override;
		bool IsOpen() { return true; }
	};
}