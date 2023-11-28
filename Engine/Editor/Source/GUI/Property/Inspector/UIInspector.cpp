#include "UIInspector.h"
#include "UIInspector.h"


void spe::UIInspector::Init()
{
	this->m_sprite_input_width = 200.0f;
	this->m_window_size_width = 390.0f;

	this->m_menu_name = "menu";

	this->m_Size = INSPECTOR_DEFAULT_WINDOW_SIZE;
	this->state = spe::InspectorState::None;

	spe::Rectangle* rec = new spe::Rectangle(sf::Vector2f(0, 0), sf::Vector2f(0, 0), sf::Color(124, 252, 0), 3.5f,
		PATH_TO_TRANSPARENT_PIC, "collider-over-sprite");

	this->m_ptr_GUIRepo->Add(rec);

	spe::Rectangle* over_sprite = new spe::Rectangle(sf::Vector2f(0, 0), sf::Vector2f(0, 0), sf::Color(0, 0, 0), 3.5f,
		PATH_TO_TRANSPARENT_PIC, "texture-over-sprite");

	this->m_ptr_GUIRepo->Add(over_sprite);

	this->m_ptr_collider_rectangle = this->m_ptr_GUIRepo->GetByName("collider-over-sprite");
	this->m_ptr_sprite_over_rectangle = this->m_ptr_GUIRepo->GetByName("texture-over-sprite");

	this->m_ptr_collider_rectangle->SortingLayerIdx = 1;
	this->m_ptr_GUIRepo->UpdateLayerIndex();

	this->m_collider.initScaleDottsUI(*this->m_ptr_GUIRepo);

	this->m_components.push_back("BoxCollider");
	this->m_components.push_back("PhysicsBody");
	this->m_components.push_back("Animator");
	this->m_components.push_back("Light Source");

	this->m_ptr_GUIRepo->InspectorData.ptr_Size = &this->m_Size;

	this->m_tag_selector.SetRepos(this->m_ptr_Repo, this->m_ptr_GUIRepo);
}

//Private functions

void spe::UIInspector::renderComponentOptions(spe::Component& component, const std::string& name)
{
	const std::string button_name = std::string(ICON_FA_COG) + "##" + name;
	const ImVec2 temp_pos = ImGui::GetCursorPos();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY());
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - 40);

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 2));
	if (spe::Style::DisplaySmybolAsButton(button_name.c_str()))
	{
		ImGui::OpenPopup(button_name.c_str());
		this->m_pop_up_cursor_pos = ImVec2(spe::UIUtility::GUICursor.Position.x - 150, spe::UIUtility::GUICursor.Position.y + 20);
	}

	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize + 0.2f);

	ImGui::SetNextWindowPos(this->m_pop_up_cursor_pos);
	if (ImGui::BeginPopup(button_name.c_str()))
	{
		if (spe::Style::DisplaySymbolInMenuItemWithText(ICON_FA_RETWEET, "Reset", 30))
		{
			component.reset();
			component.exist = true;
		}
		if (!component.base_component)
		{
			if (spe::Style::DisplaySymbolInMenuItemWithText(ICON_FA_TRASH, "Delete", 30))
			{
				spe::Light* childPtr = dynamic_cast<Light*>(&component);
				if (childPtr) {
					childPtr->deleteLight();
				}

				component.reset();
				component.exist = false;
			}
		}
		ImGui::EndPopup();
	}
	
	ImGui::PopStyleVar();
	ImGui::SetCursorPos(temp_pos);

	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
}

void spe::UIInspector::generalSettings()
{
	this->renderBackgroundBehindComponent();
	if (ImGui::TreeNode("General"))
	{
		ImGui::Dummy(ImVec2(0, 16));

		if (ImGui::Button("Reaload Textures"))
		{
			this->m_ptr_Repo->ReloadTextures();
		}
		ImGui::TreePop();
	}
}

