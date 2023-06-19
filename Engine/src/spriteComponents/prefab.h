#pragma once

#include <spriteComponents/component.h>

#include <iostream>

namespace s2d
{
	class Sprite;
	class Prefab : public s2d::Component
	{
	private:
		s2d::Sprite* m_ptr_attachedSprite;
		void init() override;
	public:

		/// <summary>
		/// The path from the user project to the file for example
		/// assets\\file
		/// </summary>
		std::string engine_path_to_file;

		/// <summary>
		/// Path from the user to the file
		/// Example: assets\\prefabs\\hello.prfb
		/// </summary>
		std::string user_path_to_file;

		/// <summary>
		/// The file name of the active file. This may not be the actual prefab
		/// sprite name because it didnt get upated yet
		/// </summary>
		std::string path_to_old_file;

		std::string file_name;

		bool load_in_memory;

		Prefab();
		Prefab(s2d::Sprite* m_attached);
		
		/// <summary>
		/// Resets the data and deletes the FILE!
		/// </summary>
		void reset() override;

		/// <summary>
		/// Always needs to getupdated when changes are made in the editor or booleans are getting changed
		/// </summary>
		void updateFile();

		void updateProps(const std::string& enginePath, const std::string& userPath, const std::string& pathToOldFile, const std::string fileName);
	};
}

