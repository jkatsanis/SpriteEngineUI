#include "UIHierarchy.h"

//Constructor

void spe::UIHierarchy::Init()
{
	this->m_child_select_timer = 0.0f;
	this->m_found_hovering = false;
	this->m_wait_one_frame = false;
	this->Hovered = false;
	this->m_Size = HIERARCHY_DEFAULT_WINDOW_SIZE;
	this->m_sprite_background_color_cnt = 1;

	this->m_ptr_GUIRepo->HierarchyData.ptr_Size = &this->m_Size;
}

//Public functions

void spe::UIHierarchy::Render()
{
	// Set window size if asset folder does not render

	if (spe::UIUtility::HandleCloseAndReloadWindow(this->m_ptr_GUIRepo->HierarchyData, this->Hovered, HIERARCHY_DEFAULT_WINDOW_SIZE))
	{
		return;
	}
	
	ImGui::SetNextWindowPos(ImVec2(0, 60));
	ImGui::Begin("##ui-hierarchy", NULL, DEFAULT_FLAGS);

	// Render Hierarchy
	this->m_sprite_background_color_cnt = 1;
	this->m_found_selected = false;

	if (!ImGui::IsPopupOpen(POPUP_NAME))
	{
		this->m_found_hovering = false;
	}

	this->renderCloseRectangle();
	this->renderHierarchyOptions();
	bool anyHovered = this->displaySprites();
	this->displayContextPopup();
	this->displayChildToParent();
	this->setSpriteAsChild();
	this->addPrefab();


	// Cleaning up
	this->cleanRepoSpritesUp(anyHovered);

	this->Hovered = spe::UIUtility::IsHovered(ImGui::GetWindowPos(), this->m_Size);

	ImGui::SetWindowSize(this->m_Size);
	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
	ImGui::End();
}

void spe::UIHierarchy::displayContextPopup()
{
	if (ImGui::IsMouseReleased(1) && this->Hovered)
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
		if (ImGui::MenuItem("Copy"))
		{
			this->copySprite();
		}
		ImGui::EndPopup();
	}
}

void spe::UIHierarchy::addSprite()
{
	if (ImGui::MenuItem("Sprite"))
	{
		const size_t vectorPos = this->m_ptr_Repo->GetAmount() + 1;
		const std::string name = "Sprite " + std::to_string(vectorPos) + " ID " + std::to_string(this->m_ptr_Repo->GetHighestId() + 1);

		// TODO: Change to valid path
		spe::Sprite* sprite = new spe::Sprite(name, spe::Vector2(0, 0), "");

		this->m_ptr_Repo->Add(sprite);
	}
}

void spe::UIHierarchy::deleteSprite()
{
	if (this->m_ptr_GUIRepo->AnimationData.IsOpen)
	{
		return;
	}
	if (this->m_ptr_GUIRepo->sprite_in_inspector == nullptr)
	{
		return;
	}
	if (this->m_ptr_GUIRepo->sprite_in_inspector != nullptr)
	{
		if (this->m_ptr_GUIRepo->sprited_hovered_in_hierarchy->getId() == this->m_ptr_GUIRepo->sprite_in_inspector->getId())
		{
			this->m_ptr_GUIRepo->sprite_in_inspector = nullptr;
		}
	}

	this->m_ptr_GUIRepo->sprite_in_inspector = nullptr;

	this->m_ptr_Repo->DeleteWithName(this->m_ptr_GUIRepo->sprited_hovered_in_hierarchy->name);
	this->m_ptr_GUIRepo->sprited_hovered_in_hierarchy = nullptr;
}