void spe::UIInspector::drawRectangleOverCurrentObject()
{
	this->m_ptr_sprite_over_rectangle->Render = true;

	sf::RectangleShape* ptr_shape = &this->m_ptr_sprite_over_rectangle->Shape;
	
	ptr_shape->setSize(sf::Vector2f(this->m_ptr_GUIRepo->sprite_in_inspector->transform.texture_size.x, this->m_ptr_GUIRepo->sprite_in_inspector->transform.texture_size.y));
	ptr_shape->setPosition(this->m_ptr_GUIRepo->sprite_in_inspector->transform.getOrigininalPosition().x, this->m_ptr_GUIRepo->sprite_in_inspector->transform.getOrigininalPosition().y);
}

void spe::UIInspector::renderOptions()
{
	const ImVec2 tempCursor = ImGui::GetCursorPos();

	this->resizeWindow();

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
	ImGui::SetCursorPosX(0);

	spe::UIUtility::DrawRectangleInGUIWIndow(
		ImVec2(this->m_Size.x + 50, 45), ImVec2(1900 - this->m_Size.x, 130), ImColor(26, 26, 26, 255));
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 50);
}

void spe::UIInspector::resizeWindow()
{
	bool pop_style = false;
	ImGui::SetCursorPosX(10);
	ImGui::SetCursorPosY(4);
	if (this->m_resize_window_data.clicked_on_resize_button)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 1));
		pop_style = true;
	}
	spe::Style::DisplaySmybolAsButton(ICON_FA_ARROW_LEFT);
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
	{
		this->m_resize_window_data.clicked_on_resize_button = true;
	}
	if (this->m_resize_window_data.clicked_on_resize_button && ImGui::IsMouseDown(0))
	{
		float movedy = 0;
		if (spe::UIUtility::GUICursor.PositionChanged)
		{
			spe::Vector2 moved = spe::UIUtility::GUICursor.LastPosition - spe::UIUtility::GUICursor.Position;
			movedy = moved.x;
		}
		if (this->m_Size.x + movedy > 350
			&& this->m_Size.x + movedy + this->m_ptr_GUIRepo->HierarchyData.ptr_Size->x < 1920)
		{
			this->m_Size.x += movedy;
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

void spe::UIInspector::renderBackgroundBehindComponent()
{
	const ImVec2 temp = ImGui::GetCursorPos();
	ImGui::SetCursorPosX(1920 - this->m_Size.x);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 76);
	spe::UIUtility::DrawRectangleInGUIWIndow(ImVec2(this->m_Size.x, 27), ImGui::GetCursorPos(), COMPONENT_SELECTED_COLOR);
	ImGui::SetCursorPos(temp);
}

void spe::UIInspector::displayDefaultInspectorView()
{
	ImGui::Dummy(ImVec2(0, 15));
	this->backgroundSetting();
	DUMMY_COMPONENT;
	this->gameEngineViewSetting();
	DUMMY_COMPONENT;
	this->generalSettings();


	const ImVec2 temp = ImGui::GetCursorPos();
	ImGui::SetCursorPosY(43.0f);
	ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - SEARCH_BAR_MARGIN);
	ImGui::SetNextItemWidth(150);
	ImGui::Text("Game Window");

	ImGui::SetCursorPos(temp);
}

void spe::UIInspector::backgroundSetting()
{
	this->renderBackgroundBehindComponent();
	if (ImGui::TreeNode("Background"))
	{
		ImGui::Dummy(ImVec2(0, 16));

		ImVec2 cursorPs = ImGui::GetCursorPos();
		
		ImVec4 clearColor = ImVec4(this->m_ptr_GUIRepo->background_color.x / 255, this->m_ptr_GUIRepo->background_color.y / 255, this->m_ptr_GUIRepo->background_color.z / 255, 1.0f);
		ImGui::ColorEdit3("##Background-Color", (float*)&clearColor);

		this->m_ptr_GUIRepo->background_color.x = clearColor.x * 255;
		this->m_ptr_GUIRepo->background_color.y = clearColor.y * 255;
		this->m_ptr_GUIRepo->background_color.z = clearColor.z * 255;
		ImGui::Dummy(ImVec2(0, 12));
		ImGui::TreePop();
	}
}

