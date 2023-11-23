#include "guiRepository.h"

// Constrcutor

spe::GUIRepository::GUIRepository()
{
	this->ptr_AssetWindowSize = nullptr;
	this->ptr_HierarchyWindowSize = nullptr;
	this->ptr_InspectorWindowSize = nullptr;
	this->m_HighestLayer = 0;
	this->m_HighestId = 0;
}

// Public functions

void spe::GUIRepository::Add(spe::Rectangle* rectangle)
{
	rectangle->ID = this->m_HighestId;
	this->m_Rectangles.push_back(rectangle);
	this->m_HighestId++;
}

void spe::GUIRepository::UpdateLayerIndex()
{
	for (size_t i = 0; i < this->m_Rectangles.size(); i++)
	{
		spe::Rectangle* const rec = this->m_Rectangles[i];
		if (rec->SortingLayerIdx > this->m_HighestLayer)
			this->m_HighestLayer = rec->SortingLayerIdx;
	}
}

void spe::GUIRepository::Render(sf::RenderWindow* ptr_render_window)
{
	for (size_t i = 0; i < this->m_HighestLayer + 1; i++)
	{
		for (size_t j = 0; j < this->GetAmount(); j++)
		{
			spe::Rectangle* rec = this->GetByVecPos((uint32_t)j);
			if (this->m_Rectangles[j]->Render
				&& rec->SortingLayerIdx == i)
			{
				ptr_render_window->draw(this->m_Rectangles[j]->Shape);
			}
		}
	}
}

spe::Rectangle* spe::GUIRepository::GetByName(const std::string& name)
{
	for (int i = 0; i < this->m_Rectangles.size(); i++)
	{
		if (this->m_Rectangles[i]->Name == name)
		{
			return this->m_Rectangles[i];
		}
	}
	throw std::out_of_range("Name was not found");
}

spe::Rectangle* spe::GUIRepository::GetById(uint32_t id)
{
	for (size_t i = 0; i < this->m_Rectangles.size(); i++)
	{
		if (this->m_Rectangles[i]->ID == id)
		{
			return this->m_Rectangles[i];
		}
	}

	throw std::out_of_range("ID was not found");
}

spe::Rectangle* spe::GUIRepository::GetByVecPos(uint32_t vec)
{
	for (size_t i = 0; i < this->m_Rectangles.size(); i++)
	{
		if (i == vec)
		{
			return this->m_Rectangles[i];
		}
	}
	throw std::out_of_range("Vecpos was not found");
}
