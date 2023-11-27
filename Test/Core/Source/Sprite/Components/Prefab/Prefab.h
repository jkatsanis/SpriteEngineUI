#pragma once

#include "Sprite/Components/Component.h"

#include <iostream>

namespace spe
{
	class Sprite;
	class Prefab : public spe::Component
	{
	private:
		spe::Sprite* m_ptr_attachedSprite;
		void init() override;
	public:

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
		Prefab(spe::Sprite* m_attached);
		Prefab(spe::Sprite* m_attached, const spe::Prefab& rhs);
		
		/// <summary>
		/// Resets the data and deletes the FILE!
		/// </summary>
		void reset() override;

		void UpdatePath();
		void UpdateName();

		void updateProps(const std::string& userPath, const std::string& pathToOldFile, const std::string fileName);
	};
}

