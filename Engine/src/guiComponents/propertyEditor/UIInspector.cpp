#include "UIInspector.h"

s2d::UIInspector::UIInspector()
{
	this->preInit();
}

void s2d::UIInspector::preInit()
{
	this->m_sprite_input_width = 200.0f;
	this->m_window_size_width = 390.0f;

	this->m_menu_name = "menu";

	this->m_window_size = INSPECTOR_DEFAULT_WINDOW_SIZE;
	this->state = s2d::InspectorState::None;

	this->m_components.push_back("BoxCollider");
	this->m_components.push_back("PhysicsBody");
	this->m_components.push_back("Animator");
	this->m_components.push_back("Prefab");
}

void s2d::UIInspector::afterInit()
{
	this->m_ptr_gui_repo->add(sf::Vector2f(0, 0), sf::Vector2f(0, 0), sf::Color(124, 252, 0), 3.5f,
		PATH_TO_TRANSPARENT_PIC, "collider-over-sprite");

	this->m_ptr_gui_repo->add(sf::Vector2f(0, 0), sf::Vector2f(0, 0), sf::Color(0, 0, 0), 3.5f,
		PATH_TO_TRANSPARENT_PIC, "texture-over-sprite");

	this->m_ptr_collider_rectangle = this->m_ptr_gui_repo->getByName("collider-over-sprite");
	this->m_ptr_sprite_over_rectangle = this->m_ptr_gui_repo->getByName("texture-over-sprite");

	this->m_collider.initScaleDottsUI(*this->m_ptr_gui_repo);
}

//Private functions

void s2d::UIInspector::render()
{
	if (s2d::UI::handleCloseAndReloadWindow(
		s2d::UIInfo::s_is_inspector_open.is_open, s2d::UIInfo::s_is_inspector_open.reload,
		this->is_hovered,
		this->m_window_size, INSPECTOR_DEFAULT_WINDOW_SIZE))
	{
		return;
	}

	ImGui::Begin("##sprite-inspector", NULL, DEFAULT_WINDOW_FLAGS);

	// Close rectangle
	s2d::UI::renderCloseRectangle(this->m_window_size.x - 300, ICON_FA_EDIT, "##inspector", "Inspector", 0);

	// Left arrow
	this->renderOptions();

	//Setting it here transparent because if we go down and out box collider is actually getting used it will update to green automatic
	if (this->m_ptr_sprite_repo->sprite_in_inspector != nullptr)
	{
		// Handle a sprite

		this->drawRectangleOverCurrentObject();
		this->setupComponents();
	}
	else
	{
		// Handle it if no sprite is selected
	}

	const ImVec2 window_pos = ImVec2(INSPECTOR_WINDOW_POS.x + INSPECTOR_DEFAULT_WINDOW_SIZE.x - this->m_window_size.x, INSPECTOR_WINDOW_POS.y);
	
	this->is_hovered = s2d::UI::isHovered(window_pos, INSPECTOR_DEFAULT_WINDOW_SIZE);

	ImGui::SetWindowPos(ImVec2(INSPECTOR_WINDOW_POS.x + INSPECTOR_DEFAULT_WINDOW_SIZE.x - this->m_window_size.x, INSPECTOR_WINDOW_POS.y));
	ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size);
	ImGui::SetWindowSize(this->m_window_size);
	ImGui::End();
}

