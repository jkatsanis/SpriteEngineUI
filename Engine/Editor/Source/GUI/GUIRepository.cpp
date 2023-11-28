#include "guiRepository.h"

#include "Sprite.h"

// Constrcutor

spe::GUIRepository::GUIRepository()
{
	this->sprited_hovered_in_hierarchy = nullptr;
	this->sprite_in_inspector = nullptr;
	this->child_to_parent = nullptr;
	this->right_clicked_sprite = nullptr;

	this->m_HighestLayer = 0;
	this->m_HighestId = 0;

	this->DragAndDropPath = " ";
	this->DragAndDropName = " ";

	this->Tools = spe::EditorTools::PositionTool;
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

void spe::GUIRepository::EraseSpriteWithName(const std::string& name)
{
	for (size_t i = 0; i < this->HierarchySprites.size(); i++)
	{
		if (name == this->HierarchySprites[i]->name)
		{
			spe::Sprite* ptr_delete = HierarchySprites[i];

			this->HierarchySprites.erase(this->HierarchySprites.begin() + i);

			std::vector<spe::Sprite*> childs_to_delete = std::vector<spe::Sprite*>(0);

			this->AddChildsToDelete(childs_to_delete, ptr_delete);

			for (size_t i = 0; i < childs_to_delete.size(); i++)
			{
				this->EraseSpriteWithId(childs_to_delete[i]->getId());
			}

			return;
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

void spe::GUIRepository::CleanUp()
{
	this->HierarchySprites.clear();
	this->sprited_hovered_in_hierarchy = nullptr;
	this->sprite_in_inspector = nullptr;
	this->child_to_parent = nullptr;
}

void spe::GUIRepository::InitHierarchySprites(std::list<spe::Sprite*>& sprites)
{
	for (auto it = sprites.begin(); it != sprites.end(); ++it)
	{
		spe::Sprite* sprite = *it;
		this->HierarchySprites.push_back(sprite);
	}
}

// Private

void spe::GUIRepository::AddChildsToDelete(std::vector<Sprite*>& ids, Sprite* parent) 
{
	for (int i = 0; i < parent->ptr_childs.size(); i++)
	{
		ids.push_back(parent->ptr_childs[i]);
		AddChildsToDelete(ids, parent->ptr_childs[i]);
	}
}

void spe::GUIRepository::EraseSpriteWithId(uint32_t id)
{
	for (int i = 0; i < this->HierarchySprites.size(); i++)
	{
		if (this->HierarchySprites[i]->getId() == id)
		{
			this->HierarchySprites.erase(this->HierarchySprites.begin() + i);
			return;
		}
	}
}
