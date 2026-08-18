#include "UIInspector.h"
#include "UIInspector.h"
#include "Core/EngineData.h"

#include <random>

#include "GUI/UIWindow.h"

// Constructor

spe::UIInspector::UIInspector()
	: m_ptr_ColliderRec(nullptr),
	m_ptr_SpriteRec(nullptr),
	m_SpriteInputWidth(0.0f),
	m_DupeNameEditorTimer(0.0f),
	m_WindowSizeWidth(0.0f),
	m_ComponentSelected(),
	m_InputName(nullptr),
	m_LightRadius(0.0f),
	m_LightIntensity(0.0f),
	m_CamZoom(0.0f),
	State(spe::InspectorState::None) { }

void spe::UIInspector::Init()
{
	this->m_SpriteInputWidth = 200.0f;
	this->m_WindowSizeWidth = 390.0f;

	this->m_MenuName = "menu";

	this->m_Size = INSPECTOR_DEFAULT_WINDOW_SIZE;
	this->State = spe::InspectorState::None;

	spe::Rectangle* rec = new spe::Rectangle(sf::Vector2f(0, 0), sf::Vector2f(0, 0), sf::Color(124, 252, 0), 3.5f,
		PATH_TO_TRANSPARENT_PIC, "collider-over-sprite");

	this->m_ptr_GUIRepo->Add(rec);

	spe::Rectangle* over_sprite = new spe::Rectangle(sf::Vector2f(0, 0), sf::Vector2f(0, 0), sf::Color(0, 0, 0), 3.5f,
		PATH_TO_TRANSPARENT_PIC, "texture-over-sprite");

	this->m_ptr_GUIRepo->Add(over_sprite);

	this->m_ptr_ColliderRec = this->m_ptr_GUIRepo->GetByName("collider-over-sprite");
	this->m_ptr_SpriteRec = this->m_ptr_GUIRepo->GetByName("texture-over-sprite");

	this->m_ptr_ColliderRec->SortingLayerIdx = 1;
	this->m_ptr_GUIRepo->UpdateLayerIndex();

	this->m_Collider.InitScaleDottsUI(*this->m_ptr_GUIRepo);

	this->m_ComponentsName.push_back("BoxCollider");
	this->m_ComponentsName.push_back("PhysicsBody");
	this->m_ComponentsName.push_back("Animator");
	this->m_ComponentsName.push_back("Light Source");
	this->m_ComponentsName.push_back("Audio");
	this->m_ComponentsName.push_back("Particles");

	this->m_ptr_GUIRepo->InspectorData.ptr_Size = &this->m_Size;

	this->m_TagSelector.SetRepos(this->m_ptr_Repo, this->m_ptr_GUIRepo);
}

//Private functions

void spe::UIInspector::RenderComponentOptions(spe::Component& component, const std::string& name)
{
	const std::string button_name = std::string(ICON_FA_COG) + "##" + name;
	const ImVec2 temp_pos = ImGui::GetCursorPos();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY());
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - 40);

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 2));
	if (spe::Style::DisplaySmybolAsButton(button_name.c_str()))
	{
		ImGui::OpenPopup(button_name.c_str());
		this->m_PopUpCursorPos = ImVec2(spe::UIUtility::GUICursor.Position.X - 150, spe::UIUtility::GUICursor.Position.Y + 20);
	}

	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize + 0.2f);

	ImGui::SetNextWindowPos(this->m_PopUpCursorPos);
	if (ImGui::BeginPopup(button_name.c_str()))
	{
		if (spe::Style::DisplaySymbolInMenuItemWithText(ICON_FA_RETWEET, "Reset", 30))
		{
			component.Reset();
			component.Exist = true;
		}
		if (!component.BaseComponent)
		{
			if (spe::Style::DisplaySymbolInMenuItemWithText(ICON_FA_TRASH, "Delete", 30))
			{
				spe::Light* childPtr = dynamic_cast<Light*>(&component);
				if (childPtr) {
					childPtr->DeleteLight();
				}

				component.Reset();
				component.Exist = false;
			}
		}
		ImGui::EndPopup();
	}
	
	ImGui::PopStyleVar();
	ImGui::SetCursorPos(temp_pos);

	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
}

void spe::UIInspector::GeneralSettings()
{
	this->RenderBackgroundBehindComponent();
	if (ImGui::TreeNode("General"))
	{
		ImGui::Dummy(ImVec2(0, 16));

		if (ImGui::Button("Reaload Textures"))
		{
			this->m_ptr_Repo->ReloadTextures();
		}
		ImGui::Checkbox("Ignore lights", &this->m_ptr_GUIRepo->RenderAlwaysWithoutLight);

		ImGui::Checkbox("Spawn in camera center", &this->m_ptr_GUIRepo->SpawnInCenter);

		ImGui::Checkbox("Enable junp through boxes when under", &this->m_ptr_GUIRepo->JumpThroughBoxes);

		ImGui::TreePop();
	}
}

void spe::UIInspector::DrawRectangleOverCurrentObject()
{
	this->m_ptr_SpriteRec->Render = true;

	sf::RectangleShape* ptr_shape = &this->m_ptr_SpriteRec->Shape;
	
	spe::Vector2 screenPos = this->m_ptr_GUIRepo->InspectorSprite->Transform.GetScreenPosition();
	spe::Vector2 textureSize = this->m_ptr_GUIRepo->InspectorSprite->Transform.TextureSize;

	ptr_shape->setSize(sf::Vector2f(textureSize.X, textureSize.Y));
	ptr_shape->setPosition(screenPos.X - textureSize.X / 2.0f, screenPos.Y - textureSize.Y / 2.0f);
}

void spe::UIInspector::RenderOptions()
{
	const ImVec2 tempCursor = ImGui::GetCursorPos();

	this->ResizeWindow();

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
	ImGui::SetCursorPosX(0);

	float windowWidth = (float)spe::EngineData::s_WindowWidth;

	spe::UIUtility::DrawRectangleInGUIWIndow(
		ImVec2(this->m_Size.x + 50, 45), ImVec2(windowWidth - 20 - this->m_Size.x, 130 - ImGui::GetScrollY()), ImColor(26, 26, 26, 255));
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 50);
}

