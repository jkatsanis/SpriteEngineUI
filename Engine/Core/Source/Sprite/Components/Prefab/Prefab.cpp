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
	this->engine_path_to_file = "";
	this->file_name = "";
	this->load_in_memory = false;
	this->user_path_to_file = "";
	this->path_to_old_file = "";
}

void spe::Prefab::updateProps(const std::string& enginePath, const std::string& userPath, const std::string& pathToOldFile, const std::string fileName)
{
	this->exist = true;
	this->user_path_to_file = userPath;
	this->engine_path_to_file = enginePath;
	this->file_name = fileName;
	this->path_to_old_file = pathToOldFile;
}
