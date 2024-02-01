#include "UIHierarchy.h"

//Constructor

spe::UIHierarchy::UIHierarchy()
{
	this->m_FoundSelected = false;
	this->m_ptr_LightRepo = nullptr;
	this->m_ChildSelectTimer = 0.0f;
	this->m_FoundHovering = false;
	this->m_WaitFrame = false;
	this->Hovered = false;
	this->m_Size = HIERARCHY_DEFAULT_WINDOW_SIZE;
	this->m_BackgroundColorCnt = 1;
	this->m_BackgroundColorCnt = 1;
}

void spe::UIHierarchy::Init()
{
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
	this->m_BackgroundColorCnt = 1;
	this->m_FoundSelected = false;

	if (!ImGui::IsPopupOpen(POPUP_NAME))
	{
		this->m_FoundHovering = false;
	}

	this->RenderCloseRectangle();
	this->RenderHierarchyOptions();
	bool anyHovered = this->DisplaySprites();
	this->DisplayContextPopup();
	this->DisplayChildToParent();
	this->SetSpriteAsChild();
	this->AddPrefab();


	// Cleaning up
	this->CleanRepoSpritesUp(anyHovered);

	this->Hovered = spe::UIUtility::IsHovered(ImGui::GetWindowPos(), this->m_Size);

	ImGui::SetWindowSize(this->m_Size);
	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
	ImGui::End();
}

void spe::UIHierarchy::DisplayContextPopup()
{
	if (ImGui::IsMouseReleased(1) && this->Hovered)
	{
		ImGui::OpenPopup(POPUP_NAME);
	}

	if (ImGui::BeginPopup(POPUP_NAME))
	{
		if (ImGui::BeginMenu("Create"))
		{
			this->AddSprite();

			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Delete"))
		{
			this->DeleteSprite();
		}
		if (ImGui::MenuItem("Copy"))
		{
			this->CopySprite();
		}
		if (ImGui::MenuItem("Unparent"))
		{
			this->Unparent();
		}
		ImGui::EndPopup();
	}
}

void spe::UIHierarchy::AddSprite()
{
	if (ImGui::MenuItem("Sprite"))
	{
		const size_t vectorPos = this->m_ptr_Repo->GetAmount() + 1;
		const std::string name = "Sprite " + std::to_string(vectorPos) + " ID " + std::to_string(this->m_ptr_Repo->GetHighestId() + 1);

		spe::Vector2 pos = spe::Vector2(0, 0);

		if (this->m_ptr_GUIRepo->SpawnInCenter)
		{
			pos = spe::Vector2(this->m_ptr_GUIRepo->Camera.Position.X, this->m_ptr_GUIRepo->Camera.Position.Y * -1);
		}

		spe::Sprite* sprite = new spe::Sprite(name, pos, PATH_TO_DEFAULT_SPRITE, *this->m_ptr_LightRepo);

		this->OnSpriteAdd(sprite);
	}
}

void spe::UIHierarchy::DeleteSprite()
{
	if (this->m_ptr_GUIRepo->AnimationData.IsOpen)
	{
		return;
	}
	if (this->m_ptr_GUIRepo->InspectorSprite == nullptr)
	{
		return;
	}
	if (this->m_ptr_GUIRepo->InspectorSprite != nullptr)
	{
		if (this->m_ptr_GUIRepo->HierarchyHoveredSprite->GetId() == this->m_ptr_GUIRepo->InspectorSprite->GetId())
		{
			this->m_ptr_GUIRepo->InspectorSprite = nullptr;
		}
	}

	this->m_ptr_GUIRepo->InspectorSprite = nullptr;

	this->m_ptr_GUIRepo->EraseSpriteWithName(this->m_ptr_GUIRepo->HierarchyHoveredSprite->Name);

	this->m_ptr_Repo->DeleteWithName(this->m_ptr_GUIRepo->HierarchyHoveredSprite->Name);
	this->m_ptr_GUIRepo->HierarchyHoveredSprite = nullptr;

}

void spe::UIHierarchy::Unparent()
{
	this->m_ptr_GUIRepo->HierarchyHoveredSprite->ClearParentData();
	this->m_ptr_GUIRepo->HierarchyHoveredSprite = nullptr;
}

void spe::UIHierarchy::CleanRepoSpritesUp(bool isAnyHovered)
{
	if (!isAnyHovered && !ImGui::IsPopupOpen(POPUP_NAME) && this->m_ptr_GUIRepo->ChildToParent == nullptr)
	{
		this->m_ptr_GUIRepo->HierarchyHoveredSprite = nullptr;
	}
	if (!ImGui::IsMouseDown(0) || this->m_WaitFrame)
	{
		if (this->m_WaitFrame)
		{
			this->m_WaitFrame = false;
			this->m_ptr_GUIRepo->ChildToParent = nullptr;
		}
		else this->m_WaitFrame = true;
	}
	if (this->m_ptr_GUIRepo->AnimationData.IsOpen)
	{
		return;
	}
	if (!this->m_FoundSelected && ImGui::IsMouseReleased(0) && this->Hovered)
	{
		this->m_ptr_GUIRepo->InspectorSprite = nullptr;
	}
}

void spe::UIHierarchy::SetHovering(spe::Sprite* sprite, bool& anyHovered)
{
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped))
	{
		this->m_FoundHovering = true;
		if (ImGui::IsMouseDown(0) && this->m_ptr_GUIRepo->ChildToParent == nullptr && this->m_ptr_GUIRepo->DragAndDropPath == " ")
		{
			this->m_ChildSelectTimer += spe::Time::s_DeltaTime;

			if (this->m_ChildSelectTimer > TIME_TO_CAN_SELECT_CHILD)
			{
				this->m_ptr_GUIRepo->ChildToParent = sprite;
			}
		}
		else
		{
			this->m_ChildSelectTimer = 0.0f;
		}
		anyHovered = true;
		this->m_ptr_GUIRepo->HierarchyHoveredSprite = sprite;
	}
	else if (!this->m_FoundHovering && !ImGui::IsPopupOpen(POPUP_NAME))
	{
		this->m_ptr_GUIRepo->HierarchyHoveredSprite = nullptr;
	}
}