void spe::UIHierarchy::cleanRepoSpritesUp(bool isAnyHovered)
{
	if (!isAnyHovered && !ImGui::IsPopupOpen(POPUP_NAME) && this->m_ptr_GUIRepo->child_to_parent == nullptr)
	{
		this->m_ptr_GUIRepo->sprited_hovered_in_hierarchy = nullptr;
	}
	if (!ImGui::IsMouseDown(0) || this->m_wait_one_frame)
	{
		if (this->m_wait_one_frame)
		{
			this->m_wait_one_frame = false;
			this->m_ptr_GUIRepo->child_to_parent = nullptr;
		}
		else this->m_wait_one_frame = true;
	}
	if (this->m_ptr_GUIRepo->AnimationData.IsOpen)
	{
		return;
	}
	if (!this->m_found_selected && ImGui::IsMouseReleased(0) && this->Hovered)
	{
		this->m_ptr_GUIRepo->sprite_in_inspector = nullptr;
	}
}

void spe::UIHierarchy::setHovering(spe::Sprite* sprite, bool& anyHovered)
{
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped))
	{
		this->m_found_hovering = true;
		if (ImGui::IsMouseDown(0) && this->m_ptr_GUIRepo->child_to_parent == nullptr && this->m_ptr_GUIRepo->DragAndDropPath == " ")
		{
			this->m_child_select_timer += spe::Time::s_delta_time;

			if (this->m_child_select_timer > TIME_TO_CAN_SELECT_CHILD)
			{
				this->m_ptr_GUIRepo->child_to_parent = sprite;
			}
		}
		else
		{
			this->m_child_select_timer = 0.0f;
		}
		anyHovered = true;
		this->m_ptr_GUIRepo->sprited_hovered_in_hierarchy = sprite;
	}
	else if (!this->m_found_hovering && !ImGui::IsPopupOpen(POPUP_NAME))
	{
		this->m_ptr_GUIRepo->sprited_hovered_in_hierarchy = nullptr;
	}
}

void spe::UIHierarchy::copySprite()
{
	if (this->m_ptr_GUIRepo->sprited_hovered_in_hierarchy == nullptr)
	{
		return;
	}

	spe::Sprite* to_copy = this->m_ptr_GUIRepo->sprited_hovered_in_hierarchy;

	spe::Sprite* copy = new spe::Sprite(*to_copy);

	this->m_ptr_Repo->Add(copy);
}

void spe::UIHierarchy::setMenuitemHovered(bool& any_hovered, spe::Sprite* sprite)
{
	bool go_in = false;
	// Handle es child
	if (sprite->IsParent())
	{
		go_in = sprite->ContainsChild(this->m_search_sprite_filter) && this->m_search_sprite_filter.CountGrep != 0;
	}
	if (this->m_search_sprite_filter.PassFilter(sprite->name.c_str())
		|| go_in)
	{
		bool pop_style = false;
		this->setSelectedBackgroundColor(sprite, pop_style);

		std::string name = sprite->name;
		if (sprite->prefab.exist)
		{
			name += " (Prefab)";
		}

		float add = 0;
		if (sprite->parent != nullptr && !sprite->IsParent())
		{
			add = ADD_WHEN_SPRITE_HAS_PARENT;
		}
		float add_when_alone = 0;
		if (sprite->parent == nullptr && !sprite->IsParent())
		{
			add_when_alone = 15;
		}

		this->drawbackgroundRectangle();

		this->drawRenderSymbol(sprite);

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + add + MENU_ITEM_PADDING);

		spe::Style::DisplaySmybolAsText(ICON_FA_CUBE);
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

void spe::UIHierarchy::addPrefab()
{
	if (this->m_ptr_GUIRepo->DragAndDropName != " "
		&& "." + spe::Utility::GetFileExtension(this->m_ptr_GUIRepo->DragAndDropName) == EXTENSION_PREFAB_FILE
		&& ImGui::IsMouseReleased(0) && this->Hovered)
	{
		// TODO
		// this->m_ptr_Repo->instanitatePrefab(this->m_ptr_Repo->asset_folder_data.drag_and_drop_path);
	}
}

void spe::UIHierarchy::renderCloseRectangle()
{
	this->m_ptr_GUIRepo->HierarchyData.IsOpen = spe::UIUtility::RenderCloseRectangle(FOLDER_SPRITE_HIERARCHY_PADDING, ICON_FA_FILE_CODE, "##close-rectangle-hierarchy", "Hierarchy", 0);
}

void spe::UIHierarchy::renderHierarchyOptions()
{
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
	ImGui::SetCursorPosX(0);
	ImGui::BeginChild("##hierarchy-options-container", ImVec2(this->m_Size.x, 45));

	ImGui::SetCursorPos(ImVec2(0, ImGui::GetCursorPosY() + 10));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.0f); // Set rounding to 5 pixels
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 3.0f)); // Add some padding for visual clarity
	ImGui::SetNextItemWidth(150);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50);
	this->m_search_sprite_filter.Draw("Search");
	ImGui::PopStyleVar(2);

	this->resizeWindow();

	ImGui::EndChild();
}

