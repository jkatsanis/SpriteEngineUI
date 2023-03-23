#pragma once

#include <iostream>

namespace s2d
{
	class Sprite;
	class Prefab 
	{
	private:
		s2d::Sprite* m_ptr_attachedSprite;
	public:
		std::string pathToFile;
		bool exists;

		Prefab();
		Prefab(s2d::Sprite* m_attached);
		
		void resetPrefab();
		void updateFile();
	};
}