void spe::UIHierarchy::CopySprite()
{
	if (this->m_ptr_GUIRepo->HierarchyHoveredSprite == nullptr)
	{
		return;
	}

	spe::Sprite* to_copy = this->m_ptr_GUIRepo->HierarchyHoveredSprite;

	spe::Sprite* copy = new spe::Sprite(*to_copy);

	this->OnSpriteAdd(copy);
}

void spe::UIHierarchy::SetMenuitemHovered(bool& any_hovered, spe::Sprite* sprite)
{
	bool go_in = false;
	// Handle es child
	if (sprite->IsParent())
	{
		go_in = sprite->ContainsChild(this->m_SearchFilter) && this->m_SearchFilter.CountGrep != 0;
	}
	if (this->m_SearchFilter.PassFilter(sprite->Name.c_str())
		|| go_in)
	{
		bool pop_style = false;
		this->SetSelectedBackgroundColor(sprite, pop_style);

		std::string name = sprite->Name;
		if (sprite->Prefab.Exist)
		{
			name += " (Prefab)";
		}

		float add = 0;
		if (sprite->ptr_Parent != nullptr && !sprite->IsParent())
		{
			add = ADD_WHEN_SPRITE_HAS_PARENT;
		}
		float add_when_alone = 0;
		if (sprite->ptr_Parent == nullptr && !sprite->IsParent())
		{
			add_when_alone = 15;
		}

		this->DrawbackgroundRectangle();

		this->DrawRenderSymbol(sprite);

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + add + MENU_ITEM_PADDING);

		spe::Style::DisplaySmybolAsText(ICON_FA_CUBE);
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + add_when_alone + 75 + add, ImGui::GetCursorPosY() - 21));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::MenuItem(name.c_str());
		ImGui::PopStyleColor();

		this->SetHovering(sprite, any_hovered);
		this->SetSpriteOnClick(sprite);
		this->DrawUIRactangleWhenHovered(sprite);

		if (pop_style)
		{
			ImGui::PopStyleColor();
		}
	}
}

void spe::UIHierarchy::AddPrefab()
{
	if (this->m_ptr_GUIRepo->DragAndDropName != " "
		&& "." + spe::Utility::GetFileExtension(this->m_ptr_GUIRepo->DragAndDropName) == EXTENSION_PREFAB_FILE
		&& ImGui::IsMouseReleased(0) && this->Hovered)
	{
		spe::Sprite* sprite = spe::Initializer::InitPrefab(this->m_ptr_GUIRepo->DragAndDropPath, *this->m_ptr_LightRepo);

		if (sprite == nullptr)
		{
			spe::Log::LogString("Prefab was not in the right format for some reason");
			return;
		}

		this->OnSpriteAdd(sprite);
	}
}