void spe::UIInspector::ResizeWindow()
{
	bool pop_style = false;
	ImGui::SetCursorPosX(10);
	ImGui::SetCursorPosY(4);
	if (this->m_WindowData.ClickedOnResizeButton)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 1));
		pop_style = true;
	}
	spe::Style::DisplaySmybolAsButton(ICON_FA_ARROW_LEFT);
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
	{
		this->m_WindowData.ClickedOnResizeButton = true;
	}
	if (this->m_WindowData.ClickedOnResizeButton && ImGui::IsMouseDown(0))
	{
		float movedy = 0;
		if (spe::UIUtility::GUICursor.PositionChanged)
		{
			spe::Vector2 moved = spe::UIUtility::GUICursor.LastPosition - spe::UIUtility::GUICursor.Position;
			movedy = moved.X;
		}
		float windowWidth = (float)spe::EngineData::s_WindowWidth;
		if (this->m_Size.x + movedy > 350
			&& this->m_Size.x + movedy + this->m_ptr_GUIRepo->HierarchyData.ptr_Size->x < windowWidth)
		{
			this->m_Size.x += movedy;
		}
	}
	else
	{
		this->m_WindowData.ClickedOnResizeButton = false;
	}
	if (pop_style)
	{
		ImGui::PopStyleColor(2);
	}
}

void spe::UIInspector::RenderBackgroundBehindComponent()
{
	const ImVec2 temp = ImGui::GetCursorPos();
	float windowWidth = (float)spe::EngineData::s_WindowWidth;
	ImGui::SetCursorPosX(windowWidth - this->m_Size.x);
	ImGui::SetCursorPosY((ImGui::GetCursorPosY() + 76) - ImGui::GetScrollY());
	spe::UIUtility::DrawRectangleInGUIWIndow(ImVec2(this->m_Size.x, 27), ImGui::GetCursorPos(), COMPONENT_SELECTED_COLOR);
	ImGui::SetCursorPos(temp);
}

void spe::UIInspector::DisplayDefaultInspectorView()
{
	ImGui::Dummy(ImVec2(0, 15));
	this->BackgroundSetting();
	DUMMY_COMPONENT;
	this->GameEngineViewSetting();
	DUMMY_COMPONENT;
	this->GeneralSettings();


	const ImVec2 temp = ImGui::GetCursorPos();
	ImGui::SetCursorPosY(43.0f);
	ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - SEARCH_BAR_MARGIN);
	ImGui::SetNextItemWidth(150);
	ImGui::Text("Game Window");

	ImGui::SetCursorPos(temp);
}


void spe::UIInspector::BackgroundSetting()
{
	this->RenderBackgroundBehindComponent();
	if (ImGui::TreeNode("Background"))
	{
		ImGui::Dummy(ImVec2(0, 16));

		ImVec2 cursorPs = ImGui::GetCursorPos();
		
		ImVec4 clearColor = ImVec4(this->m_ptr_GUIRepo->BackgroundColor.X / 255, this->m_ptr_GUIRepo->BackgroundColor.Y / 255, this->m_ptr_GUIRepo->BackgroundColor.Z / 255, 1.0f);
		ImGui::ColorEdit3("##Background-Color", (float*)&clearColor);

		this->m_ptr_GUIRepo->BackgroundColor.X = clearColor.x * 255;
		this->m_ptr_GUIRepo->BackgroundColor.Y = clearColor.y * 255;
		this->m_ptr_GUIRepo->BackgroundColor.Z = clearColor.z * 255;
		ImGui::Dummy(ImVec2(0, 12));
		ImGui::TreePop();
	}
}

void spe::UIInspector::GameEngineViewSetting()
{
	this->RenderBackgroundBehindComponent();

	const std::string button_name = std::string(ICON_FA_COG) + "##" + "camera-comp";
	const ImVec2 temp_pos = ImGui::GetCursorPos();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY());
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - 40);

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 2));
	if (spe::Style::DisplaySmybolAsButton(button_name.c_str()))
	{
		ImGui::OpenPopup(button_name.c_str());
		this->m_PopUpCursorPos = ImVec2(spe::UIUtility::GUICursor.Position.X - 150, spe::UIUtility::GUICursor.Position.Y + 20);
	}

	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize + 0.2f);

	ImGui::SetNextWindowPos(this->m_PopUpCursorPos);
	if (ImGui::BeginPopup(button_name.c_str()))
	{
		if (spe::Style::DisplaySymbolInMenuItemWithText(ICON_FA_RETWEET, "Reset", 30))
		{
			this->m_ptr_GUIRepo->Camera.Reset();
		}
		
		ImGui::EndPopup();
	}

	ImGui::PopStyleVar();
	ImGui::SetCursorPos(temp_pos);

	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);

	this->m_CamZoom = this->m_ptr_GUIRepo->Camera.GetZoom();

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
		ImGui::InputFloat("##X", &this->m_ptr_GUIRepo->Camera.Position.X, 0, 0, "%g");

		//Y
		ImGui::SetCursorPos(ImVec2(x += 125, y += 2));
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::SetCursorPos(ImVec2(x += 40, y -= 2));
		ImGui::InputFloat("##Y", &this->m_ptr_GUIRepo->Camera.Position.Y, 0, 0, "%g");

		//Zoom
		ImGui::Dummy(ImVec2(0, 10));
		ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize + 0.1f);
		ImGui::Text("Zoom");
		ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2(x -= 120, y += 45));
		ImGui::SliderFloat("##Zoom", &this->m_CamZoom, 0.1f, 4.0f, "%g");

		ImGui::Dummy(ImVec2(0, 15));
		ImGui::Text("Speed");
		ImGui::PushItemWidth(100);
		ImGui::SetCursorPos(ImVec2(x, ImGui::GetCursorPosY() - 25));
		ImGui::InputFloat("##speed-camera", &this->m_ptr_GUIRepo->Camera.CameraSpeed, 0, 0, "%g");

		this->m_ptr_GUIRepo->Camera.SetZoom(this->m_CamZoom);
		ImGui::TreePop();
	}
}