void spe::UIInspector::gameEngineViewSetting()
{
	this->renderBackgroundBehindComponent();

	const std::string button_name = std::string(ICON_FA_COG) + "##" + "camera-comp";
	const ImVec2 temp_pos = ImGui::GetCursorPos();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY());
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - 40);

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 2));
	if (spe::Style::DisplaySmybolAsButton(button_name.c_str()))
	{
		ImGui::OpenPopup(button_name.c_str());
		this->m_pop_up_cursor_pos = ImVec2(spe::UIUtility::GUICursor.Position.x - 150, spe::UIUtility::GUICursor.Position.y + 20);
	}

	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize + 0.2f);

	ImGui::SetNextWindowPos(this->m_pop_up_cursor_pos);
	if (ImGui::BeginPopup(button_name.c_str()))
	{
		if (spe::Style::DisplaySymbolInMenuItemWithText(ICON_FA_RETWEET, "Reset", 30))
		{
			this->m_ptr_GUIRepo->Camera.reset();
		}
		
		ImGui::EndPopup();
	}

	ImGui::PopStyleVar();
	ImGui::SetCursorPos(temp_pos);

	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);

	this->m_cam_zoom = this->m_ptr_GUIRepo->Camera.getZoom();

	if (ImGui::TreeNode("Camera"))
	{
		float x = ImGui::GetCursorPosX();
		float y = ImGui::GetCursorPosY();

		//X
		ImGui::Dummy(ImVec2(0, 16));
		ImGui::Text("X");
		ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::SetCursorPos(ImVec2(x += 40, y += 16));
		ImGui::InputFloat("##X", &this->m_ptr_GUIRepo->Camera.Position.x, 0, 0, "%g");

		//Y
		ImGui::SetCursorPos(ImVec2(x += 125, y += 2));
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::SetCursorPos(ImVec2(x += 40, y -= 2));
		ImGui::InputFloat("##Y", &this->m_ptr_GUIRepo->Camera.Position.y, 0, 0, "%g");

		//Zoom
		ImGui::Dummy(ImVec2(0, 10));
		ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize + 0.1f);
		ImGui::Text("Zoom");
		ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2(x -= 120, y += 45));
		ImGui::SliderFloat("##Zoom", &this->m_cam_zoom, 0.1f, 4.0f, "%g");

		ImGui::Dummy(ImVec2(0, 15));
		ImGui::Text("Speed");
		ImGui::PushItemWidth(100);
		ImGui::SetCursorPos(ImVec2(x, ImGui::GetCursorPosY() - 25));
		ImGui::InputFloat("##speed-camera", &this->m_ptr_GUIRepo->Camera.camera_speed, 0, 0, "%g");

		this->m_ptr_GUIRepo->Camera.setZoom(this->m_cam_zoom);
		ImGui::TreePop();
	}
}

#pragma region Component displaayer

void spe::UIInspector::setCompontents()
{
	if (this->m_current_component_selected == "BoxCollider")
	{
		this->m_ptr_GUIRepo->sprite_in_inspector->collider.exist = true;
		this->m_current_component_selected = " ";
	}
	if (this->m_current_component_selected == "PhysicsBody")
	{
		this->m_ptr_GUIRepo->sprite_in_inspector->physicsBody.exist = true;
		this->m_current_component_selected = " ";
	}
	if (this->m_current_component_selected == "Animator")
	{
		this->m_ptr_GUIRepo->sprite_in_inspector->animator.exist = true;
		this->m_current_component_selected = " ";
	}
	if (this->m_current_component_selected == "Prefab")
	{
		this->m_ptr_GUIRepo->sprite_in_inspector->prefab.exist = true;
		this->m_current_component_selected = " ";
	}
	if (this->m_current_component_selected == "Light Source")
	{
		this->m_ptr_GUIRepo->sprite_in_inspector->light.enable();
		this->m_current_component_selected = " ";
	}
}