void spe::UIHierarchy::RenderCloseRectangle()
{
	this->m_ptr_GUIRepo->HierarchyData.IsOpen = spe::UIUtility::RenderCloseRectangle(FOLDER_SPRITE_HIERARCHY_PADDING, ICON_FA_FILE_CODE, "##close-rectangle-hierarchy", "Hierarchy", 0);
}

void spe::UIHierarchy::RenderHierarchyOptions()
{
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
	ImGui::SetCursorPosX(0);
	ImGui::BeginChild("##hierarchy-options-container", ImVec2(this->m_Size.x, 45));

	ImGui::SetCursorPos(ImVec2(0, ImGui::GetCursorPosY() + 10));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.0f); // Set rounding to 5 pixels
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 3.0f)); // Add some padding for visual clarity
	ImGui::SetNextItemWidth(150);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50);
	this->m_SearchFilter.Draw("Search");
	ImGui::PopStyleVar(2);

	this->ResizeWindow();

	ImGui::EndChild();
}

void spe::UIHierarchy::ResizeWindow()
{
	bool pop_style = false;
	if (this->m_ResizeData.ClickedOnResizeButton)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 1));
		pop_style = true;
	}
	ImGui::SetCursorPos(ImVec2(this->m_Size.x - 40, 0));
	spe::Style::DisplaySmybolAsButton(ICON_FA_ARROW_RIGHT);
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
	{
		this->m_ResizeData.AdditionalAdd = (this->m_Size.x - spe::UIUtility::GUICursor.Position.X - 30) * -1;;
		this->m_ResizeData.ClickedOnResizeButton = true;
	}
	if (this->m_ResizeData.ClickedOnResizeButton && ImGui::IsMouseDown(0))
	{
		const float new_size = spe::UIUtility::GUICursor.Position.X - this->m_ResizeData.AdditionalAdd + 30;
		const float max_size_right = new_size + this->m_ptr_GUIRepo->InspectorData.ptr_Size->x;

		if (new_size > 245
			&& max_size_right < 1920)
		{
			this->m_Size.x = new_size;
		}
	}
	else
	{
		this->m_ResizeData.ClickedOnResizeButton = false;
	}
	if (pop_style)
	{
		ImGui::PopStyleColor(2);
	}


	this->m_Size.y = HIERARCHY_DEFAULT_WINDOW_SIZE.y - this->m_ptr_GUIRepo->AssetFolderData.ptr_Size->y + (HIERARCHY_DEFAULT_WINDOW_SIZE.x - 10);
}

void spe::UIHierarchy::DrawbackgroundRectangle()
{
	if (this->m_BackgroundColorCnt < 1)
	{
		this->m_BackgroundColorCnt++;
		return;
	}
	this->m_BackgroundColorCnt = 0;

	const ImVec2 temp = ImGui::GetCursorPos();
	ImGui::SetCursorPosX(0);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 80.5f - ImGui::GetScrollY());
	spe::UIUtility::DrawRectangleInGUIWIndow(
		ImVec2(this->m_Size.x, 20), ImGui::GetCursorPos(), SPRITE_BACKGROUND_COLOR);
	ImGui::SetCursorPos(temp);
}

void spe::UIHierarchy::SetSpriteOnClick(spe::Sprite* sprite)
{
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped)
		&& ImGui::IsMouseReleased(0))
	{
		this->m_FoundSelected = true;
		if (this->m_ptr_GUIRepo->AnimationData.IsOpen)
		{
			return;
		}
		this->m_ptr_GUIRepo->InspectorSprite = sprite;
	}
}

void spe::UIHierarchy::DrawUIRactangleWhenHovered(spe::Sprite* sprite)
{
	if (this->m_ptr_GUIRepo->AnimationData.IsOpen)
	{
		return;
	}
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped)
		|| this->m_ptr_GUIRepo->HierarchyHoveredSprite != nullptr
		&& this->m_ptr_GUIRepo->HierarchyHoveredSprite->GetId() == sprite->GetId())
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

void spe::UIHierarchy::DrawRenderSymbol(spe::Sprite* child)
{
	const ImVec2 cursor = ImGui::GetCursorPos();
	ImGui::SetCursorPos(ImVec2(17, cursor.y - 5));
	const std::string icon = child->SpriteRenderer.Render ? ICON_FA_EYE : ICON_FA_EYE_SLASH;
	const std::string name = icon + std::string("##") + std::to_string(child->GetId());
	if (spe::Style::DisplaySmybolAsButton(name.c_str()))
	{
		child->SpriteRenderer.Render = !child->SpriteRenderer.Render;
	}
	ImGui::SetCursorPos(cursor);
}

