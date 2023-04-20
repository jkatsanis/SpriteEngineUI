#include "UIHierarchy.h"

//Constructor

s2d::UIHierarchy::UIHierarchy()
{
	this->init();
}

s2d::UIHierarchy::UIHierarchy(s2d::SpriteRepository& repo)
{
	this->init();
	this->m_ptr_repo = &repo;	
}

void s2d::UIHierarchy::init()
{
	this->m_ptr_repo = nullptr;
	this->m_child_select_timer = 0.0f;
	this->m_clicked_on_resize_button = false;
	this->m_found_hovering = false;
	this->m_ptr_asset_window_size = nullptr;
	this->m_wait_one_frame = false;
	this->is_hovered = false;
	this->m_window_size = HIERARCHY_DEFAULT_WINDOW_SIZE;
	this->m_sprite_background_color = 1;
}

//Public functions

void s2d::UIHierarchy::displayHierarchyWindow()
{
	// Set window size if asset folder does not render

	this->m_window_size = (s2d::UIInfo::s_is_asset_folder_open.is_open)
		? ImVec2(this->m_window_size.x, WINDOW_SIZE_HIERARCHY_Y- this->m_ptr_asset_window_size->y)
		: ImVec2(this->m_window_size.x, WINDOW_SIZE_HIERARCHY_Y);

	if (s2d::UI::handleCloseAndReloadWindow(
		s2d::UIInfo::s_is_hierarchy_open.is_open, s2d::UIInfo::s_is_hierarchy_open.reload,
		this->is_hovered,
		this->m_window_size, HIERARCHY_DEFAULT_WINDOW_SIZE))
	{
		return;
	}

	ImGui::Begin("##ui-hierarchy", NULL,
		ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoScrollbar);

	// Render Hierarchy
	this->m_sprite_background_color = 1;

	if (!ImGui::IsPopupOpen(POPUP_NAME))
	{
		this->m_found_hovering = false;
	}

	this->renderCloseRectangle();
	this->renderHierarchyOptions();
	ImGui::Dummy(ImVec2(0, 10));
    bool anyHovered = this->displaySprites();
	this->displayContextPopup();
	this->displayChildToParent();
	this->setSpriteAsChild();

	// Cleaning up

	this->cleanRepoSpritesUp(anyHovered);

	this->is_hovered = s2d::UI::isHovered(WINDOW_POS, this->m_window_size);

	ImGui::SetWindowPos(WINDOW_POS);
	ImGui::SetWindowSize(this->m_window_size);
	ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size);
	ImGui::End();
}

void s2d::UIHierarchy::displayContextPopup()
{
	if (ImGui::IsMouseReleased(1) && this->is_hovered)
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
	if (!ImGui::IsMouseDown(0) || this->m_wait_one_frame)
	{
		if (this->m_wait_one_frame)
		{
			this->m_wait_one_frame = false;
			this->m_ptr_repo->child_to_parent = nullptr;
		}
		else this->m_wait_one_frame = true;
	}
}

void s2d::UIHierarchy::setHovering(s2d::Sprite* sprite, bool& anyHovered)
{
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped))
	{
		this->m_found_hovering = true;
		if (ImGui::IsMouseDown(0) && this->m_ptr_repo->child_to_parent == nullptr && this->m_ptr_repo->asset_folder_data.drag_and_drop_path == " ")
		{
			this->m_child_select_timer += s2d::Time::deltaTime;

			if (this->m_child_select_timer > TIME_TO_CAN_SELECT_CHILD)
			{
				this->m_ptr_repo->child_to_parent = sprite;
			}
		}
		else
		{
			this->m_child_select_timer = 0.0f;
		}
		anyHovered = true;
		this->m_ptr_repo->sprited_hovered_in_hierarchy = sprite;
	}
	else if (!this->m_found_hovering && !ImGui::IsPopupOpen(POPUP_NAME))
	{
		this->m_ptr_repo->sprited_hovered_in_hierarchy = nullptr;
	}
}

void s2d::UIHierarchy::setMenuitemHovered(bool& any_hovered, s2d::Sprite* sprite)
{		
	// Handle es child
	if (this->m_search_sprite_filter.PassFilter(sprite->name.c_str()))
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
	s2d::UIInfo::s_is_hierarchy_open.is_open = s2d::UI::renderCloseRectangle(
		 FOLDER_SPRITE_HIERARCHY_PADDING, ICON_FA_FILE_CODE, "##close-rectangle-hierarchy", "Hierarchy", 0);
}

void s2d::UIHierarchy::renderHierarchyOptions()
{
	const ImVec2 tempCursor = ImGui::GetCursorPos();

	this->resizeWindow();

	ImGui::SetCursorPos(tempCursor);

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
	ImGui::SetCursorPosX(0);
	ImGui::BeginChild("##hierarchy-options-container", ImVec2(this->m_window_size.x, 45));

	ImGui::SetCursorPos(ImVec2(0, ImGui::GetCursorPosY() + 10));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.0f); // Set rounding to 5 pixels
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 3.0f)); // Add some padding for visual clarity
	ImGui::SetNextItemWidth(150);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50);
	this->m_search_sprite_filter.Draw("Search");
	ImGui::PopStyleVar(2);

	ImGui::EndChild();	
}

void s2d::UIHierarchy::resizeWindow()
{
	bool pop_style = false;
	if (this->m_clicked_on_resize_button)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 1));
		pop_style = true;
	}
	ImGui::SetCursorPos(ImVec2(this->m_window_size.x - 40, 0));
	s2d::FontManager::displaySmybolAsButton(ICON_FA_ARROW_RIGHT);
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
	{
		this->m_clicked_on_resize_button = true;
	}
	if (this->m_clicked_on_resize_button && ImGui::IsMouseDown(0))
	{
		float moved_x = 0;
		if (s2d::UI::s_gui_cursor.position_changed)
		{
			s2d::Vector2 moved = s2d::UI::s_gui_cursor.lastPos - s2d::UI::s_gui_cursor.position;
			moved_x = moved.x;
		}
		if (this->m_window_size.x - moved_x > 245)
		{
			this->m_window_size.x -= moved_x;
		}
	}
	else
	{
		this->m_clicked_on_resize_button = false;
	}
	if (pop_style)
	{
		ImGui::PopStyleColor(2);
	}
}

void s2d::UIHierarchy::drawbackgroundRectangle()
{
	if (this->m_sprite_background_color < 1)
	{
		this->m_sprite_background_color++;
		return;
	}
	this->m_sprite_background_color = 0;

	const ImVec2 cursorPos = ImGui::GetCursorPos();
	ImGui::SetCursorPosX(0);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 73);
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 cursor_pos = ImGui::GetCursorPos();
	ImVec2 node_size = ImVec2(100, 20); // change this to the desired size
	draw_list->AddRectFilled(cursor_pos, ImVec2(cursor_pos.x + this->m_window_size.x, cursor_pos.y - 20), IM_COL32(30, 30, 30, 255));


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
		if (this->m_search_sprite_filter.PassFilter(name.c_str()) || parent->containsChild(this->m_search_sprite_filter))
		{
			if (parent->containsChild(this->m_search_sprite_filter) && this->m_search_sprite_filter.CountGrep != 0)
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
			if (this->m_ptr_repo->sprited_hovered_in_hierarchy == nullptr)
			{
				this->setHovering(parent, anyHovered);
			}
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
