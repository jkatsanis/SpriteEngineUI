#pragma once

#include <physicalComponents/sprite.h>

#include <vector>

namespace s2d
{
	class PrefabRepositor
	{
	private:
		static std::vector<s2d::Sprite*> s_prefabs;

	public:		
		/// <summary>
		/// Gets the prefab
		/// Will not be addded to the sprites in the scene
		/// </summary>
		/// <param name="name">You specified in the editor</param>
		/// <returns>The prefab or nullptr if you enter a invalid name</returns>
		static s2d::Sprite* getPrefabByName(const std::string& name);

		static void addPrefab(s2d::Sprite* prefab) { s2d::PrefabRepositor::s_prefabs.push_back(prefab); }
	};
}