#pragma region Component displaayer

void spe::UIInspector::SetCompontents()
{
	if (this->m_ComponentSelected == "BoxCollider")
	{
		this->m_ptr_GUIRepo->InspectorSprite->Collider.Exist = true;
		this->m_ComponentSelected = " ";
	}
	if (this->m_ComponentSelected == "PhysicsBody")
	{
		this->m_ptr_GUIRepo->InspectorSprite->Physicsbody.Exist = true;
		this->m_ComponentSelected = " ";
	}
	if (this->m_ComponentSelected == "Animator")
	{
		this->m_ptr_GUIRepo->InspectorSprite->Animator.Exist = true;
		this->m_ComponentSelected = " ";
	}
	if (this->m_ComponentSelected == "Prefab")
	{
		this->m_ptr_GUIRepo->InspectorSprite->Prefab.Exist = true;
		this->m_ComponentSelected = " ";
	}
	if (this->m_ComponentSelected == "Light Source")
	{
		this->m_ptr_GUIRepo->InspectorSprite->Light.Enable();
		this->m_ComponentSelected = " ";
	}
	if (this->m_ComponentSelected == "Audio")
	{
		this->m_ptr_GUIRepo->InspectorSprite->Audio.Exist = true;
		this->m_ComponentSelected = " ";
	}
	if (this->m_ComponentSelected == "Particles")
	{
		this->m_ptr_GUIRepo->InspectorSprite->Particles.Exist = true;
		this->m_ComponentSelected = " ";
	}
}

void spe::UIInspector::RenameSprite()
{
	static float s_TimeToUpdate = 0.0f;

	s_TimeToUpdate += spe::Time::s_DeltaTime;

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

				if (sprite_2->Name == sprite_1->Name && sprite_2->GetId() != sprite_1->GetId())
				{
					sprite_2->Name += "(D)";
				}
			}
		}
	}
}

void spe::UIInspector::ComponentSelector()
{
	const ImVec2 temp = ImGui::GetCursorPos();
	ImGui::SetCursorPosY(38.5f);
	ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - SEARCH_BAR_MARGIN);
	ImGui::SetNextItemWidth(150);
	static char input_buffer[255];
	COPY_STRING(input_buffer, this->m_ptr_GUIRepo->InspectorSprite->Name.c_str());
	ImGui::InputText("##input-sprite-name", input_buffer, 255);

	if (input_buffer[0] != '\0')
	{
		this->m_ptr_GUIRepo->InspectorSprite->Name = std::string(input_buffer);
	}

	const ImVec2 temp2 = ImGui::GetCursorPos();
	ImGui::SetCursorPos(ImVec2(10, 50));

	const std::string text = "ID:" + std::to_string(this->m_ptr_GUIRepo->InspectorSprite->GetId());
	ImGui::Text(text.c_str());

	ImGui::SetCursorPos(temp2);

	this->m_TagSelector.Render();
	this->Hovered = this->m_TagSelector.Hovered;
	ImGui::SetCursorPos(temp);

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
	ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ADD_COMPONENTS_MARGIN);
	ImGui::SetNextItemWidth(250);
	if (ImGui::BeginCombo("##Components", "Add Components"))
	{
		for (int i = 0; i < this->m_ComponentsName.size(); i++)
		{
			bool is_selected = (this->m_ComponentSelected == this->m_ComponentsName[i]);
			if (ImGui::Selectable(this->m_ComponentsName[i], is_selected))
			{
				this->m_ComponentSelected = this->m_ComponentsName[i];
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void spe::UIInspector::SetupComponents()
{
	ImGui::Dummy(ImVec2(0, 15));
	// Base Components
	this->TransformComponent();
	DUMMY_COMPONENT;


	this->SpriteRendererComponent();
	DUMMY_COMPONENT;
	
	//Collider
	if (this->m_ptr_GUIRepo->InspectorSprite->Collider.Exist)
	{
		this->BoxColliderComponent();
		DUMMY_COMPONENT;
	}
	else
	{
		this->m_Collider.LeaveEditMode();
		this->m_ptr_ColliderRec->Render = false;
	}

	//PhysicsBody
	if (this->m_ptr_GUIRepo->InspectorSprite->Physicsbody.Exist)
	{
		this->PhysicsBodyComponent();
		DUMMY_COMPONENT;
	}

	//Animator
	if (this->m_ptr_GUIRepo->InspectorSprite->Animator.Exist)
	{
		this->AnimatorComponent();
		DUMMY_COMPONENT;
	}

	// Prefab
	if (this->m_ptr_GUIRepo->InspectorSprite->Prefab.Exist)
	{
		this->PrefabComponent();
		DUMMY_COMPONENT;
	}

	// Light Source
	if (this->m_ptr_GUIRepo->InspectorSprite->Light.Exist)
	{
		this->LightComponent();
		DUMMY_COMPONENT;
	}

	if (this->m_ptr_GUIRepo->InspectorSprite->Audio.Exist)
	{
		this->AudioComponent();
		DUMMY_COMPONENT;
	}

	if (this->m_ptr_GUIRepo->InspectorSprite->Particles.Exist)
	{
		this->ParticlesComponent();
		DUMMY_COMPONENT;
	}

	this->ComponentSelector();
	this->SetCompontents();
}

void spe::UIInspector::InputXY(const char* label, float& inputX, float& inputY, float x, float y)
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
}

void spe::UIInspector::TransformComponent()
{
	spe::Vector2 tempPos = this->m_ptr_GUIRepo->InspectorSprite->Transform.GetPosition();
	this->RenderBackgroundBehindComponent();
	this->RenderComponentOptions(this->m_ptr_GUIRepo->InspectorSprite->Transform, "Transform");
	if (ImGui::TreeNode("Transform"))
	{
		float x = ImGui::GetCursorPosX();
		float y = ImGui::GetCursorPosY();

		ImGui::Dummy(ImVec2(0, 8));
		InputXY("Position", tempPos.X, tempPos.Y, x, y);

		y += 45;

		spe::Vector2 temp_scale = this->m_ptr_GUIRepo->InspectorSprite->Transform.GetScale();

		ImGui::Dummy(ImVec2(0, 10));
		InputXY("Scale", temp_scale.X, temp_scale.Y, x, y);
		y += 45;

		int rotation = this->m_ptr_GUIRepo->InspectorSprite->Transform.GetRotation();
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

		this->m_ptr_GUIRepo->InspectorSprite->Transform.SetScale(temp_scale);
		this->m_ptr_GUIRepo->InspectorSprite->Transform.SetRotation(rotation);
		this->m_ptr_GUIRepo->InspectorSprite->Transform.SetPosition(tempPos);

		ImGui::TreePop();
	}
}

void spe::UIInspector::SpriteRendererComponent()
{
	this->RenderBackgroundBehindComponent();
	this->RenderComponentOptions(this->m_ptr_GUIRepo->InspectorSprite->SpriteRenderer, "Sprite Renderer");
	if (ImGui::TreeNode("Sprite Renderer"))
	{
		std::string input = spe::Utility::GetNamePathSplit(this->m_ptr_GUIRepo->InspectorSprite->SpriteRenderer.Path);
		this->m_SpriteName = this->m_ptr_GUIRepo->InspectorSprite->Name;

		float y = ImGui::GetCursorPos().y;
		float x = ImGui::GetCursorPos().x;

		ImGui::SetCursorPos(ImVec2(x += 20.0f, y += 10));
		ImGui::Text("Sprite");

		ImGui::SetCursorPos(ImVec2(x += 100, y - 5));
		ImGui::SetNextItemWidth(this->m_SpriteInputWidth);
		ImGui::InputText("##spriteRenderer", &input[0], CHARM_MAX_BUFFER);

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped) && ImGui::IsMouseReleased(0) && this->m_ptr_GUIRepo->DragAndDropPath!= " ")
		{
			this->m_ptr_GUIRepo->InspectorSprite->SetSpriteTexture(this->m_ptr_GUIRepo->DragAndDropPath);
		}

		ImGui::Dummy(ImVec2(0, 3.8f));

		//Sorting Layer
		ImGui::SetCursorPosX(x -= 100.0f);
		ImGui::Text("Sorting Layer");
		ImGui::SameLine();
		ImGui::PushItemWidth(42.0f);
		int32_t layerIndex = this->m_ptr_GUIRepo->InspectorSprite->SpriteRenderer.SortinLayerIdx;
		ImGui::InputInt("##Sorting Layer", &layerIndex, 0, 0);
		ImGui::PopItemWidth();

		if (layerIndex < 0)
		{
			layerIndex = 0;
		}

		if ((uint32_t)layerIndex != this->m_ptr_GUIRepo->InspectorSprite->SpriteRenderer.SortinLayerIdx)
		{
			this->m_ptr_Repo->SetSpriteSortingLayer(layerIndex, this->m_ptr_GUIRepo->InspectorSprite);
		}

		ImGui::Dummy(ImVec2(0, 2));
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
		ImGui::Text("Effected by light");
		spe::UIUtility::SameLine(0);
		ImGui::Checkbox("##light-checkbox", &this->m_ptr_GUIRepo->InspectorSprite->SpriteRenderer.EffectedByLight);
		ImGui::Dummy(ImVec2(0, 7));

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
		ImGui::Text("Render (ingame)");
		spe::UIUtility::SameLine(0);
		ImGui::Checkbox("##render-checkbox", &this->m_ptr_GUIRepo->InspectorSprite->SpriteRenderer.Render);

		ImGui::TreePop();
	}
}

