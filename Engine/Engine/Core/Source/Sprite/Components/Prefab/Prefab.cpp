#include "prefab.h"

#include "Sprite/Sprite.h"


// Constructor 

spe::Prefab::Prefab()
{
	this->Init();
}

spe::Prefab::Prefab(spe::Sprite* m_attached)
{
	this->Init();
	this->m_ptr_Sprite = m_attached;
}

spe::Prefab::Prefab(spe::Sprite* m_attached, const spe::Prefab& rhs)
{
	this->m_ptr_Sprite = m_attached;
	this->FileName = rhs.FileName;
	this->Exist = rhs.Exist;
	this->PathToOldFile = rhs.PathToOldFile;
	this->PathToFile = rhs.PathToFile;
}

void spe::Prefab::Init()
{
	this->m_ptr_Sprite = nullptr;
	this->Exist = false;
	this->LoadInMemory = false;
}

// Public functions

void spe::Prefab::Reset()
{
	//TODO: delete file 

	this->Exist = false;
	this->FileName = "";
	this->LoadInMemory = false;
	this->PathToFile = "";
	this->PathToOldFile = "";
}

void spe::Prefab::UpdateProps(const std::string& userPath, const std::string& pathToOldFile, const std::string fileName)
{
	this->Exist = true;
	this->PathToFile = userPath;
	this->FileName = fileName;
	this->PathToOldFile = pathToOldFile;
}

void spe::Prefab::UpdatePath()
{
	if (!this->Exist)
	{
		return;
	}
	this->PathToOldFile = this->PathToFile;
	std::string newPath = "";

	size_t erase = this->PathToFile.size() - FileName.size();

	for (int i = 0; i < erase + 1; i++)
	{
		newPath.push_back(this->PathToFile[i]);
	}
	newPath.pop_back();

	newPath += this->m_ptr_Sprite->Name + EXTENSION_PREFAB_FILE;
	this->PathToFile = newPath;
}

void spe::Prefab::UpdateName()
{
	if (!this->Exist)
	{
		return;
	}
	std::vector<std::string> parts = spe::Utility::Split(this->PathToFile, '\\');
	this->FileName = parts[parts.size() - 1];
}