void s2d::UIInspector::renderComponentOptions(s2d::Component& component, const std::string& name)
{
	const std::string button_name = std::string(ICON_FA_COG) + "##" + name;
	const ImVec2 temp_pos = ImGui::GetCursorPos();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY());
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - 40);

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 2));
	if (s2d::FontManager::displaySmybolAsButton(button_name.c_str()))
	{
		ImGui::OpenPopup(button_name.c_str());
		this->m_pop_up_cursor_pos = ImVec2(s2d::UI::s_gui_cursor.position.x - 150, s2d::UI::s_gui_cursor.position.y + 20);
	}
	ImGui::SetNextWindowPos(this->m_pop_up_cursor_pos);
	if (ImGui::BeginPopup(button_name.c_str()))
	{
		if (s2d::FontManager::displaySymbolInMenuItemWithText(ICON_FA_RETWEET, "Reset"))
		{
			component.reset();
			component.exist = true;
		}
		if (!component.base_component)
		{
			if (s2d::FontManager::displaySymbolInMenuItemWithText(ICON_FA_TRASH, "Delete"))
			{
				std::cout << "Deleting";
				component.reset();
				component.exist = false;
			}
		}
		ImGui::EndPopup();
	}
	
	ImGui::PopStyleVar();
	ImGui::SetCursorPos(temp_pos);
}

void s2d::UIInspector::drawRectangleOverCurrentObject()
{
	this->m_ptr_sprite_over_rectangle->render = true;

	sf::RectangleShape* ptr_shape = &this->m_ptr_sprite_over_rectangle->shape;
	
	ptr_shape->setSize(sf::Vector2f(this->m_ptr_sprite_repo->sprite_in_inspector->transform.textureSize.x, this->m_ptr_sprite_repo->sprite_in_inspector->transform.textureSize.y));
	ptr_shape->setPosition(this->m_ptr_sprite_repo->sprite_in_inspector->getOrigininalPosition().x, this->m_ptr_sprite_repo->sprite_in_inspector->getOrigininalPosition().y);
}

void s2d::UIInspector::renderOptions()
{
	const ImVec2 tempCursor = ImGui::GetCursorPos();

	this->resizeWindow();

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
	ImGui::SetCursorPosX(0);

	s2d::UI::drawRectangleInGUIWIndow(
		ImVec2(this->m_window_size.x + 50, 45), ImVec2(1900 - this->m_window_size.x, 130), ImColor(26, 26, 26, 255));
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 50);
}

void s2d::UIInspector::resizeWindow()
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
	s2d::FontManager::displaySmybolAsButton(ICON_FA_ARROW_LEFT);
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
	{
		this->m_resize_window_data.clicked_on_resize_button = true;
	}
	if (this->m_resize_window_data.clicked_on_resize_button && ImGui::IsMouseDown(0))
	{
		float movedy = 0;
		if (s2d::UI::s_gui_cursor.position_changed)
		{
			s2d::Vector2 moved = s2d::UI::s_gui_cursor.lastPos - s2d::UI::s_gui_cursor.position;
			movedy = moved.x;
		}
		if (this->m_window_size.x + movedy > 350
			&& this->m_window_size.x + movedy + this->m_ptr_gui_repo->ptr_hierarchy_window_size->x < 1920)
		{
			this->m_window_size.x += movedy;
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

void s2d::UIInspector::checkDupeName()
{
	for (int i = 0; i < this->m_ptr_sprite_repo->amount(); i++)
	{
		s2d::Sprite* const sprite = this->m_ptr_sprite_repo->readAt(i);
		if (this->m_ptr_sprite_repo->sprite_in_inspector->name == sprite->name 
			&& this->m_ptr_sprite_repo->sprite_in_inspector->getId() != sprite->getId())
		{
			this->m_ptr_sprite_repo->sprite_in_inspector->name = "<no dupe (" + std::to_string(this->m_ptr_sprite_repo->dupeNameCounter) += ")>";
			this->m_ptr_sprite_repo->dupeNameCounter++;
		}
	}
}

void s2d::UIInspector::renderBackgroundBehindComponent()
{
	const ImVec2 temp = ImGui::GetCursorPos();
	ImGui::SetCursorPosX(1920 - this->m_window_size.x);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 76);
	s2d::UI::drawRectangleInGUIWIndow(ImVec2(this->m_window_size.x, 27), ImGui::GetCursorPos(), COMPONENT_SELECTED_COLOR);
	ImGui::SetCursorPos(temp);
}

#pragma region  defaultInspectorView

void s2d::UIInspector::displayDefaultInspectorView()
{
	ImGui::Dummy(ImVec2(0, 4));
	ImGui::Text(" Game Window settings");
	ImGui::Dummy(ImVec2(0, 11));
	ImGui::Separator();

	//Colors
	this->backgroundSetting();
	this->gameEngineViewSetting();
}

void s2d::UIInspector::backgroundSetting()
{
	ImGui::Dummy(ImVec2(0, 10));
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 300, ImGui::GetCursorPosY() - 10));	

	if (s2d::FontManager::displaySmybolAsButton(ICON_FA_RETWEET "##B"))
	{
		this->background_color = DEFAULT_BACKGROUND_COLOR;
	}
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 41 + 10);
    if (ImGui::TreeNode("Background"))
	{
		ImGui::Dummy(ImVec2(0, 16));

		ImVec2 cursorPs = ImGui::GetCursorPos();

	    ImVec4 clearColor = ImVec4(this->background_color.x / 255, this->background_color.y / 255, this->background_color.z / 255, 1.0f);
		ImGui::ColorEdit3("Background-Color", (float*)&clearColor);

		this->background_color.x = clearColor.x * 255;
		this->background_color.y = clearColor.y * 255;
		this->background_color.z = clearColor.z * 255;
		ImGui::Dummy(ImVec2(0, 12));
		ImGui::TreePop();
	}
}

