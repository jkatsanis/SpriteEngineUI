#include "UIHierarchy.h"

//Constructor

s2d::UIHierarchy::UIHierarchy()
{
	this->m_childSelectTimer = 0.0f;
	this->m_clickedOnResizeButton = false;
	this->m_foundHovering = false;
	this->isHovered = false;
	this->m_ptr_repo = nullptr;
	this->m_waitOneFrame = false;
	this->m_ptr_assetWindowSize = nullptr;
	this->m_windowSizeX = -1;
}

s2d::UIHierarchy::UIHierarchy(s2d::SpriteRepository& repo)
{
	this->m_childSelectTimer = 0.0f;
	this->m_clickedOnResizeButton = false;
	this->m_foundHovering = false;
	this->m_ptr_assetWindowSize = nullptr;
	this->m_waitOneFrame = false;
	this->isHovered = false;
	this->m_ptr_repo = &repo;
	this->m_ptr_repo->sprite_in_inspector = nullptr;
	this->m_windowSize = ImVec2(250.0f, 1080.0f - 350.0f);
	this->m_windowSizeX = 300;
	this->m_spriteBackgRoundRowCounter = 1;
}

//Public functions

void s2d::UIHierarchy::displayHierarchyWindow()
{
	// Set window size if asset folder does not render

	this->m_windowSize = (s2d::UIInfo::s_isAssetFolderActive)
		? ImVec2(this->m_windowSizeX, WINDOW_SIZE_HIERARCHY_Y- this->m_ptr_assetWindowSize->y)
		: ImVec2(this->m_windowSizeX, WINDOW_SIZE_HIERARCHY_Y);

	ImGui::Begin("##ui-hierarchy", NULL,
		ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoScrollbar);

	// Render Hierarchy

	if (!ImGui::IsPopupOpen(POPUP_NAME))
	{
		this->m_foundHovering = false;
	}

	this->m_spriteBackgRoundRowCounter = 1;
	this->renderCloseRectangle();
	this->renderHierarchyOptions();
	ImGui::Dummy(ImVec2(0, 10));
    bool anyHovered = this->displaySprites();
	this->displayContextPopup();
	this->displayChildToParent();
	this->setSpriteAsChild();

	// Cleaning up

	this->cleanRepoSpritesUp(anyHovered);

	this->isHovered = s2d::UI::isHovered(WINDOW_POS, this->m_windowSize);

	ImGui::SetWindowPos(WINDOW_POS);
	ImGui::SetWindowSize(this->m_windowSize);
	ImGui::SetWindowFontScale(s2d::UIInfo::s_defaultFontSize);
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

void s2d::UIHierarchy::setHovering(s2d::Sprite* sprite, bool& anyHovered)
{
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped))
	{
		this->m_foundHovering = true;
		if (ImGui::IsMouseDown(0) && this->m_ptr_repo->child_to_parent == nullptr && this->m_ptr_repo->assetFolderData.dragAndDropPath == " ")
		{
			this->m_childSelectTimer += s2d::Time::deltaTime;

			if (this->m_childSelectTimer > TIME_TO_CAN_SELECT_CHILD)
			{
				this->m_ptr_repo->child_to_parent = sprite;
			}
		}
		else
		{
			this->m_childSelectTimer = 0.0f;
		}
		anyHovered = true;
		this->m_ptr_repo->sprited_hovered_in_hierarchy = sprite;
	}
	else if (!this->m_foundHovering && !ImGui::IsPopupOpen(POPUP_NAME))
	{
		this->m_ptr_repo->sprited_hovered_in_hierarchy = nullptr;
	}
}

void s2d::UIHierarchy::setMenuitemHovered(bool& any_hovered, s2d::Sprite* sprite)
{		
	// Handle es child
	if (this->m_searchSpriteFilter.PassFilter(sprite->name.c_str()))
	{
		bool popStyle = false;
		if (this->m_ptr_repo->sprite_in_inspector != nullptr
			&& this->m_ptr_repo->sprite_in_inspector->getId() == sprite->getId())
		{
			// Set color in Hirarchy
			popStyle = true;
			ImGui::PushStyleColor(ImGuiCol_Text, SPRITE_SELECTED_COLOR);
		}

		std::string name = sprite->name;
		if (sprite->prefab.exists)
		{
			name += " (Prefab)";
		}

		float add = 0;
		if (sprite->parent != nullptr)
		{
			add = ADD_WHEN_SPRITE_HAS_PARENT;
		}
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + MENU_ITEM_PADDING + add);

		this->drawbackgroundRectangle();
		// Set sprite in inspector
		ImGui::MenuItem(name.c_str());
		this->setHovering(sprite, any_hovered);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped)
			&& ImGui::IsMouseReleased(0))
		{
			this->m_ptr_repo->sprite_in_inspector = sprite;
		}

		if (popStyle)
		{
			ImGui::PopStyleColor();
		}
	}
}

