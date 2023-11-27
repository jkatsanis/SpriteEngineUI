#pragma once

#include <iostream>
#include <vector>

#include "SpriteRepository.h"
#include "GUI/GUIRepository.h"
#include "Core/SeceneHandler.h"
#include "GUI/UIUtility/UIModels.h"

namespace spe
{
	/// <summary>
	/// This class assumes your are in th engine dir
	/// </summary>
	class Savesystem
	{
	public:
		Savesystem() = delete;

		static void CreateAnimationSaveFile(const spe::Sprite* ptr_sprite, const spe::Animation& animationToSave, const spe::SpriteRepository* sprites);

		static void UpdateSceneFile(const spe::SceneHandler& handler);
		static void UpdateSpriteFile(const spe::SpriteRepository& repo);
		static void UpdateBackgroundFile(const spe::Vector3& bg);
		static void UpdateHighestIndexFile(uint32_t idx);
		static void UpdateCameraFile(const spe::Camera& camera);
		static void UpdateTagsFile(const spe::SpriteRepository& repo);

		/// <summary>
		/// Saves everything
		/// </summary>
		/// <param name="sprites">The sprites</param>
		/// <param name="gui">Camera, background</param>
		/// <param name="scene">The scenes</param>
		static void SaveEverything(const spe::SpriteRepository& sprites,
							const spe::Camera& gui,
							const spe::Vector3& bg,
							const spe::SceneHandler& scene);

		static void SaveProjects(const std::vector<spe::UserProjectInfo>& projects);

		static std::string GetPropertyLineWithSeperator(const spe::Sprite* sprite);

		static void CreateOrUpdatePrefabFile(const spe::Sprite* content, const std::string& pathToFile, const std::string& oldFilePath);
	};
}