void s2d::UIInspector::gameEngineViewSetting()
{
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 300, ImGui::GetCursorPosY() - 10));

	//if (s2d::FontManager::displaySmybolAsButton(ICON_FA_RETWEET "##C"))
	//{
	//	s2d::GameObject::ptr_camera_tRealTimeEditor->reset();
	//}
	//ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 41 + 10);
	//if (ImGui::TreeNode("Camera"))
	//{
	//	float x = ImGui::GetCursorPosX();
	//	float y = ImGui::GetCursorPosY();

	//	//X
	//	ImGui::Dummy(ImVec2(0, 16));
	//	ImGui::Text("X");
	//	ImGui::SameLine();
	//	ImGui::PushItemWidth(100);
	//	ImGui::SetCursorPos(ImVec2(x += 40, y += 16));
	//	ImGui::InputFloat("##X", &s2d::GameObject::ptr_camera_tRealTimeEditor->transform.position.x, 0, 0, "%g");

	//	//Y
	//	ImGui::SetCursorPos(ImVec2(x += 125, y += 2));
	//	ImGui::Text("Y");
	//	ImGui::SameLine();
	//	ImGui::PushItemWidth(100);
	//	ImGui::SetCursorPos(ImVec2(x += 40, y -= 2));
	//	ImGui::InputFloat("##Y", &s2d::GameObject::ptr_camera_tRealTimeEditor->transform.position.y, 0, 0, "%g");

	//	//Zoom
	//	ImGui::Dummy(ImVec2(0, 10));
	//	ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size + 0.1f);
	//	ImGui::Text("Zoom");
	//	ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size);
	//	ImGui::SameLine();
	//	ImGui::SetCursorPos(ImVec2(x -= 120, y += 54));
	//	ImGui::SliderFloat("##Zoom", &s2d::GameObject::ptr_camera_tRealTimeEditor->cameraZoom, 0.1f, 4.0f, "%g");

	//	if (s2d::GameObject::ptr_camera_tRealTimeEditor->cameraZoom <= 0)
	//	{
	//		s2d::GameObject::ptr_camera_tRealTimeEditor->cameraZoom = 0.8f;
	//	}

	//	ImGui::TreePop();
	//}
}

#pragma endregion

#pragma region Component displaayer

