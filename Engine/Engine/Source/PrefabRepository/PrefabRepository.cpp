#include "PrefabRepository.h"

// public static functions

spe::Sprite* spe::PrefabRepository::GetPrefabByName(const std::string& name)
{
	for (size_t i = 0; i < spe::PrefabRepository::s_prefabs.size(); i++)
	{
		if (spe::PrefabRepository::s_prefabs[i]->Name == name)
		{
			// return a COPY
			spe::Sprite* copy = new spe::Sprite(*spe::PrefabRepository::s_prefabs[i]);
			return copy;
		}
	}
	return nullptr;
}

void spe::PrefabRepository::LoadPrefabsInMemory(spe::LightRepository& lightRepo)
{
	std::vector<std::string> prefab_paths;
	spe::Utility::GetFilePathWithExtensionInFolder("Assets", EXTENSION_PREFAB_FILE, prefab_paths);

	for (size_t i = 0; i < prefab_paths.size(); i++)
	{
		try
		{
			spe::Sprite* prefab = spe::Initializer::InitPrefab(prefab_paths[i], lightRepo);
			if (prefab->Prefab.LoadInMemory)
			{

				spe::PrefabRepository::s_prefabs.push_back(prefab);
			}
			else
			{
				delete prefab;
			}
		}
		catch (const std::exception& e)
		{
			std::string err = e.what();
			spe::Log::LogString("An exception occurred: " + err);
			continue;
		}

	}
}

// statics

std::vector<spe::Sprite*> spe::PrefabRepository::s_prefabs = std::vector<spe::Sprite*>(0);


