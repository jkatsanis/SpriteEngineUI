#pragma once

#include "Sprite/Sprite.h"
#include "UtilityFunctions.h"
#include "RessourceHandler/Initializer.h"
#include "Sprite/Components/Light/LightRepository.h"

#include <vector>

namespace spe
{
	class PrefabRepository
	{
	private:
		static std::vector<spe::Sprite*> s_prefabs;


	public:
		/// <summary>
		/// Gets the prefab
		/// Will not be addded to the sprites in the scene
		/// </summary>
		/// <param name="name">You specified in the editor</param>
		/// <returns>The prefab or nullptr if you enter a invalid name</returns>
		static spe::Sprite* GetPrefabByName(const std::string& name);

		static void AddPrefab(spe::Sprite* prefab) { spe::PrefabRepository::s_prefabs.push_back(prefab); }

		static void LoadPrefabsInMemory(spe::LightRepository& lightRepo);
	};
}

