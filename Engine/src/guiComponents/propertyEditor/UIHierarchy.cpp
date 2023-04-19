#include "UIHierarchy.h"

//Constructor

s2d::UIHierarchy::UIHierarchy()
{
	this->isHovered = false;
	this->m_ptr_repo = nullptr;
	this->m_waitOneFrame = false;
}

s2d::UIHierarchy::UIHierarchy(s2d::SpriteRepository& repo)
{
	this->m_waitOneFrame = false;
	this->isHovered = false;
	this->m_ptr_repo = &repo;
	this->m_ptr_repo->sprite_in_inspector = nullptr;
	this->m_windowSize = ImVec2(250.0f, 1080.0f - 350.0f);
}

//Public functions

void s2d::UIHierarchy::displayHierarchyWindow()
{
	// Set window size if asset folder does not render

	this->m_windowSize = (s2d::UIInfo::s_isAssetFolderActive)
		? ImVec2(250.0f, 1080.0f - this->m_ptr_assetWindowSize->y)
		: ImVec2(250.0f, 1080.0f);

	ImGui::Begin("UIHierarchy", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

	// Render Hierarchy

    bool anyHovered = this->displaySprites();
	this->displayContextPopup();
	this->displayChildToParent();
	this->setSpriteAsChild();

	// Cleaning up

	this->cleanRepoSpritesUp(anyHovered);

	this->isHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenBlockedByPopup);

	ImGui::SetWindowPos(WINDOW_POS);
	ImGui::SetWindowSize(this->m_windowSize);
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
		const std::string name = "Sprite " + std::to_string(vectorPos) + " ID " + std::to_string(this->m_ptr_repo->highestSpriteId + 1);

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
	if (this->m_ptr_repo->sprite_in_inspector != nullptr)
	{
		if (this->m_ptr_repo->sprited_hovered_in_hierarchy->getId() == this->m_ptr_repo->sprite_in_inspector->getId())
		{
			this->m_ptr_repo->sprite_in_inspector = nullptr;
		}
	}

	this->m_ptr_repo->sprite_in_inspector = nullptr;
	
	this->m_ptr_repo->deleteWithName(this->m_ptr_repo->sprited_hovered_in_hierarchy->name);
	this->m_ptr_repo->sprited_hovered_in_hierarchy = nullptr;
}

void s2d::UIHierarchy::cleanRepoSpritesUp(bool isAnyHovered)
{
	if (!isAnyHovered && !ImGui::IsPopupOpen(POPUP_NAME) && this->m_ptr_repo->child_to_parent == nullptr)
	{
		this->m_ptr_repo->sprited_hovered_in_hierarchy = nullptr;
	}
	if (!ImGui::IsMouseDown(0) || this->m_waitOneFrame)
	{
		if (this->m_waitOneFrame)
		{
			this->m_waitOneFrame = false;
			this->m_ptr_repo->child_to_parent = nullptr;
		}
		else this->m_waitOneFrame = true;
	}
}

void s2d::UIHierarchy::setMenuitemHovered(bool& any_hovered, s2d::Sprite* sprite)
{		
	// Handle es child

	bool popStyle = false;
	if (this->m_ptr_repo->sprite_in_inspector != nullptr
		&& this->m_ptr_repo->sprite_in_inspector->getId() == sprite->getId())
	{
		// Set color in Hirarchy
		popStyle = true;
		ImGui::PushStyleColor(ImGuiCol_Text, SPRITE_SELECTED_COLOR);
	}

	// Set sprite in inspector
	ImGui::MenuItem(sprite->name.c_str());
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped)
		&& ImGui::IsMouseReleased(0))
	{
		this->m_ptr_repo->sprite_in_inspector = sprite;
	}

	if (popStyle)
	{
		ImGui::PopStyleColor();
	}

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped))
	{
		if (ImGui::IsMouseDown(0) && this->m_ptr_repo->child_to_parent == nullptr)
		{
			this->m_ptr_repo->child_to_parent = sprite;
		}
		any_hovered = true;
		this->m_ptr_repo->sprited_hovered_in_hierarchy = sprite;
	}
}

