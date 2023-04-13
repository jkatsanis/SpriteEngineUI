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
	this->loadInMemory = false;
}

// Public functions

void s2d::Prefab::resetPrefab()
{
	//TODO: delete file 

	this->exists = false;
	this->enginePathToFile = "";
	this->fileName = "";
	this->loadInMemory = false;
	this->userPathToFile = "";
	this->pathToOldFile = "";
}

void s2d::Prefab::updateFile()
{ 
	std::string newPath = "";

	size_t erase = this->enginePathToFile.size() - fileName.size();

	for (int i = 0; i < erase + 1; i++)
	{
		newPath.push_back(this->enginePathToFile[i]);
	}
	newPath.pop_back();

	newPath += this->m_ptr_attachedSprite->name + EXTENSION_PREFAB_FILE;

	this->enginePathToFile = newPath;
	this->exists = true;
	s2d::flc::createOrUpdatePrefabFile(this->m_ptr_attachedSprite, newPath, this->pathToOldFile);
	this->updateProps(newPath, s2d::UI::getUserProjectPathSeperatetFromEnginePath(newPath), newPath, this->m_ptr_attachedSprite->name + EXTENSION_PREFAB_FILE);
}

void s2d::Prefab::updateProps(const std::string& enginePath, const std::string& userPath, const std::string& pathToOldFile, const std::string fileName)
{
	this->exists = true;
	this->userPathToFile = userPath;
	this->enginePathToFile = enginePath;
	this->fileName = fileName;
	this->pathToOldFile = pathToOldFile;
}