void spe::UIInspector::BoxColliderComponent()
{
	float x = ImGui::GetCursorPosX();
	float y = ImGui::GetCursorPosY();

	this->RenderBackgroundBehindComponent();
	this->RenderComponentOptions(this->m_ptr_GUIRepo->InspectorSprite->Collider, "BoxCollider");
	if (ImGui::TreeNode("BoxCollider"))
	{
		this->m_ptr_ColliderRec->Render = true;
		ImGui::Dummy(ImVec2(0, 4));
		x = ImGui::GetCursorPos().x;
		y = ImGui::GetCursorPos().y;

		this->m_Collider.Edit(x, y);
		this->m_Collider.Solid(x, y, this->m_ptr_GUIRepo->InspectorSprite);
		this->m_Collider.Width(x, y, this->m_ptr_GUIRepo->InspectorSprite);
		this->m_Collider.Height(this->m_ptr_GUIRepo->InspectorSprite);
		this->m_Collider.Controller(this->m_ptr_GUIRepo->InspectorSprite);

		ImGui::TreePop();

		//Transparent since we open the boxcollider and we want to open the colider (rec)
		this->m_Collider.DrawBoxCollider(this->m_ptr_GUIRepo->InspectorSprite, this->m_ptr_ColliderRec);
		ImGui::Dummy(ImVec2(0, 9));
	}
	else
	{
		this->m_Collider.LeaveEditMode();
		this->m_ptr_ColliderRec->Render = false;
	}
}

