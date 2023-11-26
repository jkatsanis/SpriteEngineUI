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
			spe::Initializer::InitSprites(this->SpriteRepository, PATH_TO_SPRITES, this->LightRepository);
			spe::Initializer::InitAnimations(this->SpriteRepository, PATH_TO_ANIMATIONS);
			spe::Initializer::IntiHighestSpriteID(this->SpriteRepository, PATH_TO_HIGHEST_INDEX);

			spe::Initializer::InitCamera(camera, PATH_TO_CAMERA);
			spe::Initializer::InitBackground(bg, PATH_TO_BACKGROUND);
		}
	}
}


void spe::SceneHandler::DeleteScene(const std::string& name)
{
	for (auto it = TotalScenes.begin(); it != TotalScenes.end(); ++it) {
		// Found a scene to delete
		if (*it == name) {
			it = TotalScenes.erase(it); 

			const std::string path = PATH_TO_SAVE_FOLDER + std::string("\\") + name;

			std::filesystem::remove_all(path);
			
			return;
		}
	}
}

void spe::SceneHandler::CreateScene(const std::string& name)
{
	spe::Utility::CopyDir("Engine\\Saves\\Template", "Engine\\Saves", name);
	this->TotalScenes.push_back(name);
}
