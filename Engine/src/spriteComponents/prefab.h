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
		std::string enginePathToFile;

		/// <summary>
		/// Path from the user to the file
		/// Example: assets\\prefabs\\hello.prfb
		/// </summary>
		std::string userPathToFile;

		/// <summary>
		/// The file name of the active file. This may not be the actual prefab
		/// sprite name because it didnt get upated yet
		/// </summary>
		std::string pathToOldFile;

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

		void updateProps(const std::string& enginePath, const std::string& userPath, const std::string& pathToOldFile, const std::string fileName);
	};
}