void s2d::UIInspector::setCompontents()
{
	if (this->m_current_component_selected == "BoxCollider")
	{
		this->m_ptr_sprite_repo->sprite_in_inspector->collider.exist = true;
		this->m_current_component_selected = " ";
	}
	if (this->m_current_component_selected == "PhysicsBody")
	{
		this->m_ptr_sprite_repo->sprite_in_inspector->physicsBody.exist = true;
		this->m_current_component_selected = " ";
	}
	if (this->m_current_component_selected == "Animator")
	{
		this->m_ptr_sprite_repo->sprite_in_inspector->animator.exist = true;
		this->m_current_component_selected = " ";
	}
	if (this->m_current_component_selected == "Prefab")
	{
		this->m_ptr_sprite_repo->sprite_in_inspector->prefab.exists = true;
		this->m_current_component_selected = " ";
	}
}

void s2d::UIInspector::componentSelector()
{
	const ImVec2 temp = ImGui::GetCursorPos();
	ImGui::SetCursorPosY(38.5f);
	ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - SEARCH_BAR_MARGIN);
	ImGui::SetNextItemWidth(150);
	static char input_buffer[255];
	strcpy_s(input_buffer, this->m_ptr_sprite_repo->sprite_in_inspector->name.c_str());
	ImGui::InputText("##input-sprite-name", input_buffer, 255);

	if (input_buffer[0] != '\0')
	{
		this->m_ptr_sprite_repo->sprite_in_inspector->name = std::string(input_buffer);
	}
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

void s2d::UIInspector::setupComponents()
{
	ImGui::Dummy(ImVec2(0, 15));
	if (this->m_search_component_filter.PassFilter("Transform"))
	{
		this->transformComponent();
		DUMMY_COMPONENT;
	}
	if (this->m_search_component_filter.PassFilter("Sprite Renderer"))
	{
		this->spriteRendererComponent();
		DUMMY_COMPONENT;
	}

	//Collider
	if (this->m_ptr_sprite_repo->sprite_in_inspector->collider.exist)
	{
		this->boxColliderComponent();
		DUMMY_COMPONENT;
	}

	//PhysicsBody
	if (this->m_ptr_sprite_repo->sprite_in_inspector->physicsBody.exist
		&& this->m_search_component_filter.PassFilter(this->m_components[1]))
	{
		this->physicsBodyComponent();
		DUMMY_COMPONENT;
	}

	//Animator
	if (this->m_ptr_sprite_repo->sprite_in_inspector->animator.exist
		&& this->m_search_component_filter.PassFilter(this->m_components[2]))
	{
		this->animatorComponent();
		DUMMY_COMPONENT;
	}

	// Prefab
	if (this->m_ptr_sprite_repo->sprite_in_inspector->prefab.exists
		&& this->m_search_component_filter.PassFilter(this->m_components[3]))
	{
		this->prefabComponent();
	}

	this->componentSelector();
	this->setCompontents();
}

void s2d::UIInspector::transformComponent()
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
	this->renderBackgroundBehindComponent();
	this->renderComponentOptions(this->m_ptr_sprite_repo->sprite_in_inspector->transform, "Transform");
	if (ImGui::TreeNode("Transform"))
	{
		float x = ImGui::GetCursorPosX();
		float y = ImGui::GetCursorPosY();

		ImGui::Dummy(ImVec2(0, 8));
		inputXY("Position", this->m_ptr_sprite_repo->sprite_in_inspector->transform.position.x,
			this->m_ptr_sprite_repo->sprite_in_inspector->transform.position.y, x, y);

		y += 45;

		s2d::Vector2 temp_scale = this->m_ptr_sprite_repo->sprite_in_inspector->transform.getScale();

		ImGui::Dummy(ImVec2(0, 10));
		inputXY("Scale", temp_scale.x, temp_scale.y, x, y);

		this->m_ptr_sprite_repo->sprite_in_inspector->transform.setScale(temp_scale);

		ImGui::SetCursorPos(ImVec2(x, y + 50));
		ImGui::TreePop();
	}
}

