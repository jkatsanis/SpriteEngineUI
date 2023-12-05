#include "SeceneHandler.h"

void spe::SceneHandler::Init(const std::string& shader)
{
	this->LightRepository.Init(shader);
}

void spe::SceneHandler::LoadScene(const std::string& name, spe::Camera& camera, spe::Vector3& bg)
{
	for (const std::string& str : this->TotalScenes)
	{
		// Found a scene to load
		if (str == name)
		{
			this->SpriteRepository.cleanUp();

			spe::EngineData::s_Scene = name;
			this->CurrentScene = str;

			spe::Initializer::InitSprites(this->SpriteRepository, PATH_TO_SPRITES, this->LightRepository);

			spe::Initializer::InitCamera(camera, PATH_TO_CAMERA);
			spe::Initializer::InitBackground(bg, PATH_TO_BACKGROUND);

			this->SpriteRepository.Initialized = true;
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
