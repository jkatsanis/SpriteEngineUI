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
	this->enginePathToFile = "";
}

void s2d::Prefab::updateFile()
{ 
	uint8_t amountToDelete = fileName.size();	
	std::string newPath = this->enginePathToFile;
	newPath.resize(newPath.size() - amountToDelete);

	newPath += this->m_ptr_attachedSprite->name + EXTENSION_PREFAB_FILE;

	s2d::flc::createOrUpdatePrefabFile(this->m_ptr_attachedSprite, newPath, this->pathToOldFile);
	this->updateProps(newPath, s2d::UI::getUserProjectPathSeperatetFromEnginePath(newPath), newPath, this->m_ptr_attachedSprite->name + EXTENSION_PREFAB_FILE);
}

void s2d::Prefab::updateProps(const std::string& enginePath, const std::string& userPath, const std::string& pathToOldFile, const std::string fileName)
{
	this->userPathToFile = userPath;
	this->enginePathToFile = enginePath;
	this->fileName = fileName;
	this->pathToOldFile = pathToOldFile;
}