void s2d::UIInspector::spriteRendererComponent()
{
	this->renderBackgroundBehindComponent();
	this->renderComponentOptions(this->m_ptr_sprite_repo->sprite_in_inspector->sprite_renderer, "Sprite Renderer");
	if (ImGui::TreeNode("Sprite Renderer"))
	{
		std::string input = s2d::UIInspector::getNamePathSplit(this->m_ptr_sprite_repo->sprite_in_inspector->sprite_renderer.path);
		this->m_sprite_name = this->m_ptr_sprite_repo->sprite_in_inspector->name;

		float y = ImGui::GetCursorPos().y;
		float x = ImGui::GetCursorPos().x;

		ImGui::SetCursorPos(ImVec2(x += 20.0f, y += 10));
		ImGui::Text("Sprite");

		ImGui::SetCursorPos(ImVec2(x += 100, y - 5));
		ImGui::SetNextItemWidth(this->m_sprite_input_width);
		ImGui::InputText("##spriteRenderer", &input[0], CHAR_MAX);

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped) && ImGui::IsMouseReleased(0) && this->m_ptr_sprite_repo->asset_folder_data.drag_and_drop_path != " ")
		{
			this->m_ptr_sprite_repo->sprite_in_inspector->setSpriteTexture(this->m_ptr_sprite_repo->asset_folder_data.drag_and_drop_path);
		}

		ImGui::Dummy(ImVec2(0, 3.8f));

		//Sorting Layer
		ImGui::SetCursorPosX(x -= 100.0f);
		ImGui::Text("Sorting Layer");
		ImGui::SameLine();
		ImGui::PushItemWidth(42.0f);
		int32_t layerIndex = this->m_ptr_sprite_repo->sprite_in_inspector->sprite_renderer.sorting_layer_index;
		ImGui::InputInt("##Sorting Layer", &layerIndex, 0, 0);
		ImGui::PopItemWidth();

		if (layerIndex < 0)
		{
			layerIndex = 0;
		}
		this->m_ptr_sprite_repo->sprite_in_inspector->sprite_renderer.sorting_layer_index = layerIndex;

		ImGui::Dummy(ImVec2(0, 7));
		ImGui::TreePop();
	}
}

void s2d::UIInspector::boxColliderComponent()
{
	float x = ImGui::GetCursorPosX();
	float y = ImGui::GetCursorPosY();

	this->renderBackgroundBehindComponent();
	this->renderComponentOptions(this->m_ptr_sprite_repo->sprite_in_inspector->collider, "BoxCollider");
	if (ImGui::TreeNode("BoxCollider"))
	{
		this->m_ptr_collider_rectangle->render = true;
		ImGui::Dummy(ImVec2(0, 4));
		float x = ImGui::GetCursorPos().x;
		float y = ImGui::GetCursorPos().y;

		this->m_collider.edit(x, y);
		this->m_collider.solid(x, y, this->m_ptr_sprite_repo->sprite_in_inspector);
		this->m_collider.width(x, y, this->m_ptr_sprite_repo->sprite_in_inspector);
		this->m_collider.height(this->m_ptr_sprite_repo->sprite_in_inspector);

		ImGui::TreePop();

		//Transparent since we open the boxcollider and we want to open the colider (rec)
		this->m_collider.drawBoxCollider(this->m_ptr_sprite_repo->sprite_in_inspector, this->m_ptr_collider_rectangle);

		ImGui::Dummy(ImVec2(0, 9));
	}
	else
	{
		this->m_ptr_collider_rectangle->render = false;
	}
}

