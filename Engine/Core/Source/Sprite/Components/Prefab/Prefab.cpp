#include "prefab.h"

#include "Sprite.h"


// Constructor 

spe::Prefab::Prefab()
{
	this->init();
}

spe::Prefab::Prefab(spe::Sprite* m_attached)
{
	this->init();
	this->m_ptr_attachedSprite = m_attached;
}

spe::Prefab::Prefab(spe::Sprite* m_attached, const spe::Prefab& rhs)
{
	this->m_ptr_attachedSprite = m_attached;
	this->file_name = rhs.file_name;
	this->exist = rhs.exist;
	this->path_to_old_file = rhs.path_to_old_file;
	this->user_path_to_file = rhs.user_path_to_file;
}

void spe::Prefab::init()
{
	this->m_ptr_attachedSprite = nullptr;
	this->exist = false;
	this->load_in_memory = false;
}

// Public functions

void spe::Prefab::reset()
{
	//TODO: delete file 

	this->exist = false;
	this->file_name = "";
	this->load_in_memory = false;
	this->user_path_to_file = "";
	this->path_to_old_file = "";
}

void spe::Prefab::updateProps(const std::string& userPath, const std::string& pathToOldFile, const std::string fileName)
{
	this->exist = true;
	this->user_path_to_file = userPath;
	this->file_name = fileName;
	this->path_to_old_file = pathToOldFile;
}

void spe::Prefab::UpdatePath()
{
	if (!this->exist)
	{
		return;
	}
	this->path_to_old_file = this->user_path_to_file;
	std::string newPath = "";

	size_t erase = this->user_path_to_file.size() - file_name.size();

	for (int i = 0; i < erase + 1; i++)
	{
		newPath.push_back(this->user_path_to_file[i]);
	}
	newPath.pop_back();

	newPath += this->m_ptr_attachedSprite->Name + EXTENSION_PREFAB_FILE;
	this->user_path_to_file = newPath;
}

void spe::Prefab::UpdateName()
{
	if (!this->exist)
	{
		return;
	}
	std::vector<std::string> parts = spe::Utility::Split(this->user_path_to_file, '\\');
	this->file_name = parts[parts.size() - 1];
}