void s2d::UIHierarchy::addPrefab()
{
//	if(this->m_ptr_repo.)
}

bool s2d::UIHierarchy::displaySprites()
{
	bool anyHoverd = false;
	for (size_t i = 0; i < this->m_ptr_repo->amount(); i++)
	{
		s2d::Sprite* sprite = this->m_ptr_repo->readAt(i);

		this->displaySprites(sprite, anyHoverd);
	}
	return anyHoverd;
}

void s2d::UIHierarchy::displaySprites(s2d::Sprite* parent, bool& any_hovered)
{	
	if (parent->parent != nullptr)
	{
		return;
	}
	// Handle as parent
	if (parent->isParent())
	{
		this->displaySpriteSeperated(parent, any_hovered);
		return;
	}

	// Set hovered sprite
	this->setMenuitemHovered(any_hovered, parent);
}

void s2d::UIHierarchy::displaySpriteSeperated(s2d::Sprite* parent, bool& b)
{
	bool popStyle = false;
	if (parent->isParent())
	{
		if (this->m_ptr_repo->sprite_in_inspector != nullptr
			&& this->m_ptr_repo->sprite_in_inspector->getId() == parent->getId())
		{
			// Set color in Hirarchy
			popStyle = true;
			ImGui::PushStyleColor(ImGuiCol_Text, SPRITE_SELECTED_COLOR);
		}
		if (ImGui::TreeNode(parent->name.c_str()))
		{
			if (popStyle)
			{
				popStyle = false;
				ImGui::PopStyleColor();
			}
			if (ImGui::IsMouseClicked(0) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped))
			{
				this->m_ptr_repo->sprite_in_inspector = parent;
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped))
			{
				if (ImGui::IsMouseDown(0) && this->m_ptr_repo->child_to_parent == nullptr)
				{
					this->m_ptr_repo->child_to_parent = parent;
				}
				b = true;
				this->m_ptr_repo->sprited_hovered_in_hierarchy = parent;
			}
			for (size_t i = 0; i < parent->ptr_childs.size(); i++)
			{
				s2d::Sprite* child = parent->ptr_childs[i];
				this->displaySpriteSeperated(child, b);
			}
			ImGui::TreePop();
		}
		if (popStyle)
		{
			popStyle = false;
			ImGui::PopStyleColor();
		}
		return;
	}

	// Set hovered sprite
	this->setMenuitemHovered(b, parent);
}


void s2d::UIHierarchy::displayChildToParent()
{
	if (this->m_ptr_repo->child_to_parent == nullptr)
	{
		return;
	}

	const ImVec2 window_pos = ImGui::GetMousePos();
	ImGui::Begin("##DragChild", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
	ImGui::Text(this->m_ptr_repo->child_to_parent->name.c_str());
	ImGui::SetWindowPos(ImVec2(window_pos.x - 15, window_pos.y - 15));
	ImGui::End();
}

void s2d::UIHierarchy::setSpriteAsChild()
{
	if (ImGui::IsMouseReleased(0) && this->m_ptr_repo->sprited_hovered_in_hierarchy != nullptr
		&& this->m_ptr_repo->child_to_parent != nullptr
		&& this->m_ptr_repo->child_to_parent->name != this->m_ptr_repo->sprited_hovered_in_hierarchy->name
		//&& !this->m_ptr_repo->sprited_hovered_in_hierarchy->containsChild(this->m_ptr_repo->child_to_parent)
		&& !this->m_ptr_repo->child_to_parent->containsChild(this->m_ptr_repo->sprited_hovered_in_hierarchy))
		
	{
		this->m_ptr_repo->child_to_parent->setParent(this->m_ptr_repo->sprited_hovered_in_hierarchy);
		this->m_ptr_repo->child_to_parent = nullptr;
	}
}