void s2d::UIInspector::physicsBodyComponent()
{
	float x = ImGui::GetCursorPosX();
	float y = ImGui::GetCursorPosY();

	this->renderBackgroundBehindComponent();
	this->renderComponentOptions(this->m_ptr_sprite_repo->sprite_in_inspector->physicsBody, "PhysicsBody");
	if (ImGui::TreeNode("PhysicsBody"))
	{
		ImGui::Dummy(ImVec2(0, 5));

		float x = ImGui::GetCursorPosX();
		float y = ImGui::GetCursorPosY();

		ImGui::Text("Gravity");
		ImGui::PushItemWidth(55);
		ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size - 0.2f);
		ImGui::SetCursorPos(ImVec2(x += 125, y -= 6.0f));
		ImGui::InputFloat("##gravity", &this->m_ptr_sprite_repo->sprite_in_inspector->physicsBody.gravity, 0, 0, "%g");
		ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size);
		ImGui::PopItemWidth();

		ImGui::Dummy(ImVec2(0, 5));

		ImGui::SetCursorPosX(x - 122);
		ImGui::Text("Mass");
		ImGui::PushItemWidth(55);
		ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size - 0.2f);
		ImGui::SetCursorPos(ImVec2(x, y += 45.0f));
		ImGui::InputFloat("##mass", &this->m_ptr_sprite_repo->sprite_in_inspector->physicsBody.mass, 0, 0, "%g");
		ImGui::PopItemWidth();
		ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size);

		ImGui::TreePop();
	}	
}

void s2d::UIInspector::animatorComponent()
{
	float y = ImGui::GetCursorPos().y;
	float x = ImGui::GetCursorPos().x;

	this->renderBackgroundBehindComponent();
	this->renderComponentOptions(this->m_ptr_sprite_repo->sprite_in_inspector->animator, "Animator");
	if (ImGui::TreeNode("Animator"))
	{
		float y = ImGui::GetCursorPos().y;
		float x = ImGui::GetCursorPos().x;

		ImGui::SetCursorPos(ImVec2(x += 8.0f, y += 10));

		ImGui::SetCursorPosX(x += 10);
		ImGui::Text("Edit");
		ImGui::SetCursorPos(ImVec2(x += 73.5f, y -= 2.5f));
		if (s2d::FontManager::displaySmybolAsButton(ICON_FA_EDIT))
		{
			s2d::UIInfo::s_is_animation_open.is_open = true;
		}

		ImGui::TreePop();
	}
}

void s2d::UIInspector::prefabComponent()
{
	float x = ImGui::GetCursorPosX();
	float y = ImGui::GetCursorPosY();

	this->renderBackgroundBehindComponent();
	if (ImGui::TreeNode("Prefab"))
	{
		ImGui::SetCursorPos(ImVec2(x += 45, y += 40));
		const std::string fileLocation = "File location: " + this->m_ptr_sprite_repo->sprite_in_inspector->prefab.userPathToFile;
		ImGui::Text(fileLocation.c_str());

		ImGui::SetCursorPos(ImVec2(x, y + 40));
		ImGui::Text("Update file");
		ImGui::SetCursorPos(ImVec2(x + 120, y + 35));
		if (s2d::FontManager::displaySmybolAsButton(ICON_FA_RETWEET))
		{
			this->m_ptr_sprite_repo->sprite_in_inspector->prefab.updateFile();
		}
		ImGui::SetCursorPos(ImVec2(x, y + 80));
		ImGui::Text("Load in memory ");
		ImGui::SetCursorPos(ImVec2(x + 170, y + 75));
		ImGui::Checkbox("##LoadInMemory", &this->m_ptr_sprite_repo->sprite_in_inspector->prefab.loadInMemory);
		ImGui::TreePop();
	}	
}

#pragma endregion

//static functions

std::string s2d::UIInspector::getNamePathSplit(std::string path)
{
	std::vector<std::string> pick;

	std::vector<std::string> splittetSring = std::splitString(path, "\\");

	return splittetSring[splittetSring.size() -1];
}

//Public functions

void s2d::UIInspector::createUIInspector()
{
	this->render();
}

void s2d::UIInspector::setGUIRepo(s2d::GUIRepository* repo)
{
	this->m_ptr_gui_repo = repo;
	this->m_ptr_gui_repo->ptr_inspector_window_size = &this->m_window_size;
	this->preInit();
	this->afterInit();
}


