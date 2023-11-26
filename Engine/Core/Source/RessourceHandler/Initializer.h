#pragma once

#include <string>
#include <fstream>

#include "Camera/Camera.h"
#include "Sprite/SpriteRepository.h"
#include "Sprite/Sprite.h"
#include "RessourceHandler/FileDataMacros.h"

namespace spe
{
	class SceneHandler;

	/// <summary>
	/// This class assumes that the paths entered are from the user project
	/// </summary>
	class Initializer
	{
	public:
		Initializer() = delete;


		/// <summary>
		/// inits the prefab. Returns a new sprite on the heap. Can be pushed back to the sprite
		/// repoistory
		/// </summary>
		static spe::Sprite* InitPrefab(const std::string& path, spe::LightRepository& repo);

		static void InitTags(spe::SpriteRepository& repo, const std::string& path);
		static void InitScenes(spe::SceneHandler& handler, const std::string& path);
		static void IntiHighestSpriteID(spe::SpriteRepository& repo, const std::string& path);
		static void InitBackground(spe::Vector3& vec, const std::string& path);
		static spe::Sprite* InitSprite(const std::string& line, spe::LightRepository& lightrepo);
		static void InitSprites(spe::SpriteRepository& spriteRepo, const std::string& path, spe::LightRepository& lightrepo);
		static void InitCamera(spe::Camera& camera, const std::string& path);


		static void InitAnimations(spe::SpriteRepository& repo, const std::string& path);
		static void InitAnimation(const std::string& path, spe::Sprite* spr);
		static void InitAnimation(const std::string& path, spe::SpriteRepository& repo);
	};
}