void spe::UIInspector::PhysicsBodyComponent()
{
	float x = ImGui::GetCursorPosX();
	float y = ImGui::GetCursorPosY();

	this->RenderBackgroundBehindComponent();
	this->RenderComponentOptions(this->m_ptr_GUIRepo->InspectorSprite->Physicsbody, "PhysicsBody");

	if (ImGui::TreeNode("PhysicsBody"))
	{
		ImGui::Dummy(ImVec2(0, 8));

		x = ImGui::GetCursorPosX();
	    y = ImGui::GetCursorPosY();

		ImGui::SetCursorPosX(55);
		ImGui::Text("Gravity");
		ImGui::PushItemWidth(55);
		ImGui::SetCursorPos(ImVec2(x += 125, y -= 6.0f));
		ImGui::InputFloat("##gravity", &this->m_ptr_GUIRepo->InspectorSprite->Physicsbody.Gravity, 0, 0, "%g");
		ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
		ImGui::PopItemWidth();

		ImGui::Dummy(ImVec2(0, 5));

		ImGui::SetCursorPosX(55);
		ImGui::Text("Mass");
		ImGui::PushItemWidth(55);
		ImGui::SetCursorPos(ImVec2(x, y += 45.0f));
		ImGui::InputFloat("##mass", &this->m_ptr_GUIRepo->InspectorSprite->Physicsbody.Mass, 0, 0, "%g");
		ImGui::PopItemWidth();

		ImGui::Dummy(ImVec2(0, 5));

		ImGui::SetCursorPosX(55);
		ImGui::Text("Use Air Friction");
		ImGui::PushItemWidth(55);
		ImGui::SetCursorPos(ImVec2(x += 20, y += 40.0f));
		ImGui::Checkbox("##air-friction", &this->m_ptr_GUIRepo->InspectorSprite->Physicsbody.UseAirFriction);
		ImGui::PopItemWidth();

		if (this->m_ptr_GUIRepo->SimulatePhysics)
		{
			spe::Vector2 velocity = this->m_ptr_GUIRepo->InspectorSprite->Physicsbody.Velocity;

			ImGui::SetCursorPosX(40);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			InputXY("Velocity", velocity.X, velocity.Y, 40, ImGui::GetCursorPosY() - 10);
			this->m_ptr_GUIRepo->InspectorSprite->Physicsbody.Velocity = velocity;
		}

		ImGui::TreePop();
	}	
}

void spe::UIInspector::AnimatorComponent()
{
	float y = ImGui::GetCursorPos().y;
	float x = ImGui::GetCursorPos().x;

	this->RenderBackgroundBehindComponent();
	this->RenderComponentOptions(this->m_ptr_GUIRepo->InspectorSprite->Animator, "Animator");
	if (ImGui::TreeNode("Animator"))
	{
		y = ImGui::GetCursorPos().y;
		x = ImGui::GetCursorPos().x;

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

void spe::UIInspector::PrefabComponent()
{
	float x = ImGui::GetCursorPosX();
	float y = ImGui::GetCursorPosY();

	this->RenderBackgroundBehindComponent();
	if (ImGui::TreeNode("Prefab"))
	{
		ImGui::SetCursorPos(ImVec2(x += 45, y += 40));
		const std::string fileLocation = "File location: " + this->m_ptr_GUIRepo->InspectorSprite->Prefab.PathToFile;
		ImGui::Text(fileLocation.c_str());

		ImGui::SetCursorPos(ImVec2(x, y + 40));
		ImGui::Text("Update file");
		ImGui::SetCursorPos(ImVec2(x + 120, y + 35));
		if (spe::Style::DisplaySmybolAsButton(ICON_FA_RETWEET))
		{
			// TODO: Delete prefab component when the file is moved (user dumb haha)
			this->m_ptr_GUIRepo->InspectorSprite->Prefab.UpdatePath();
			const spe::Prefab& prefab = this->m_ptr_GUIRepo->InspectorSprite->Prefab;
			spe::Savesystem::CreateOrUpdatePrefabFile(this->m_ptr_GUIRepo->InspectorSprite,
				prefab.PathToFile, prefab.PathToOldFile);

		}
		ImGui::SetCursorPos(ImVec2(x, y + 80));
		ImGui::Text("Load in memory ");
		ImGui::SetCursorPos(ImVec2(x + 170, y + 75));
		ImGui::Checkbox("##LoadInMemory", &this->m_ptr_GUIRepo->InspectorSprite->Prefab.LoadInMemory);
		ImGui::TreePop();
	}	
}

void spe::UIInspector::LightComponent()
{
	this->m_LightRadius = this->m_ptr_GUIRepo->InspectorSprite->Light.GetRadius();
	this->m_LightIntensity = this->m_ptr_GUIRepo->InspectorSprite->Light.GetIntensity();


	this->RenderBackgroundBehindComponent();
	this->RenderComponentOptions(this->m_ptr_GUIRepo->InspectorSprite->Light, "Light Source");
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
		ImGui::InputFloat("##light-radius", &this->m_LightRadius, 0, 0, "%g");

		this->m_ptr_GUIRepo->InspectorSprite->Light.SetRadius(this->m_LightRadius);

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 14.5f);
		ImGui::Text("Intensity: ");
		spe::UIUtility::SameLine(0);
		ImGui::SetNextItemWidth(50);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 38);
		ImGui::InputFloat("##light-intensity", &this->m_LightIntensity, 0, 0, "%g");

		this->m_ptr_GUIRepo->InspectorSprite->Light.SetIntensity(this->m_LightIntensity);

		
		ImGui::Text("Color: ");
		const sf::Vector3f& clr = this->m_ptr_GUIRepo->InspectorSprite->Light.GetColor();
		ImVec4 clear_color = ImVec4(clr.x, clr.y, clr.z, 1.0f);
		ImGui::ColorEdit3("##light-Color", (float*)&clear_color);

		this->m_ptr_GUIRepo->InspectorSprite->Light.SetColor(sf::Vector3f(clear_color.x, clear_color.y, clear_color.z));

		ImGui::TreePop();
	}
}

