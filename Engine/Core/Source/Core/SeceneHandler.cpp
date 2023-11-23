#include "SeceneHandler.h"

void spe::SceneHandler::LoadScene(const std::string& name, spe::Camera& camera, spe::Vector3& bg)
{
	for (const std::string& str : this->TotalScenes)
	{
		// Found a scene to load
		if (str == name)
		{
			std::string crnt = spe::Utility::GetCurrentDir();

			spe::EngineData::s_Scene = name;
			this->CurrentScene = str;
			spe::Initializer::InitSprites(this->SpriteRepository, PATH_TO_SPRITES);
			spe::Initializer::InitAnimations(this->SpriteRepository, PATH_TO_ANIMATIONS);
			spe::Initializer::IntiHighestSpriteID(this->SpriteRepository, PATH_TO_HIGHEST_INDEX);

			spe::Initializer::InitCamera(camera, PATH_TO_CAMERA);
			spe::Initializer::InitBackground(bg, PATH_TO_BACKGROUND);
		}
	}
}