void spe::UIHierarchy::resizeWindow()
{
	bool pop_style = false;
	if (this->m_resize_window_data.clicked_on_resize_button)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 1));
		pop_style = true;
	}
	ImGui::SetCursorPos(ImVec2(this->m_Size.x - 40, 0));
	spe::Style::DisplaySmybolAsButton(ICON_FA_ARROW_RIGHT);
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
	{
		this->m_resize_window_data.additinal_add = (this->m_Size.x - spe::UIUtility::GUICursor.x - 30) * -1;;
		this->m_resize_window_data.clicked_on_resize_button = true;
	}
	if (this->m_resize_window_data.clicked_on_resize_button && ImGui::IsMouseDown(0))
	{
		const float new_size = spe::UIUtility::GUICursor.x - this->m_resize_window_data.additinal_add + 30;
		const float max_size_right = new_size +  200; // TODO

		if (new_size > 245
			&& max_size_right < 1920)
		{
			this->m_Size.x = new_size;
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

void spe::UIHierarchy::drawbackgroundRectangle()
{
	if (this->m_sprite_background_color_cnt < 1)
	{
		this->m_sprite_background_color_cnt++;
		return;
	}
	this->m_sprite_background_color_cnt = 0;

	const ImVec2 temp = ImGui::GetCursorPos();
	ImGui::SetCursorPosX(0);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 80.5f - ImGui::GetScrollY());
	spe::UIUtility::DrawRectangleInGUIWIndow(
		ImVec2(this->m_Size.x, 20), ImGui::GetCursorPos(), SPRITE_BACKGROUND_COLOR);
	ImGui::SetCursorPos(temp);
}

void spe::UIHierarchy::setSpriteOnClick(spe::Sprite* sprite)
{
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped)
		&& ImGui::IsMouseReleased(0))
	{
		this->m_found_selected = true;
		if (this->m_ptr_GUIRepo->AnimationData.IsOpen)
		{
			return;
		}
		this->m_ptr_GUIRepo->sprite_in_inspector = sprite;
	}
}

void spe::UIHierarchy::drawUIRactangleWhenHovered(spe::Sprite* sprite)
{
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped)
		|| this->m_ptr_GUIRepo->sprited_hovered_in_hierarchy != nullptr
		&& this->m_ptr_GUIRepo->sprited_hovered_in_hierarchy->getId() == sprite->getId())
	{
		const ImVec2 temp = ImGui::GetCursorPos();

		ImGui::SetCursorPosX(0);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 57);
		const ImVec2 cursor_pos = ImVec2(ImGui::GetCursorPosX(),
			ImGui::GetCursorPosY() - ImGui::GetScrollY());
		spe::UIUtility::DrawRectangleInGUIWIndow(ImVec2(this->m_Size.x, 20),
			cursor_pos, ImColor(50.0f, 50.0f, 50.0f, 0.3f));

		ImGui::SetCursorPos(temp);
	}
}

void spe::UIHierarchy::drawRenderSymbol(spe::Sprite* child)
{
	const ImVec2 cursor = ImGui::GetCursorPos();
	ImGui::SetCursorPos(ImVec2(17, cursor.y - 5));
	const std::string icon = child->sprite_renderer.render ? ICON_FA_EYE : ICON_FA_EYE_SLASH;
	const std::string name = icon + std::string("##") + std::to_string(child->getId());
	if (spe::Style::DisplaySmybolAsButton(name.c_str()))
	{
		child->sprite_renderer.render = !child->sprite_renderer.render;
	}
	ImGui::SetCursorPos(cursor);
}

