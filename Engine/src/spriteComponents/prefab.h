#pragma once

#include <iostream>

namespace s2d
{
	class Sprite;
	class Prefab 
	{
	private:
		s2d::Sprite* m_ptr_attachedSprite;

		void init();
	public:

		/// <summary>
		/// The path from the user project to the file for example
		/// assets\\file
		/// </summary>
		std::string pathToFile;

		/// <summary>
		/// The file name of the active file. This may not be the actual prefab
		/// sprite name because it didnt get upated yet
		/// </summary>
		std::string fileName;
		bool exists;
		bool loadInMemory;

		Prefab();
		Prefab(s2d::Sprite* m_attached);
		
		/// <summary>
		/// Resets the data and deletes the FILE!
		/// </summary>
		void resetPrefab();

		/// <summary>
		/// Always needs to getupdated when changes are made in the editor or booleans are getting changed
		/// </summary>
		void updateFile();
	};
}