void spe::UIInspector::AudioComponent()
{
	this->m_LightRadius = this->m_ptr_GUIRepo->InspectorSprite->Light.GetRadius();
	this->m_LightIntensity = this->m_ptr_GUIRepo->InspectorSprite->Light.GetIntensity();
	float y = ImGui::GetCursorPos().y;
	float x = ImGui::GetCursorPos().x;

	this->RenderBackgroundBehindComponent();
	this->RenderComponentOptions(this->m_ptr_GUIRepo->InspectorSprite->Audio, "Audio");
	if (ImGui::TreeNode("Audio"))
	{
		std::vector<spe::AudioInfo> infos = this->m_ptr_GUIRepo->InspectorSprite->Audio.GetAllAudiosInfo();
		if (Style::DisplaySymbolButtonWithText(ICON_FA_PLUS, "Add Audio"))
		{
			std::string name;
			bool nameExists;
			do {
			    std::random_device rd;
			    std::mt19937 gen(rd());
			    std::uniform_int_distribution<> dis(100000, 999999);
			    name = "Audio_" + std::to_string(dis(gen));
			    nameExists = this->m_ptr_GUIRepo->InspectorSprite->Audio.GetAudioByName(name) != nullptr;

			} while (nameExists);
			this->m_ptr_GUIRepo->InspectorSprite->Audio.AddAudio(
			new Audio("", Audio::AudioType::MUSIC, name));
			this->m_ptr_GUIRepo->ptr_CurrentAudio = this->m_ptr_GUIRepo->InspectorSprite->Audio.GetAudioByName(name);
			this->m_ptr_GUIRepo->AudioData.IsOpen = true;
			this->m_ptr_GUIRepo->AudioData.Reload = true;
		}
		for (auto &[Name, Path, Type] : infos) {
			ImGui::PushID(Name.c_str());
			y = ImGui::GetCursorPos().y;
			x = ImGui::GetCursorPos().x;

			ImGui::SetCursorPos(ImVec2(x += 8.0f, y += 10));

			ImGui::SetCursorPosX(x += 4);
			if (Style::DisplaySmybolAsButton(ICON_FA_EDIT))
			{
				Audio* audio = this->m_ptr_GUIRepo->InspectorSprite->Audio.GetAudioByName(Name);
				if (audio != nullptr && audio == this->m_currentlyPlaying) {
					// audio cant be nullptr; thus currentlyPlaying cant be nullptr either
					// ReSharper disable once CppDFANullDereference
					this->m_currentlyPlaying->Stop();
				}
				this->m_ptr_GUIRepo->ptr_CurrentAudio = audio;
				this->m_ptr_GUIRepo->AudioData.IsOpen = true;
				this->m_ptr_GUIRepo->AudioData.Reload = true;
			}
			ImGui::SameLine();
			// ImGui::SetCursorPosX(x += 2);
			// preview play
			if (Style::DisplaySmybolAsButton(ICON_FA_PLAY))
			{
				if (Audio* audio = this->m_ptr_GUIRepo->InspectorSprite->Audio.GetAudioByName(Name); audio != nullptr)
				{
					// is audio playing
					if (audio == this->m_currentlyPlaying)
					{
						// audio cant be nullptr, currentlyplaying cant be nullptr bc its equal to audio
						// ReSharper disable once CppDFANullDereference
						this->m_currentlyPlaying->Stop();
					}
					else
					{
						// if current audio is playing
						if (this->m_currentlyPlaying != nullptr) {
							if (this->m_currentlyPlaying->GetStatus() == sf::SoundSource::Status::Playing) {
								this->m_currentlyPlaying->Stop();
							}
						}
						this->m_currentlyPlaying = audio;
						this->m_wasCurrentlyPlayingLooping = this->m_currentlyPlaying->GetLoop();
						if (this->m_currentlyPlaying->GetType() == Audio::AudioType::SOUND) {
							this->m_currentlyPlaying->SetRelativeToListener(true);
						}

						if (this->m_wasCurrentlyPlayingLooping) {
							this->m_currentlyPlaying->SetLoop(false);
						}
						this->m_currentlyPlaying->Load();
						this->m_currentlyPlaying->Play();
					}
				}
			}

			if (this->m_currentlyPlaying != nullptr && this->m_currentlyPlaying->GetStatus() == sf::SoundSource::Status::Stopped) {
				if (this->m_wasCurrentlyPlayingLooping) {
					this->m_currentlyPlaying->SetLoop(true);
				}
				if (this->m_currentlyPlaying->GetType() == Audio::AudioType::SOUND) {
					this->m_currentlyPlaying->SetRelativeToListener(false);
				}
				this->m_currentlyPlaying = nullptr;
			}

			ImGui::SameLine();
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);

			// get file name from path
			std::string file = spe::Utility::GetNamePathSplit(Path);
			const std::string::size_type pos = file.find_last_of('.');
			std::string fName = file.substr(0, pos);
			if (fName.length() > 20) {
				fName = fName.substr(0, 8)
						+ "[...]"
						+ fName.substr(fName.length() - 8);
			}
			fName += "." + file.substr(pos + 1);

			ImGui::Text("%s (%s):", Name.c_str(), Type.c_str());
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 100);
			ImGui::Text("%s", fName.c_str());
			ImGui::PopID();
		}

		ImGui::TreePop();
	}
}