void spe::UIHierarchy::setSelectedBackgroundColor(spe::Sprite* sprite, bool& pop_style)
{
	if (this->m_ptr_GUIRepo->sprite_in_inspector != nullptr
		&& this->m_ptr_GUIRepo->sprite_in_inspector->getId() == sprite->getId())
	{
		// Set color in Hirarchy
		pop_style = true;
		ImGui::PushStyleColor(ImGuiCol_Text, SPRITE_SELECTED_COLOR);
	}
}

bool spe::UIHierarchy::displaySprites()
{
	bool anyHoverd = false;

	std::list<spe::Sprite*>& sprites = this->m_ptr_Repo->GetSprites();

	for (auto it = sprites.begin(); it != sprites.end(); ++it)
	{
		spe::Sprite* sprite = *it;
		this->displaySprites(sprite, anyHoverd);

	}

	return anyHoverd;
}

void spe::UIHierarchy::displaySprites(spe::Sprite* parent, bool& any_hovered)
{
	if (parent->parent != nullptr)
	{
		return;
	}
	// Handle as parent
	if (parent->IsParent())
	{
		this->displaySpriteSeperated(parent, any_hovered);
		return;
	}
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
	// Set hovered sprite
	this->setMenuitemHovered(any_hovered, parent);
}

void spe::UIHierarchy::displaySpriteSeperated(spe::Sprite* parent, bool& any_hovered)
{
	if (parent->IsParent())
	{
		std::string name = parent->name;
		if (parent->prefab.exist)
		{
			name += " (Prefab)";
		}
		const bool contains_child = (parent->ContainsChild(this->m_search_sprite_filter) && this->m_search_sprite_filter.CountGrep != 0) || parent->ContainsChild(this->m_ptr_GUIRepo->sprite_in_inspector);
		if (this->m_search_sprite_filter.PassFilter(name.c_str()) || contains_child)
		{
			bool entered_node = false;
			bool pop_style_tree_node = true;
			bool pop_style = false;

			this->setSelectedBackgroundColor(parent, pop_style);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));
			this->drawbackgroundRectangle();
			this->drawRenderSymbol(parent);

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + TREE_NODE_PADDING);
			if (contains_child)
			{
				ImGui::SetNextItemOpen(true);
			}
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
					spe::Sprite* child = parent->ptr_childs[i];
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

void spe::UIHierarchy::displayChildToParent()
{
	if (this->m_ptr_GUIRepo->child_to_parent == nullptr)
	{
		return;
	}

	const ImVec2 window_pos = ImGui::GetMousePos();
	ImGui::Begin("##DragChild", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
	ImGui::Text(this->m_ptr_GUIRepo->child_to_parent->name.c_str());
	ImGui::SetWindowPos(ImVec2(window_pos.x - 15, window_pos.y - 15));
	ImGui::End();
}

void spe::UIHierarchy::setSpriteAsChild()
{
	if (ImGui::IsMouseReleased(0) && this->m_ptr_GUIRepo->sprited_hovered_in_hierarchy != nullptr
		&& this->m_ptr_GUIRepo->child_to_parent != nullptr
		&& this->m_ptr_GUIRepo->child_to_parent->name != this->m_ptr_GUIRepo->sprited_hovered_in_hierarchy->name
		//&& !this->m_ptr_GUIRepo->sprited_hovered_in_hierarchy->containsChild(this->m_ptr_GUIRepo->child_to_parent)
		&& !this->m_ptr_GUIRepo->child_to_parent->ContainsChild(this->m_ptr_GUIRepo->sprited_hovered_in_hierarchy))

	{
		this->m_ptr_GUIRepo->child_to_parent->setParent(this->m_ptr_GUIRepo->sprited_hovered_in_hierarchy);
		this->m_ptr_GUIRepo->child_to_parent = nullptr;
	}
}
