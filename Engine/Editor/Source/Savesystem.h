#pragma once

#include <iostream>
#include <vector>

#include "SpriteRepository.h"
#include "GUI/GUIRepository.h"
#include "Core/SeceneHandler.h"
#include "GUI/UIUtility/UIModels.h"

namespace spe
{
	class Savesystem
	{
	public:
		Savesystem() = delete;

		static void CreateAnimationSaveFile(const spe::Sprite* ptr_sprite, const spe::Animation& animationToSave);

		/// <summary>
		/// Saves everything
		/// </summary>
		/// <param name="sprites">The sprites</param>
		/// <param name="gui">Camera, background</param>
		/// <param name="scene">The scenes</param>
		static void SaveEverything(const spe::SpriteRepository& sprites,
							const spe::GUIRepository& gui,
							const spe::SceneHandler& scene);

		static void SaveProjects(const std::vector<spe::UserProjectInfo>& projects);
	};
}