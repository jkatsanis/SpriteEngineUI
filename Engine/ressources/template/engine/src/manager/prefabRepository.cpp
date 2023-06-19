#include "prefabRepository.h"

// public static functions

s2d::Sprite* s2d::PrefabRepositor::getPrefabByName(const std::string& name)
{
	for (size_t i = 0; i < s2d::PrefabRepositor::s_prefabs.size(); i++)
	{
		if (s2d::PrefabRepositor::s_prefabs[i]->name == name)
		{
			// return a COPY
			s2d::Sprite* copy = new s2d::Sprite(*s2d::PrefabRepositor::s_prefabs[i]);
			return copy;
		}
	}
	return nullptr;
}

// statics

std::vector<s2d::Sprite*> s2d::PrefabRepositor::s_prefabs = std::vector<s2d::Sprite*>(0);


