#include "prefab.h"
#include <physicalComponents/sprite.h>
#include <manager/saveSystem.h>

// Constructor 

s2d::Prefab::Prefab()
{
	init();
}

s2d::Prefab::Prefab(s2d::Sprite* m_attached)
{
	init();
	this->m_ptr_attachedSprite = m_attached;
}

void s2d::Prefab::init()
{
	this->m_ptr_attachedSprite = nullptr;
	this->exists = false;
	this->loadInMemory = false;
}

// Public functions

void s2d::Prefab::resetPrefab()
{
	//TODO: delete file 

	this->exists = false;
	this->pathToFile = "";
}

void s2d::Prefab::updateFile()
{ 
	std::string fileName = this->m_ptr_attachedSprite->name + EXTENSION_PREFAB_FILE;

	int amountToDelete = this->pathToFile.size() - fileName.size();

	std::string fnPath = this->pathToFile;
	fnPath.erase(fnPath.begin() + amountToDelete);

	const std::string path = s2d::EngineData::s_pathToUserProject + "\\" + fnPath;
	s2d::flc::createOrUpdatePrefabFile(this->m_ptr_attachedSprite, path);
}

