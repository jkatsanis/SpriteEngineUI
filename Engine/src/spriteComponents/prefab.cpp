#include "prefab.h"
#include <physicalComponents/sprite.h>

// Constructor 

s2d::Prefab::Prefab()
{
	this->m_ptr_attachedSprite = nullptr;
	this->exists = false;
}

s2d::Prefab::Prefab(s2d::Sprite* m_attached)
{
	this->m_ptr_attachedSprite = m_attached;
	this->exists = false;
}

// Public functions

void s2d::Prefab::resetPrefab()
{
	//TODO: delete file 

	this->exists = false;
	this->pathToFile = "";
}

