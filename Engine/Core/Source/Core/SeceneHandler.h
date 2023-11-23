#pragma once

#include <Include/SFML/Graphics.hpp>
#include <iostream>

#include "Core/EngineData.h"
#include "Sprite/SpriteRepository.h"
#include "RessourceHandler/Initializer.h"

namespace spe
{
	class SceneHandler
	{
	public:
		std::vector<std::string> TotalScenes;
		std::string CurrentScene;

		spe::SpriteRepository SpriteRepository;

		void LoadScene(const std::string& name, spe::Camera& camera, spe::Vector3& b);
	};

}