void spe::UIInspector::ParticlesComponent()
{
	float y = ImGui::GetCursorPos().y;
	float x = ImGui::GetCursorPos().x;

	this->RenderBackgroundBehindComponent();
	this->RenderComponentOptions(this->m_ptr_GUIRepo->InspectorSprite->Particles, "Particles");
	if (ImGui::TreeNode("Particles"))
	{
		y = ImGui::GetCursorPos().y;
		x = ImGui::GetCursorPos().x;

		ImGui::Dummy(ImVec2(0, 8));

		// Play/Stop buttons
		ImGui::SetCursorPosX(x + 20);

		if (this->m_ptr_GUIRepo->InspectorSprite->Particles.GetParticleSystem().IsEmitting())
		{
			if (spe::Style::DisplaySymbolButtonWithText(ICON_FA_STOP, "Stop"))
			{
				this->m_ptr_GUIRepo->InspectorSprite->Particles.Stop();
			}
		}
		else
		{
			if (spe::Style::DisplaySymbolButtonWithText(ICON_FA_PLAY, "Play"))
			{
				this->m_ptr_GUIRepo->InspectorSprite->Particles.Play();
			}
		}

		ImGui::SameLine();
		if (spe::Style::DisplaySymbolButtonWithText(ICON_FA_TRASH, "Clear"))
		{
			this->m_ptr_GUIRepo->InspectorSprite->Particles.Clear();
		}

		ImGui::Dummy(ImVec2(0, 5));

		// Texture Path
		ImGui::SetCursorPosX(x + 20);
		ImGui::Text("Texture");
		ImGui::SameLine();

		static char texturePath[512] = "";
		const std::string& currentPath = this->m_ptr_GUIRepo->InspectorSprite->Particles.GetTexturePath();
		strncpy(texturePath, currentPath.c_str(), sizeof(texturePath) - 1);
		texturePath[sizeof(texturePath) - 1] = '\0';

		ImGui::SetNextItemWidth(150);
		if (ImGui::InputText("##particleTexture", texturePath, sizeof(texturePath)))
		{
			this->m_ptr_GUIRepo->InspectorSprite->Particles.SetTexturePath(std::string(texturePath));
		}

		// Drag and drop support
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped) && ImGui::IsMouseReleased(0) && this->m_ptr_GUIRepo->DragAndDropPath != " ")
		{
			this->m_ptr_GUIRepo->InspectorSprite->Particles.SetTexturePath(this->m_ptr_GUIRepo->DragAndDropPath);
		}

		ImGui::SameLine();
		if (spe::Style::DisplaySmybolAsButton(ICON_FA_TIMES))
		{
			this->m_ptr_GUIRepo->InspectorSprite->Particles.SetTexturePath("");
		}

		ImGui::Dummy(ImVec2(0, 5));

		// Get config reference
		spe::ParticleEmitterConfig& config = this->m_ptr_GUIRepo->InspectorSprite->Particles.GetConfig();

		// Play On Start
		ImGui::SetCursorPosX(x + 20);
		ImGui::Text("Play On Start");
		ImGui::SameLine();
		ImGui::Checkbox("##playOnStart", &config.PlayOnStart);

		// Emission Rate
		ImGui::SetCursorPosX(x + 20);
		ImGui::Text("Emission Rate");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::InputFloat("##emissionRate", &config.EmissionRate, 0, 0, "%.1f");

		// Max Particles
		ImGui::SetCursorPosX(x + 20);
		ImGui::Text("Max Particles");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		int maxParticles = static_cast<int>(config.MaxParticles);
		if (ImGui::InputInt("##maxParticles", &maxParticles, 0, 0))
		{
			if (maxParticles > 0)
			{
				config.MaxParticles = static_cast<uint32_t>(maxParticles);
			}
		}

		// Lifetime
		ImGui::SetCursorPosX(x + 20);
		ImGui::Text("Lifetime");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60);
		ImGui::InputFloat("##lifetimeMin", &config.LifetimeMin, 0, 0, "%.1f");
		ImGui::SameLine();
		ImGui::Text("-");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60);
		ImGui::InputFloat("##lifetimeMax", &config.LifetimeMax, 0, 0, "%.1f");

		// Speed
		ImGui::SetCursorPosX(x + 20);
		ImGui::Text("Speed");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60);
		ImGui::InputFloat("##speedMin", &config.SpeedMin, 0, 0, "%.1f");
		ImGui::SameLine();
		ImGui::Text("-");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60);
		ImGui::InputFloat("##speedMax", &config.SpeedMax, 0, 0, "%.1f");

		// Size
		ImGui::SetCursorPosX(x + 20);
		ImGui::Text("Start Size");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60);
		ImGui::InputFloat("##startSizeMin", &config.StartSizeMin, 0, 0, "%.1f");
		ImGui::SameLine();
		ImGui::Text("-");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60);
		ImGui::InputFloat("##startSizeMax", &config.StartSizeMax, 0, 0, "%.1f");

		ImGui::SetCursorPosX(x + 20);
		ImGui::Text("End Size");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60);
		ImGui::InputFloat("##endSizeMin", &config.EndSizeMin, 0, 0, "%.1f");
		ImGui::SameLine();
		ImGui::Text("-");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60);
		ImGui::InputFloat("##endSizeMax", &config.EndSizeMax, 0, 0, "%.1f");

		// Start Color
		ImGui::SetCursorPosX(x + 20);
		ImGui::Text("Start Color");
		ImVec4 startColor = ImVec4(
			config.StartColorMin.r / 255.0f,
			config.StartColorMin.g / 255.0f,
			config.StartColorMin.b / 255.0f,
			config.StartColorMin.a / 255.0f
		);
		if (ImGui::ColorEdit4("##startColor", reinterpret_cast<float *>(&startColor)))
		{
			config.StartColorMin = sf::Color(
				static_cast<sf::Uint8>(startColor.x * 255),
				static_cast<sf::Uint8>(startColor.y * 255),
				static_cast<sf::Uint8>(startColor.z * 255),
				static_cast<sf::Uint8>(startColor.w * 255)
			);
			config.StartColorMax = config.StartColorMin;
		}

		// End Color
		ImGui::SetCursorPosX(x + 20);
		ImGui::Text("End Color");
		ImVec4 endColor = ImVec4(
			config.EndColorMin.r / 255.0f,
			config.EndColorMin.g / 255.0f,
			config.EndColorMin.b / 255.0f,
			config.EndColorMin.a / 255.0f
		);
		if (ImGui::ColorEdit4("##endColor", reinterpret_cast<float *>(&endColor)))
		{
			config.EndColorMin = sf::Color(
				static_cast<sf::Uint8>(endColor.x * 255),
				static_cast<sf::Uint8>(endColor.y * 255),
				static_cast<sf::Uint8>(endColor.z * 255),
				static_cast<sf::Uint8>(endColor.w * 255)
			);
			config.EndColorMax = config.EndColorMin;
		}

		// Gravity
		ImGui::SetCursorPosX(x + 20);
		ImGui::Text("Gravity");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60);
		ImGui::InputFloat("##gravityX", &config.Gravity.X, 0, 0, "%.1f");
		ImGui::SameLine();
		ImGui::Text(",");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60);
		ImGui::InputFloat("##gravityY", &config.Gravity.Y, 0, 0, "%.1f");

		// Drag
		ImGui::SetCursorPosX(x + 20);
		ImGui::Text("Drag");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::InputFloat("##drag", &config.Drag, 0, 0, "%.2f");

		// Direction
		ImGui::SetCursorPosX(x + 20);
		ImGui::Text("Direction");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60);
		ImGui::InputFloat("##dirX", &config.Direction.X, 0, 0, "%.2f");
		ImGui::SameLine();
		ImGui::Text(",");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60);
		ImGui::InputFloat("##dirY", &config.Direction.Y, 0, 0, "%.2f");

		// Direction Spread
		ImGui::SetCursorPosX(x + 20);
		ImGui::Text("Spread");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::SliderFloat("##spread", &config.DirectionSpread, 0.0f, 360.0f, "%.1f");

		// Offset
		ImGui::SetCursorPosX(x + 20);
		ImGui::Text("Offset");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60);
		ImGui::InputFloat("##offsetX", &config.Offset.X, 0, 0, "%.1f");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60);
		ImGui::InputFloat("##offsetY", &config.Offset.Y, 0, 0, "%.1f");

		ImGui::Dummy(ImVec2(0, 5));

		// Emission Shape
		ImGui::SetCursorPosX(x + 20);
		ImGui::Text("Emission Shape");
		ImGui::SameLine();
		const char* shapeNames[] = { "Point", "Circle", "Rectangle", "Line", "Edge" };
		int currentShape = static_cast<int>(config.Shape);
		ImGui::SetNextItemWidth(100);
		if (ImGui::Combo("##emissionShape", &currentShape, shapeNames, IM_ARRAYSIZE(shapeNames)))
		{
			config.Shape = static_cast<spe::EmissionShape>(currentShape);
		}

		// Shape-specific settings
		if (config.Shape == spe::EmissionShape::Circle)
		{
			ImGui::SetCursorPosX(x + 20);
			ImGui::Text("Radius");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(80);
			ImGui::InputFloat("##shapeRadius", &config.ShapeRadius, 0, 0, "%.1f");
		}
		else if (config.Shape == spe::EmissionShape::Rectangle || config.Shape == spe::EmissionShape::Edge)
		{
			ImGui::SetCursorPosX(x + 20);
			ImGui::Text("Size");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(60);
			ImGui::InputFloat("##shapeSizeX", &config.ShapeSize.X, 0, 0, "%.1f");
			ImGui::SameLine();
			ImGui::Text("x");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(60);
			ImGui::InputFloat("##shapeSizeY", &config.ShapeSize.Y, 0, 0, "%.1f");
		}
		else if (config.Shape == spe::EmissionShape::Line)
		{
			ImGui::SetCursorPosX(x + 20);
			ImGui::Text("Length");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(80);
			ImGui::InputFloat("##lineLength", &config.ShapeSize.X, 0, 0, "%.1f");

			ImGui::SetCursorPosX(x + 20);
			ImGui::Text("Line Angle");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(80);
			ImGui::SliderFloat("##lineAngle", &config.LineAngle, 0.0f, 360.0f, "%.1f");
		}

		ImGui::Dummy(ImVec2(0, 5));

		// Blend Mode
		ImGui::SetCursorPosX(x + 20);
		ImGui::Text("Additive Blend");
		ImGui::SameLine();
		bool isAdditive = (config.BlendMode == sf::BlendAdd);
		if (ImGui::Checkbox("##blendMode", &isAdditive))
		{
			config.BlendMode = isAdditive ? sf::BlendAdd : sf::BlendAlpha;
		}

		// Follow Sprite
		ImGui::SetCursorPosX(x + 20);
		ImGui::Text("Follow Sprite");
		ImGui::SameLine();
		bool followSprite = this->m_ptr_GUIRepo->InspectorSprite->Particles.GetFollowSprite();
		if (ImGui::Checkbox("##followSprite", &followSprite))
		{
			this->m_ptr_GUIRepo->InspectorSprite->Particles.SetFollowSprite(followSprite);
		}

		// Active particle count
		ImGui::Dummy(ImVec2(0, 5));
		ImGui::SetCursorPosX(x + 20);
		uint32_t activeCount = this->m_ptr_GUIRepo->InspectorSprite->Particles.GetParticleSystem().GetActiveParticleCount();
		ImGui::Text("Active Particles: %u", activeCount);

		ImGui::TreePop();
	}
}