void spe::UIInspector::renameSprite()
{
	static float s_TimeToUpdate = 0.0f;

	s_TimeToUpdate += spe::Time::s_delta_time;

	if (s_TimeToUpdate > 0.25f)
	{
		s_TimeToUpdate = 0.0f;

		std::list<spe::Sprite*>& sprites = this->m_ptr_Repo->GetSprites();

		for (auto it = sprites.begin(); it != sprites.end(); ++it)
		{
			const spe::Sprite* sprite_1 = *it;

			for (auto it_2 = sprites.begin(); it_2 != sprites.end(); ++it_2)
			{
				 spe::Sprite* sprite_2 = *it_2;

				if (sprite_2->name == sprite_1->name && sprite_2->getId() != sprite_1->getId())
				{
					sprite_2->name += "(D)";
				}
			}
		}
	}
}

void spe::UIInspector::componentSelector()
{
	const ImVec2 temp = ImGui::GetCursorPos();
	ImGui::SetCursorPosY(38.5f);
	ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - SEARCH_BAR_MARGIN);
	ImGui::SetNextItemWidth(150);
	static char input_buffer[255];
	strcpy_s(input_buffer, this->m_ptr_GUIRepo->sprite_in_inspector->name.c_str());
	ImGui::InputText("##input-sprite-name", input_buffer, 255);

	if (input_buffer[0] != '\0')
	{
		this->m_ptr_GUIRepo->sprite_in_inspector->name = std::string(input_buffer);
	}

	const ImVec2 temp2 = ImGui::GetCursorPos();
	ImGui::SetCursorPos(ImVec2(10, 50));

	const std::string text = "ID:" + std::to_string(this->m_ptr_GUIRepo->sprite_in_inspector->getId());
	ImGui::Text(text.c_str());

	ImGui::SetCursorPos(temp2);

	this->m_tag_selector.Render();
	this->Hovered = this->m_tag_selector.Hovered;
	ImGui::SetCursorPos(temp);

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
	ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ADD_COMPONENTS_MARGIN);
	ImGui::SetNextItemWidth(250);
	if (ImGui::BeginCombo("##Components", "Add Components"))
	{
		for (int i = 0; i < this->m_components.size(); i++)
		{
			bool is_selected = (this->m_current_component_selected == this->m_components[i]);
			if (ImGui::Selectable(this->m_components[i], is_selected))
			{
				this->m_current_component_selected = this->m_components[i];
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void spe::UIInspector::setupComponents()
{
	ImGui::Dummy(ImVec2(0, 15));
	// Base Components
	this->transformComponent();
	DUMMY_COMPONENT;


	this->spriteRendererComponent();
	DUMMY_COMPONENT;
	
	//Collider
	if (this->m_ptr_GUIRepo->sprite_in_inspector->collider.exist)
	{
		this->boxColliderComponent();
		DUMMY_COMPONENT;
	}
	else
	{
		this->m_collider.leaveEditMode();
		this->m_ptr_collider_rectangle->Render = false;
	}

	//PhysicsBody
	if (this->m_ptr_GUIRepo->sprite_in_inspector->physicsBody.exist)
	{
		this->physicsBodyComponent();
		DUMMY_COMPONENT;
	}

	//Animator
	if (this->m_ptr_GUIRepo->sprite_in_inspector->animator.exist)
	{
		this->animatorComponent();
		DUMMY_COMPONENT;
	}

	// Prefab
	if (this->m_ptr_GUIRepo->sprite_in_inspector->prefab.exist)
	{
		this->prefabComponent();
		DUMMY_COMPONENT;
	}

	// Light Source
	if (this->m_ptr_GUIRepo->sprite_in_inspector->light.exist)
	{
		this->lightComponent();
		DUMMY_COMPONENT;
	}

	this->componentSelector();
	this->setCompontents();
}

void spe::UIInspector::transformComponent()
{
	auto inputXY = [](const char* label, float& inputX, float& inputY, float x, float y)
	{	
		static float s_inputWidth = 80;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
		ImGui::Text(label);
		ImGui::SetCursorPos(ImVec2(x + 90, y + 13));
		ImGui::Text("X");
		ImGui::PushItemWidth(s_inputWidth);
		ImGui::SetCursorPos(ImVec2(x + 120, y + 8));

		std::string x_inputId = std::string("##x") + std::string(label);

		ImGui::InputFloat(x_inputId.c_str(), &inputX, 0, 0, "%g");
		ImGui::PopItemWidth();

		x += 120;
		ImGui::SetCursorPos(ImVec2(x + 90, y + 13));
		ImGui::Text("Y");
		ImGui::PushItemWidth(s_inputWidth);
		ImGui::SetCursorPos(ImVec2(x + 120, y + 8));

		std::string y_inputId = std::string("##y") + std::string(label);

		ImGui::InputFloat(y_inputId.c_str(), &inputY, 0, 0, "%g");
		ImGui::PopItemWidth();
	};
	spe::Vector2 tempPos = this->m_ptr_GUIRepo->sprite_in_inspector->transform.GetPosition();
	this->renderBackgroundBehindComponent();
	this->renderComponentOptions(this->m_ptr_GUIRepo->sprite_in_inspector->transform, "Transform");
	if (ImGui::TreeNode("Transform"))
	{
		float x = ImGui::GetCursorPosX();
		float y = ImGui::GetCursorPosY();

		ImGui::Dummy(ImVec2(0, 8));
		inputXY("Position", tempPos.x, tempPos.y, x, y);

		y += 45;

		spe::Vector2 temp_scale = this->m_ptr_GUIRepo->sprite_in_inspector->transform.getScale();

		ImGui::Dummy(ImVec2(0, 10));
		inputXY("Scale", temp_scale.x, temp_scale.y, x, y);
		y += 45;

		int rotation = this->m_ptr_GUIRepo->sprite_in_inspector->transform.getRotation();
		ImGui::SetCursorPos(ImVec2(x, y + 10));
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
		ImGui::Text("Rotation");
		ImGui::SetCursorPos(ImVec2(x + 120, y + 10));
		ImGui::PushItemWidth(80);
		std::string x_inputId = std::string("##x") + std::string("Rotation");
		ImGui::InputInt(x_inputId.c_str(), &rotation, 0, 0);
		ImGui::PopItemWidth();
		ImGui::Dummy(ImVec2(0, 10));
		ImGui::SetCursorPos(ImVec2(x, y + 50));

		this->m_ptr_GUIRepo->sprite_in_inspector->transform.setScale(temp_scale);
		this->m_ptr_GUIRepo->sprite_in_inspector->transform.setRotation(rotation);
		this->m_ptr_GUIRepo->sprite_in_inspector->transform.SetPosition(tempPos);

		ImGui::TreePop();
	}
}

void spe::UIInspector::spriteRendererComponent()
{
	this->renderBackgroundBehindComponent();
	this->renderComponentOptions(this->m_ptr_GUIRepo->sprite_in_inspector->sprite_renderer, "Sprite Renderer");
	if (ImGui::TreeNode("Sprite Renderer"))
	{
		std::string input = spe::Utility::GetNamePathSplit(this->m_ptr_GUIRepo->sprite_in_inspector->sprite_renderer.path);
		this->m_sprite_name = this->m_ptr_GUIRepo->sprite_in_inspector->name;

		float y = ImGui::GetCursorPos().y;
		float x = ImGui::GetCursorPos().x;

		ImGui::SetCursorPos(ImVec2(x += 20.0f, y += 10));
		ImGui::Text("Sprite");

		ImGui::SetCursorPos(ImVec2(x += 100, y - 5));
		ImGui::SetNextItemWidth(this->m_sprite_input_width);
		ImGui::InputText("##spriteRenderer", &input[0], CHAR_MAX);

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped) && ImGui::IsMouseReleased(0) && this->m_ptr_GUIRepo->DragAndDropPath!= " ")
		{
			this->m_ptr_GUIRepo->sprite_in_inspector->setSpriteTexture(this->m_ptr_GUIRepo->DragAndDropPath);
		}

		ImGui::Dummy(ImVec2(0, 3.8f));

		//Sorting Layer
		ImGui::SetCursorPosX(x -= 100.0f);
		ImGui::Text("Sorting Layer");
		ImGui::SameLine();
		ImGui::PushItemWidth(42.0f);
		int32_t layerIndex = this->m_ptr_GUIRepo->sprite_in_inspector->sprite_renderer.sorting_layer_index;
		ImGui::InputInt("##Sorting Layer", &layerIndex, 0, 0);
		ImGui::PopItemWidth();

		if (layerIndex < 0)
		{
			layerIndex = 0;
		}

		if (layerIndex != this->m_ptr_GUIRepo->sprite_in_inspector->sprite_renderer.sorting_layer_index)
		{
			this->m_ptr_Repo->SetSpriteSortingLayer(layerIndex, this->m_ptr_GUIRepo->sprite_in_inspector);
		}

		ImGui::Dummy(ImVec2(0, 2));
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
		ImGui::Text("Effected by light");
		spe::UIUtility::SameLine(0);
		ImGui::Checkbox("##light-checkbox", &this->m_ptr_GUIRepo->sprite_in_inspector->sprite_renderer.effected_by_light);
		ImGui::Dummy(ImVec2(0, 7));
		ImGui::TreePop();
	}
}

void spe::UIInspector::boxColliderComponent()
{
	float x = ImGui::GetCursorPosX();
	float y = ImGui::GetCursorPosY();

	this->renderBackgroundBehindComponent();
	this->renderComponentOptions(this->m_ptr_GUIRepo->sprite_in_inspector->collider, "BoxCollider");
	if (ImGui::TreeNode("BoxCollider"))
	{
		this->m_ptr_collider_rectangle->Render = true;
		ImGui::Dummy(ImVec2(0, 4));
		float x = ImGui::GetCursorPos().x;
		float y = ImGui::GetCursorPos().y;

		this->m_collider.edit(x, y);
		this->m_collider.solid(x, y, this->m_ptr_GUIRepo->sprite_in_inspector);
		this->m_collider.width(x, y, this->m_ptr_GUIRepo->sprite_in_inspector);
		this->m_collider.height(this->m_ptr_GUIRepo->sprite_in_inspector);

		ImGui::TreePop();

		//Transparent since we open the boxcollider and we want to open the colider (rec)
		this->m_collider.drawBoxCollider(this->m_ptr_GUIRepo->sprite_in_inspector, this->m_ptr_collider_rectangle);
		ImGui::Dummy(ImVec2(0, 9));
	}
	else
	{
		this->m_collider.leaveEditMode();
		this->m_ptr_collider_rectangle->Render = false;
	}
}

void spe::UIInspector::physicsBodyComponent()
{
	float x = ImGui::GetCursorPosX();
	float y = ImGui::GetCursorPosY();

	this->renderBackgroundBehindComponent();
	this->renderComponentOptions(this->m_ptr_GUIRepo->sprite_in_inspector->physicsBody, "PhysicsBody");
	if (ImGui::TreeNode("PhysicsBody"))
	{
		ImGui::Dummy(ImVec2(0, 5));

		float x = ImGui::GetCursorPosX();
		float y = ImGui::GetCursorPosY();

		ImGui::Text("Gravity");
		ImGui::PushItemWidth(55);
		ImGui::SetCursorPos(ImVec2(x += 125, y -= 6.0f));
		ImGui::InputFloat("##gravity", &this->m_ptr_GUIRepo->sprite_in_inspector->physicsBody.gravity, 0, 0, "%g");
		ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
		ImGui::PopItemWidth();

		ImGui::Dummy(ImVec2(0, 5));

		ImGui::SetCursorPosX(x - 122);
		ImGui::Text("Mass");
		ImGui::PushItemWidth(55);
		ImGui::SetCursorPos(ImVec2(x, y += 45.0f));
		ImGui::InputFloat("##mass", &this->m_ptr_GUIRepo->sprite_in_inspector->physicsBody.mass, 0, 0, "%g");
		ImGui::PopItemWidth();

		ImGui::TreePop();
	}	
}

void spe::UIInspector::animatorComponent()
{
	float y = ImGui::GetCursorPos().y;
	float x = ImGui::GetCursorPos().x;

	this->renderBackgroundBehindComponent();
	this->renderComponentOptions(this->m_ptr_GUIRepo->sprite_in_inspector->animator, "Animator");
	if (ImGui::TreeNode("Animator"))
	{
		float y = ImGui::GetCursorPos().y;
		float x = ImGui::GetCursorPos().x;

		ImGui::SetCursorPos(ImVec2(x += 8.0f, y += 10));

		ImGui::SetCursorPosX(x += 10);
		ImGui::Text("Edit");
		ImGui::SetCursorPos(ImVec2(x += 73.5f, y -= 2.5f));
		if (spe::Style::DisplaySmybolAsButton(ICON_FA_EDIT))
		{
			this->m_ptr_GUIRepo->AnimationData.IsOpen = true;

			// Gets handleed by UIAnimation.cpp
			this->m_ptr_GUIRepo->AnimationData.Reload = true;
		}

		ImGui::TreePop();
	}
}

void spe::UIInspector::prefabComponent()
{
	float x = ImGui::GetCursorPosX();
	float y = ImGui::GetCursorPosY();

	this->renderBackgroundBehindComponent();
	if (ImGui::TreeNode("Prefab"))
	{
		ImGui::SetCursorPos(ImVec2(x += 45, y += 40));
		const std::string fileLocation = "File location: " + this->m_ptr_GUIRepo->sprite_in_inspector->prefab.user_path_to_file;
		ImGui::Text(fileLocation.c_str());

		ImGui::SetCursorPos(ImVec2(x, y + 40));
		ImGui::Text("Update file");
		ImGui::SetCursorPos(ImVec2(x + 120, y + 35));
		if (spe::Style::DisplaySmybolAsButton(ICON_FA_RETWEET))
		{
			// TODO: Delete prefab component when the file is moved (user dumb haha)
			this->m_ptr_GUIRepo->sprite_in_inspector->prefab.UpdatePath();
			const spe::Prefab& prefab = this->m_ptr_GUIRepo->sprite_in_inspector->prefab;
			spe::Savesystem::CreateOrUpdatePrefabFile(this->m_ptr_GUIRepo->sprite_in_inspector,
				prefab.user_path_to_file, prefab.path_to_old_file);

		}
		ImGui::SetCursorPos(ImVec2(x, y + 80));
		ImGui::Text("Load in memory ");
		ImGui::SetCursorPos(ImVec2(x + 170, y + 75));
		ImGui::Checkbox("##LoadInMemory", &this->m_ptr_GUIRepo->sprite_in_inspector->prefab.load_in_memory);
		ImGui::TreePop();
	}	
}

void spe::UIInspector::lightComponent()
{
	this->m_light_radius = this->m_ptr_GUIRepo->sprite_in_inspector->light.getRadius();
	this->m_light_intensity = this->m_ptr_GUIRepo->sprite_in_inspector->light.getIntensity();


	this->renderBackgroundBehindComponent();
	this->renderComponentOptions(this->m_ptr_GUIRepo->sprite_in_inspector->light, "Light Source");
	if (ImGui::TreeNode("Light Source"))
	{
		float y = ImGui::GetCursorPos().y;
		float x = ImGui::GetCursorPos().x;

		ImGui::SetCursorPos(ImVec2(x += 8.0f, y += 10));

		ImGui::SetCursorPosX(x += 5);
		ImGui::Text("Radius: ");
		spe::UIUtility::SameLine(0);
		ImGui::SetNextItemWidth(50);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 47);
		ImGui::InputFloat("##light-radius", &this->m_light_radius, 0, 0, "%g");

		this->m_ptr_GUIRepo->sprite_in_inspector->light.setRadius(this->m_light_radius);

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 14.5f);
		ImGui::Text("Intensity: ");
		spe::UIUtility::SameLine(0);
		ImGui::SetNextItemWidth(50);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 38);
		ImGui::InputFloat("##light-intensity", &this->m_light_intensity, 0, 0, "%g");

		this->m_ptr_GUIRepo->sprite_in_inspector->light.setIntensity(this->m_light_intensity);

		
		ImGui::Text("Color: ");
		const sf::Vector3f& clr = this->m_ptr_GUIRepo->sprite_in_inspector->light.getColor();
		ImVec4 clear_color = ImVec4(clr.x, clr.y, clr.z, 1.0f);
		ImGui::ColorEdit3("##light-Color", (float*)&clear_color);

		this->m_ptr_GUIRepo->sprite_in_inspector->light.setColor(sf::Vector3f(clear_color.x, clear_color.y, clear_color.z));

		ImGui::TreePop();
	}
}

