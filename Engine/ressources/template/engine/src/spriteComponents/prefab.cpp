#include "prefab.h"

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
	this->exist = false;
	this->load_in_memory = false;
}

// Public functions

void s2d::Prefab::reset()
{
	//TODO: delete file 
	this->exist = false;
	this->load_in_memory = false;
}