void s2d::UIHierarchy::addPrefab()
{
//	if(this->m_ptr_repo.)
}

void s2d::UIHierarchy::renderCloseRectangle()
{
	 s2d::UI::renderCloseRectangle(
		 FOLDER_SPRITE_HIERARCHY_PADDING, ICON_FA_FILE_CODE, "##close-rectangle-hierarchy", "Hierarchy", 0);
}

void s2d::UIHierarchy::renderHierarchyOptions()
{
	const ImVec2 tempCursor = ImGui::GetCursorPos();

	this->resizeWindow();

	ImGui::SetCursorPos(tempCursor);

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
	ImGui::SetCursorPosX(FOLDER_SPRITE_HIERARCHY_PADDING);
	ImGui::BeginChild("##options-container", ImVec2(this->m_windowSize.x - FOLDER_SPRITE_HIERARCHY_PADDING * 2, 45));

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 10, ImGui::GetCursorPosY() + 10));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.0f); // Set rounding to 5 pixels
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 3.0f)); // Add some padding for visual clarity
	ImGui::SetNextItemWidth(150);
	this->m_searchSpriteFilter.Draw("Search");
	ImGui::PopStyleVar(2);

	ImGui::EndChild();	
}

void s2d::UIHierarchy::resizeWindow()
{
	bool popStyle = false;
	if (this->m_clickedOnResizeButton)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 1));
		popStyle = true;
	}
	ImGui::SetCursorPos(ImVec2(this->m_windowSize.x - 40, 0));
	s2d::FontManager::displaySmybolAsButton(ICON_FA_ARROW_RIGHT);
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
	{
		this->m_clickedOnResizeButton = true;
	}
	if (this->m_clickedOnResizeButton && ImGui::IsMouseDown(0))
	{
		float movedy = 0;
		if (s2d::UI::s_guiCorsor.posiitonChanged)
		{
			s2d::Vector2 moved = s2d::UI::s_guiCorsor.lastPos - s2d::UI::s_guiCorsor.position;
			movedy = moved.x;
		}
		this->m_windowSizeX -= movedy;
	}
	else
	{
		this->m_clickedOnResizeButton = false;
	}
	if (popStyle)
	{
		ImGui::PopStyleColor(2);
	}
}

void s2d::UIHierarchy::drawbackgroundRectangle()
{
	if (this->m_spriteBackgRoundRowCounter < 1)
	{
		this->m_spriteBackgRoundRowCounter++;
		return;
	}
	this->m_spriteBackgRoundRowCounter = 0;

	const ImVec2 cursorPos = ImGui::GetCursorPos();
	ImGui::SetCursorPosX(0);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 73);
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 cursor_pos = ImGui::GetCursorPos();
	ImVec2 node_size = ImVec2(100, 20); // change this to the desired size
	draw_list->AddRectFilled(cursor_pos, ImVec2(cursor_pos.x + this->m_windowSizeX, cursor_pos.y - 20), IM_COL32(30, 30, 30, 255));


	ImGui::SetCursorPos(cursorPos);
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

void s2d::UIHierarchy::displaySpriteSeperated(s2d::Sprite* parent, bool& anyHovered)
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
		std::string name = parent->name;
		if (parent->prefab.exists)
		{
			name += " (Prefab)";
		}
		if (this->m_searchSpriteFilter.PassFilter(name.c_str()) || parent->containsChild(this->m_searchSpriteFilter))
		{
			if (parent->containsChild(this->m_searchSpriteFilter) && this->m_searchSpriteFilter.CountGrep != 0)
			{
				ImGui::SetNextItemOpen(true);
			}
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + TREE_NODE_PADDING);
			this->drawbackgroundRectangle();
			if (ImGui::TreeNode(name.c_str()))
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
				this->setHovering(parent, anyHovered);
				for (size_t i = 0; i < parent->ptr_childs.size(); i++)
				{
					s2d::Sprite* child = parent->ptr_childs[i];
					this->displaySpriteSeperated(child, anyHovered);
				}
				ImGui::TreePop();
			}
			this->setHovering(parent, anyHovered);
		}
		if (popStyle)
		{
			popStyle = false;
			ImGui::PopStyleColor();
		}
		return;
	}

	// Set hovered sprite
	this->setMenuitemHovered(anyHovered, parent);
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