#pragma endregion

//Public functions

void spe::UIInspector::Render()
{
	if (spe::UIUtility::HandleCloseAndReloadWindow(this->m_ptr_GUIRepo->InspectorData, this->Hovered, INSPECTOR_DEFAULT_WINDOW_SIZE))
	{
		return;
	}

	this->Hovered = false;
	ImGui::Begin("##sprite-inspector", NULL, DEFAULT_FLAGS);

	// Close rectangle
	this->m_ptr_GUIRepo->InspectorData.IsOpen = spe::UIUtility::RenderCloseRectangle(this->m_Size.x - 300, ICON_FA_EDIT, "##inspector", "Inspector", 0);

	// Left arrow
	this->renderOptions();

	this->renameSprite();

	//Setting it here transparent because if we go down and out box collider is actually getting used it will update to green automatic
	if (this->m_ptr_GUIRepo->sprite_in_inspector != nullptr)
	{
		// Handle a sprite

		this->drawRectangleOverCurrentObject();
		this->setupComponents();
	}
	else
	{
		// Handle it if no sprite is selected
		this->displayDefaultInspectorView();
		this->m_collider.leaveEditMode();
		this->m_ptr_sprite_over_rectangle->Render = false;
		this->m_ptr_collider_rectangle->Render = false;
	}

	const ImVec2 window_pos = ImVec2(INSPECTOR_WINDOW_POS.x + INSPECTOR_DEFAULT_WINDOW_SIZE.x - this->m_Size.x, INSPECTOR_WINDOW_POS.y);

	if (!this->Hovered)
	{
		this->Hovered = spe::UIUtility::IsHovered(window_pos, INSPECTOR_DEFAULT_WINDOW_SIZE);
	}

	ImGui::SetWindowPos(ImVec2(INSPECTOR_WINDOW_POS.x + INSPECTOR_DEFAULT_WINDOW_SIZE.x - this->m_Size.x, INSPECTOR_WINDOW_POS.y));
	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
	ImGui::SetWindowSize(this->m_Size);
	ImGui::End();
}
