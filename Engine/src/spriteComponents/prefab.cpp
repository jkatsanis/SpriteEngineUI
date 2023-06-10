#include "prefab.h"
#include <physicalComponents/sprite.h>
#include <manager/saveSystem.h>

// Constructor 

s2d::Prefab::Prefab()
{
	this->init();
}

s2d::Prefab::Prefab(s2d::Sprite* m_attached)
{
	this->init();
	this->m_ptr_attachedSprite = m_attached;
}

void s2d::Prefab::init()
{
	this->m_ptr_attachedSprite = nullptr;
	this->exists = false;
	this->load_in_memory = false;
}

// Public functions

void s2d::Prefab::resetPrefab()
{
	//TODO: delete file 

	this->exists = false;
	this->engine_path_to_file = "";
	this->file_name = "";
	this->load_in_memory = false;
	this->user_path_to_file = "";
	this->path_to_old_file = "";
}

void s2d::Prefab::updateFile()
{ 
	std::string newPath = "";

	size_t erase = this->engine_path_to_file.size() - file_name.size();

	for (int i = 0; i < erase + 1; i++)
	{
		newPath.push_back(this->engine_path_to_file[i]);
	}
	newPath.pop_back();

	newPath += this->m_ptr_attachedSprite->name + EXTENSION_PREFAB_FILE;

	this->engine_path_to_file = newPath;
	this->exists = true;
	s2d::flc::createOrUpdatePrefabFile(this->m_ptr_attachedSprite, newPath, this->path_to_old_file);
	this->updateProps(newPath, s2d::UI::getUserProjectPathSeperatetFromEnginePath(newPath), newPath, this->m_ptr_attachedSprite->name + EXTENSION_PREFAB_FILE);
}

void s2d::Prefab::updateProps(const std::string& enginePath, const std::string& userPath, const std::string& pathToOldFile, const std::string fileName)
{
	this->exists = true;
	this->user_path_to_file = userPath;
	this->engine_path_to_file = enginePath;
	this->file_name = fileName;
	this->path_to_old_file = pathToOldFile;
}
