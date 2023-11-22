#pragma once

#include "Core/GameWindow.h"
#include "UtilityFunctions.h"
#include "RessourceHandler/FileDataMacros.h"


#include "SpriteRepository.h"
#include "RessourceHandler/Initializer.h"

#include <iostream>

namespace spe
{
	class Editor
	{
	private:
		spe::GameWindow m_Window;
		spe::SpriteRepository m_SpriteRepository;


	public:
		Editor();

		void Update();
		bool IsOpen() { return true; }
	};
}