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
		/// This functions assumes you are in the user projuect path. Use setcurrentdir if you in the engine lol
		/// </summary>
		/// 
	
		// Sprite
		static void PostInitAnimation(spe::Sprite* sprite, const std::string& file_path, spe::SpriteRepository& repo);

		static void InitTags(spe::SpriteRepository& repo, const std::string& path);
		static void InitScenes(spe::SceneHandler& handler, const std::string& path);
		static void IntiHighestSpriteID(spe::SpriteRepository& repo, const std::string& path);
		static void InitBackground(spe::Vector3& vec, const std::string& path);
		static spe::Sprite* InitSprite(const std::string& line);
		static void InitSprites(spe::SpriteRepository& spriteRepo, const std::string& path);
		static void InitAnimations(spe::SpriteRepository& repo, const std::string& path);
		static void initAnimation(const std::string& path, spe::SpriteRepository& repo, int32_t idx);
		static void InitCamera(spe::Camera& camera, const std::string& path);
	};
}

