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
	this->m_found_hovering = false;
	this->m_wait_one_frame = false;
	this->is_hovered = false;
	this->m_window_size = HIERARCHY_DEFAULT_WINDOW_SIZE;
	this->m_sprite_background_color_cnt = 1;
}

//Public functions

void s2d::UIHierarchy::displayHierarchyWindow()
{
	// Set window size if asset folder does not render
	this->m_window_size = (s2d::UIInfo::s_is_asset_folder_open.is_open)
		? ImVec2(this->m_window_size.x, WINDOW_SIZE_HIERARCHY_Y - this->m_ptr_gui_repo->ptr_asset_window_size->y)
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
	this->m_sprite_background_color_cnt = 1;
	this->m_found_selected = false;

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
	this->addPrefab();

	// Cleaning up
	this->cleanRepoSpritesUp(anyHovered);

	this->is_hovered = s2d::UI::isHovered(WINDOW_POS, this->m_window_size);

	ImGui::SetWindowPos(WINDOW_POS);
	ImGui::SetWindowSize(this->m_window_size);
	ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size);
	ImGui::End();
}

void s2d::UIHierarchy::setGUIRepo(s2d::GUIRepository* repo)
{
	this->m_ptr_gui_repo = repo;
	this->m_ptr_gui_repo->ptr_hierarchy_window_size = &this->m_window_size;
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

		s2d::Sprite* sprite = new s2d::Sprite(name, s2d::Vector2(0, 0), s2d::SpriteData::s_default_sprite_path);

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
	if (!this->m_found_selected && ImGui::IsMouseReleased(0) && this->is_hovered)
	{
		this->m_ptr_repo->sprite_in_inspector = nullptr;
	}
}

void s2d::UIHierarchy::setHovering(s2d::Sprite* sprite, bool& anyHovered)
{
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped))
	{
		this->m_found_hovering = true;
		if (ImGui::IsMouseDown(0) && this->m_ptr_repo->child_to_parent == nullptr && this->m_ptr_repo->asset_folder_data.drag_and_drop_path == " ")
		{
			this->m_child_select_timer += s2d::Time::s_delta_time;

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
	bool go_in = false;
	// Handle es child
	if (sprite->isParent())
	{
		go_in = sprite->containsChild(this->m_search_sprite_filter) && this->m_search_sprite_filter.CountGrep != 0;
	}
	if (this->m_search_sprite_filter.PassFilter(sprite->name.c_str())
		|| go_in)
	{
		bool pop_style = false;
		this->setSelectedBackgroundColor(sprite, pop_style);

		std::string name = sprite->name;
		if (sprite->prefab.exists)
		{
			name += " (Prefab)";
		}

		float add = 0;
		if (sprite->parent != nullptr && !sprite->isParent())
		{
			add = ADD_WHEN_SPRITE_HAS_PARENT;
		}
		float add_when_alone = 0;
		if (sprite->parent == nullptr && !sprite->isParent())
		{
			add_when_alone = 15;
		}

		this->drawbackgroundRectangle();

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + add + MENU_ITEM_PADDING);
		s2d::FontManager::displaySmybolAsText(ICON_FA_CUBE);
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + add_when_alone + 75 + add, ImGui::GetCursorPosY() - 21));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::MenuItem(name.c_str());
		ImGui::PopStyleColor();

		this->setHovering(sprite, any_hovered);
		this->setSpriteOnClick(sprite);
		this->drawUIRactangleWhenHovered(sprite);

		if (pop_style)
		{
			ImGui::PopStyleColor();
		}
	}
}

void s2d::UIHierarchy::addPrefab()
{
	if (this->m_ptr_repo->asset_folder_data.darg_and_drop_name != " "
		&& "." + std::getFileExtension(this->m_ptr_repo->asset_folder_data.darg_and_drop_name) == EXTENSION_PREFAB_FILE
		&& ImGui::IsMouseReleased(0) && this->is_hovered)
	{
	
		this->m_ptr_repo->instanitatePrefab(this->m_ptr_repo->asset_folder_data.drag_and_drop_path);
	}
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
	if (this->m_resize_window_data.clicked_on_resize_button)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 1));
		pop_style = true;
	}
	ImGui::SetCursorPos(ImVec2(this->m_window_size.x - 40, 0));
	s2d::FontManager::displaySmybolAsButton(ICON_FA_ARROW_RIGHT);
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
	{
		this->m_resize_window_data.additinal_add = (this->m_window_size.x - s2d::UI::s_gui_cursor.position.x - 30) * -1;;
		this->m_resize_window_data.clicked_on_resize_button = true;
	}
	if (this->m_resize_window_data.clicked_on_resize_button && ImGui::IsMouseDown(0))
	{
		const float new_size = s2d::UI::s_gui_cursor.position.x - this->m_resize_window_data.additinal_add + 30;
		const float max_size_right = new_size + this->m_ptr_gui_repo->ptr_inspector_window_size->x;

		if (new_size > 245
			&& max_size_right < 1920)
		{
			this->m_window_size.x = new_size;
		}
	}
	else
	{
		this->m_resize_window_data.clicked_on_resize_button = false;
	}
	if (pop_style)
	{
		ImGui::PopStyleColor(2);
	}
}