void spe::UIHierarchy::OnSpriteAdd(spe::Sprite* spr)
{
	this->m_ptr_Repo->Add(spr);
	this->m_ptr_GUIRepo->HierarchySprites.push_back(spr);
}

void spe::UIHierarchy::SetSelectedBackgroundColor(spe::Sprite* sprite, bool& pop_style)
{
	if (this->m_ptr_GUIRepo->InspectorSprite != nullptr
		&& this->m_ptr_GUIRepo->InspectorSprite->GetId() == sprite->GetId())
	{
		// Set color in Hirarchy
		pop_style = true;
		ImGui::PushStyleColor(ImGuiCol_Text, SPRITE_SELECTED_COLOR);
	}
}

bool spe::UIHierarchy::DisplaySprites()
{
	bool anyHoverd = false;

	for (size_t i = 0; i < this->m_ptr_GUIRepo->HierarchySprites.size(); i++)
	{
		this->DisplaySprites(this->m_ptr_GUIRepo->HierarchySprites[i], anyHoverd);

	}

	return anyHoverd;
}

void spe::UIHierarchy::DisplaySprites(spe::Sprite* parent, bool& any_hovered)
{
	if (parent->ptr_Parent != nullptr)
	{
		return;
	}
	// Handle as parent
	if (parent->IsParent())
	{
		this->DisplaySpriteSeperated(parent, any_hovered);
		return;
	}
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
	// Set hovered sprite
	this->SetMenuitemHovered(any_hovered, parent);
}

void spe::UIHierarchy::DisplaySpriteSeperated(spe::Sprite* parent, bool& any_hovered)
{
	if (parent->IsParent())
	{
		std::string name = parent->Name;
		if (parent->Prefab.Exist)
		{
			name += " (Prefab)";
		}
		const bool contains_child = (parent->ContainsChild(this->m_SearchFilter) && this->m_SearchFilter.CountGrep != 0) || parent->ContainsChild(this->m_ptr_GUIRepo->InspectorSprite);
		if (this->m_SearchFilter.PassFilter(name.c_str()) || contains_child)
		{
			bool entered_node = false;
			bool pop_style_tree_node = true;
			bool pop_style = false;

			this->SetSelectedBackgroundColor(parent, pop_style);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));
			this->DrawbackgroundRectangle();
			this->DrawRenderSymbol(parent);

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
				this->SetSpriteOnClick(parent);
				this->DrawUIRactangleWhenHovered(parent);
				this->SetHovering(parent, any_hovered);

				for (size_t i = 0; i < parent->ptr_Childs.size(); i++)
				{
					spe::Sprite* child = parent->ptr_Childs[i];
					this->DisplaySpriteSeperated(child, any_hovered);
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
				this->SetHovering(parent, any_hovered);
				this->SetSpriteOnClick(parent);
				this->DrawUIRactangleWhenHovered(parent);
			}
		}
		return;
	}

	// Set hovered sprite
	this->SetMenuitemHovered(any_hovered, parent);
}

void spe::UIHierarchy::DisplayChildToParent()
{
	if (this->m_ptr_GUIRepo->ChildToParent == nullptr)
	{
		return;
	}

	const ImVec2 window_pos = ImGui::GetMousePos();
	ImGui::Begin("##DragChild", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
	ImGui::Text(this->m_ptr_GUIRepo->ChildToParent->Name.c_str());
	ImGui::SetWindowPos(ImVec2(window_pos.x - 15, window_pos.y - 15));
	ImGui::End();
}

void spe::UIHierarchy::SetSpriteAsChild()
{
	if (ImGui::IsMouseReleased(0) && this->m_ptr_GUIRepo->HierarchyHoveredSprite != nullptr
		&& this->m_ptr_GUIRepo->ChildToParent != nullptr
		&& this->m_ptr_GUIRepo->ChildToParent->Name != this->m_ptr_GUIRepo->HierarchyHoveredSprite->Name
		//&& !this->m_ptr_GUIRepo->sprited_hovered_in_hierarchy->containsChild(this->m_ptr_GUIRepo->child_to_parent)
		&& !this->m_ptr_GUIRepo->ChildToParent->ContainsChild(this->m_ptr_GUIRepo->HierarchyHoveredSprite))

	{
		this->m_ptr_GUIRepo->ChildToParent->SetParent(this->m_ptr_GUIRepo->HierarchyHoveredSprite);
		this->m_ptr_GUIRepo->ChildToParent = nullptr;
	}
}
