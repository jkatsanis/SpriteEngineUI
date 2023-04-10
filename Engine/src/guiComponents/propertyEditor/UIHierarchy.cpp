#include "UIHierarchy.h"

//Constructor

s2d::UIHierarchy::UIHierarchy()
{
	this->isHovered = false;
	this->m_ptr_repo = nullptr;
}

s2d::UIHierarchy::UIHierarchy(s2d::SpriteRepository& repo)
{
	this->isHovered = false;
	this->m_ptr_repo = &repo;
	this->m_ptr_repo->sprite_in_inspector = nullptr;
}

//Public functions

void s2d::UIHierarchy::displayHierarchyWindow()
{
	ImGui::Begin("UIHierarchy", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

	// Render Hierarchy

	this->displaySprites();
	this->displayContextPopup();

	this->isHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenBlockedByPopup);

	ImGui::SetWindowPos(WINDOW_POS);
	ImGui::SetWindowSize(WINDOW_SIZE);
	ImGui::End();
}

void s2d::UIHierarchy::displayContextPopup()
{
	if (ImGui::IsMouseReleased(1) && this->isHovered)
	{
		ImGui::OpenPopup(POPUP_NAME);
	}

	if (ImGui::BeginPopup(POPUP_NAME))
	{
		if (ImGui::BeginMenu("Create"))
		{
			this->addSprite();

			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Delete"))
		{
			this->deleteSprite();
		}
		ImGui::EndPopup();
	}
}

void s2d::UIHierarchy::addSprite()
{
	if (ImGui::MenuItem("Sprite"))
	{
		const size_t vectorPos = this->m_ptr_repo->amount() + 1;
		const std::string name = "Sprite " + std::to_string(vectorPos) + " ID " + std::to_string(s2d::SpriteData::highestSpriteID + 1);

		s2d::Sprite* sprite = new s2d::Sprite(name, s2d::Vector2(0, 0), s2d::SpriteData::defaultSpritePath);

		this->m_ptr_repo->add(sprite);
	}
}

void s2d::UIHierarchy::deleteSprite()
{
	if (this->m_ptr_repo->sprited_hovered_in_hierarchy == nullptr)
	{
		return;
	}
	this->m_ptr_repo->deleteWithName(this->m_ptr_repo->sprited_hovered_in_hierarchy->name);
}

void s2d::UIHierarchy::displaySprites()
{
	bool anyHoverd = false;
	for (size_t i = 0; i < this->m_ptr_repo->amount(); i++)
	{
		s2d::Sprite* sprite = this->m_ptr_repo->readAt(i);

		this->displaySprites(sprite, anyHoverd);
	}
	if (!anyHoverd && !ImGui::IsPopupOpen(POPUP_NAME))
	{
		this->m_ptr_repo->sprited_hovered_in_hierarchy = nullptr;
	}
}

void s2d::UIHierarchy::displaySprites(s2d::Sprite* parent, bool& anyHovered)
{	
	// Handle as parent
	if (parent->isParent())
	{
		if (ImGui::TreeNode(parent->name.c_str()))
		{
			for (size_t i = 0; i < parent->childs.size(); i++)
			{
				s2d::Sprite* child = *parent->childs[i].get();
				this->displaySprites(child, anyHovered);
			}
			ImGui::TreePop();
		}
		return;
	}

	// Handle es child
	if (ImGui::MenuItem(parent->name.c_str()))
	{
		this->m_ptr_repo->sprite_in_inspector = parent;
	}
	
	// Set hovered sprite
	if (ImGui::IsItemHovered())
	{
		anyHovered = true;
		this->m_ptr_repo->sprited_hovered_in_hierarchy = parent;
		return;
	}
}