#pragma endregion

//Public functions

void spe::UIInspector::Render()
{
    float windowWidth = (float)spe::EngineData::s_WindowWidth;
    float windowHeight = (float)spe::EngineData::s_WindowHeight;

    // Subtract Console Height (480) and TopBar (56)
    ImVec2 dynamicDefaultSize = ImVec2(INSPECTOR_DEFAULT_WINDOW_SIZE.x, windowHeight - 56.0f - 480.0f);

	if (spe::UIUtility::HandleCloseAndReloadWindow(this->m_ptr_GUIRepo->InspectorData, this->Hovered, dynamicDefaultSize))
	{
		return;
	}

	this->Hovered = false;
	ImGui::Begin("##sprite-inspector", NULL, DEFAULT_FLAGS);

	// Close rectangle
	this->m_ptr_GUIRepo->InspectorData.IsOpen = spe::UIUtility::RenderCloseRectangle(this->m_Size.x - 300, ICON_FA_EDIT, "##inspector", "Inspector", 0);

	// Left arrow
	this->RenderOptions();

	this->RenameSprite();

	//Setting it here transparent because if we go down and out box collider is actually getting used it will update to green automatic
	if (this->m_ptr_GUIRepo->InspectorSprite != nullptr)
	{
		// Handle a sprite

		this->DrawRectangleOverCurrentObject();
		this->SetupComponents();
	}
	else
	{
		// Handle it if no sprite is selected
		this->DisplayDefaultInspectorView();
		this->m_Collider.LeaveEditMode();
		this->m_ptr_SpriteRec->Render = false;
		this->m_ptr_ColliderRec->Render = false;
	}

	const ImVec2 window_pos = ImVec2(windowWidth - this->m_Size.x, INSPECTOR_WINDOW_POS.y);

	if (!this->Hovered)
	{
		this->Hovered = spe::UIUtility::IsHovered(window_pos, dynamicDefaultSize);
	}

	ImGui::SetWindowPos(window_pos);
	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);

    this->m_Size.y = windowHeight - 56.0f - 480.0f;
	ImGui::SetWindowSize(this->m_Size);
	ImGui::End();
}