void s2d::UIHierarchy::drawbackgroundRectangle()
{
	if (this->m_sprite_background_color_cnt < 1)
	{
		this->m_sprite_background_color_cnt++;
		return;
	}
	this->m_sprite_background_color_cnt = 0;

	const ImVec2 temp = ImGui::GetCursorPos();
	ImGui::SetCursorPosX(0);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 73);
	s2d::UI::drawRectangleInGUIWIndow(
		ImVec2(this->m_window_size.x, 20), ImGui::GetCursorPos(), SPRITE_BACKGROUND_COLOR);
	ImGui::SetCursorPos(temp);
}

void s2d::UIHierarchy::setSpriteOnClick(s2d::Sprite* sprite)
{
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped)
		&& ImGui::IsMouseReleased(0))
	{
		this->m_found_selected = true;
		this->m_ptr_repo->sprite_in_inspector = sprite;
	}
}

void s2d::UIHierarchy::drawUIRactangleWhenHovered(s2d::Sprite* sprite)
{
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped)
		|| this->m_ptr_repo->sprited_hovered_in_hierarchy != nullptr
		&& this->m_ptr_repo->sprited_hovered_in_hierarchy->getId() == sprite->getId())
	{
		const ImVec2 temp = ImGui::GetCursorPos();

		ImGui::SetCursorPosX(0);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 50);
		s2d::UI::drawRectangleInGUIWIndow(ImVec2(this->m_window_size.x, 20),
			ImGui::GetCursorPos(), ImColor(50.0f, 50.0f, 50.0f, 0.3f));

		ImGui::SetCursorPos(temp);
	}
}

void s2d::UIHierarchy::setSelectedBackgroundColor(s2d::Sprite* sprite, bool& pop_style)
{
	if (this->m_ptr_repo->sprite_in_inspector != nullptr
		&& this->m_ptr_repo->sprite_in_inspector->getId() == sprite->getId())
	{
		// Set color in Hirarchy
		pop_style = true;
		ImGui::PushStyleColor(ImGuiCol_Text, SPRITE_SELECTED_COLOR);
	}
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
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
	// Set hovered sprite
	this->setMenuitemHovered(any_hovered, parent);
}

void s2d::UIHierarchy::displaySpriteSeperated(s2d::Sprite* parent, bool& any_hovered)
{
	if (parent->isParent())
	{
		std::string name = parent->name;
		if (parent->prefab.exists)
		{
			name += " (Prefab)";
		}
		if (this->m_search_sprite_filter.PassFilter(name.c_str()) || parent->containsChild(this->m_search_sprite_filter))
		{
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + TREE_NODE_PADDING);
			bool entered_node = false;
			if (parent->containsChild(this->m_search_sprite_filter) && this->m_search_sprite_filter.CountGrep != 0)
			{
				ImGui::SetNextItemOpen(true);
			}
			bool pop_style_tree_node = true;
			bool pop_style = false;
			this->setSelectedBackgroundColor(parent, pop_style);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));
			this->drawbackgroundRectangle();
			if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow))
			{
				// Poppping background
				ImGui::PopStyleColor();
				pop_style_tree_node = false;

				if (pop_style)
				{
					ImGui::PopStyleColor();
					pop_style = false;
				}
				entered_node = true;
				this->setSpriteOnClick(parent);
				this->drawUIRactangleWhenHovered(parent);
				this->setHovering(parent, any_hovered);

				for (size_t i = 0; i < parent->ptr_childs.size(); i++)
				{
					s2d::Sprite* child = parent->ptr_childs[i];
					this->displaySpriteSeperated(child, any_hovered);
				}
				ImGui::TreePop();
			}
			if (pop_style)
			{
				ImGui::PopStyleColor();
			}
			if (pop_style_tree_node)
			{
				ImGui::PopStyleColor();
			}
			if (!entered_node)
			{
				this->setHovering(parent, any_hovered);
				this->setSpriteOnClick(parent);
				this->drawUIRactangleWhenHovered(parent);
			}
		}
		return;
	}

	// Set hovered sprite
	this->setMenuitemHovered(any_hovered, parent);
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
