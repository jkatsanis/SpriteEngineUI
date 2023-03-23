#include "UIInspector.h"

s2d::UIInspector::UIInspector()
{
	this->m_spriteInputWidth = 200.0f;
	this->m_windowSizeWidth = 390.0f;

	this->m_currentSpriteInInspector = nullptr;
	this->m_menuName = "menu";
	s2d::GameObject::rects.push_back(m_rectangle);
	s2d::GameObject::rects.push_back(this->m_boxCollider);

	this->m_defaultBackgroundColor = s2d::Vector3(139, 165, 187);
	this->state = s2d::InspectorState::None;
	this->m_textureOverSprite.loadFromFile(PATH_TO_RESSOURCS"/Sprites/transparent.png");
}

//Private functions

void s2d::UIInspector::render()
{
	//Setting it here transparent because if we go down and out box collider is actually getting used it will update to green automatic
	s2d::GameObject::rects[1].setOutlineColor(sf::Color(0, 0, 255, 0));
	//s2d::UIHirachy::selectedSprite = s2d::Sprite::activeSprites[3];
	if (s2d::UIHirachy::s_selectedSprite != nullptr)
	{
		this->m_currentSpriteInInspector = s2d::UIHirachy::s_selectedSprite;
		this->state = s2d::InspectorState::SpriteEditorWindow;
		this->m_inputName = &this->m_currentSpriteInInspector->name[0];

		ImGui::SetCursorPos(ImVec2(15, 46));
		ImGui::InputText("##name", this->m_inputName, CHAR_MAX);
		this->m_currentSpriteInInspector->name = std::string(this->m_inputName);

		this->checkDupeName();

		//Spacing
		ImGui::Dummy(ImVec2(0.0f, 6.2f));

		//Line
		ImGui::Separator();
		this->drawRectangleOverCurrentObject();

		//COMPONENTS
		this->setupComponents();
	}
	else //Since we dont clicked a valid sprite we display the default inspector view
	{
		this->state = s2d::InspectorState::GameWindowEditor;
		this->m_currentSpriteInInspector = nullptr;
		this->m_currentComponentSelected = " ";
		this->m_spriteName = " ";

		//Setting the rect to transparent because we dont want to show it when we deletet a sprite/´display default insp view
		s2d::GameObject::rects[0].setOutlineColor(sf::Color(0, 0, 255, 0));

		this->displayDefaultInspectorView();
	}

	//Setting Inspector window size
	ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);
	ImGui::SetWindowPos(ImVec2(1530.0f, 0.0f));
	ImGui::SetWindowSize(ImVec2(this->m_windowSizeWidth, 1080.0f));

	this->isHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenBlockedByPopup);
}

void s2d::UIInspector::checkDupeName()
{
	for (s2d::Sprite* spr : s2d::Sprite::s_sprites)
	{
		if (this->m_currentSpriteInInspector->name == spr->name && this->m_currentSpriteInInspector->getId() != spr->getId())
		{
			this->m_currentSpriteInInspector->name = "<no dupe (" + std::to_string(SpriteData::dupeNameCounter) += ")>";
			SpriteData::dupeNameCounter++;
		}
	}
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
		this->backgroundColor = this->m_defaultBackgroundColor;
	}
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 41 + 10);
    if (ImGui::TreeNode("Background"))
	{
		ImGui::Dummy(ImVec2(0, 16));

		ImVec2 cursorPs = ImGui::GetCursorPos();

	    ImVec4 clearColor = ImVec4(this->backgroundColor.x / 255, this->backgroundColor.y / 255, this->backgroundColor.z / 255, 1.0f);
		ImGui::ColorEdit3("Background-Color", (float*)&clearColor);

		this->backgroundColor.x = clearColor.x * 255;
		this->backgroundColor.y = clearColor.y * 255;
		this->backgroundColor.z = clearColor.z * 255;
		ImGui::Dummy(ImVec2(0, 12));
		ImGui::TreePop();
	}
}

void s2d::UIInspector::gameEngineViewSetting()
{
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 300, ImGui::GetCursorPosY() - 10));

	if (s2d::FontManager::displaySmybolAsButton(ICON_FA_RETWEET "##C"))
	{
		s2d::GameObject::ptr_camera_tRealTimeEditor->reset();
	}
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 41 + 10);
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
		ImGui::InputFloat("##X", &s2d::GameObject::ptr_camera_tRealTimeEditor->transform.position.x, 0, 0, "%g");

		//Y
		ImGui::SetCursorPos(ImVec2(x += 125, y += 2));
		ImGui::Text("Y");
		ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::SetCursorPos(ImVec2(x += 40, y -= 2));
		ImGui::InputFloat("##Y", &s2d::GameObject::ptr_camera_tRealTimeEditor->transform.position.y, 0, 0, "%g");

		//Zoom
		ImGui::Dummy(ImVec2(0, 10));
		ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize + 0.1f);
		ImGui::Text("Zoom");
		ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);
		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2(x -= 120, y += 54));
		ImGui::SliderFloat("##Zoom", &s2d::GameObject::ptr_camera_tRealTimeEditor->cameraZoom, 0.1f, 4.0f, "%g");

		if (s2d::GameObject::ptr_camera_tRealTimeEditor->cameraZoom <= 0)
		{
			s2d::GameObject::ptr_camera_tRealTimeEditor->cameraZoom = 0.8f;
		}

		ImGui::TreePop();
	}
}

#pragma endregion

#pragma region Component displaayer

void s2d::UIInspector::setupComponents()
{
	this->transformComponent();
	this->spriteRendererComponent();

	//Collider
	if (this->m_currentSpriteInInspector->collider.exists)
	{
		this->boxColliderComponent();
	}

	//PhysicsBody
	if (this->m_currentSpriteInInspector->physicsBody.exists)
	{
		this->physicsBodyComponent();
	}

	//Animator
	if (this->m_currentSpriteInInspector->animator.exists)
	{
		this->animatorComponent();
	}

	// Prefab
	if (this->m_currentSpriteInInspector->prefab.exists)
	{
		this->prefabComponent();
	}

	this->componentSelector();
	this->setCompontents();
}

void s2d::UIInspector::drawRectangleOverCurrentObject()
{
	s2d::GameObject::rects[0].setOutlineColor(sf::Color(0, 0, 0));
	s2d::GameObject::rects[0].setOutlineThickness(3.5f);
	s2d::GameObject::rects[0].setSize(sf::Vector2f(this->m_currentSpriteInInspector->transform.textureSize.x, this->m_currentSpriteInInspector->transform.textureSize.y));
	s2d::GameObject::rects[0].setPosition(this->m_currentSpriteInInspector->getOrigininalPosition().x, this->m_currentSpriteInInspector->getOrigininalPosition().y);
	s2d::GameObject::rects[0].setTexture(&this->m_textureOverSprite);
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

	if (ImGui::TreeNode("Transform"))
	{
		float x = ImGui::GetCursorPosX();
		float y = ImGui::GetCursorPosY();

		ImGui::Dummy(ImVec2(0, 8));
		inputXY("Position", this->m_currentSpriteInInspector->transform.position.x, 
			this->m_currentSpriteInInspector->transform.position.y, x, y);

		y += 45;

		s2d::Vector2 temp_scale = this->m_currentSpriteInInspector->transform.getScale();

		ImGui::Dummy(ImVec2(0, 3));
		inputXY("Scale", temp_scale.x, temp_scale.y, x, y);

		this->m_currentSpriteInInspector->transform.setScale(temp_scale);

		ImGui::SetCursorPos(ImVec2(x, y + 50));
		ImGui::TreePop();
	}
}

void s2d::UIInspector::spriteRendererComponent()
{
	//Setting Sprite Renderer Component
	if (ImGui::TreeNode("Sprite Renderer"))
	{
		std::string input = s2d::UIInspector::getNamePathSplit(this->m_currentSpriteInInspector->path);
		this->m_spriteName = this->m_currentSpriteInInspector->name;

		float y = ImGui::GetCursorPos().y;
		float x = ImGui::GetCursorPos().x;

		ImGui::SetCursorPos(ImVec2(x += 15.0f, y += 10));
		ImGui::Text("Sprite");

		ImGui::SetCursorPos(ImVec2(x += 100, y - 5));
		ImGui::SetNextItemWidth(this->m_spriteInputWidth);
		ImGui::InputText("##spriteRenderer", &input[0], CHAR_MAX);

		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(0) && s2d::UIAssetFolder::dragAndDropPath != " ")
		{
			this->m_currentSpriteInInspector->setSpriteTexture(s2d::UIAssetFolder::dragAndDropPath);
		}

		ImGui::Dummy(ImVec2(0, 3.8f));

		//Sorting Layer
		ImGui::SetCursorPosX(x -= 97.7f);
		ImGui::Text("Sorting Layer");
		ImGui::SameLine();
		ImGui::PushItemWidth(42.0f);
		ImGui::InputInt("##Sorting Layer", &this->m_currentSpriteInInspector->sortingLayerIndex, 0, 0);
		ImGui::PopItemWidth();

		ImGui::Dummy(ImVec2(0, 7));
		ImGui::TreePop();
	}
}

void s2d::UIInspector::boxColliderComponent()
{
	float x = ImGui::GetCursorPosX();
	float y = ImGui::GetCursorPosY();

	ImGui::SetCursorPos(ImVec2(x += 320, y -= 4));
	if (s2d::FontManager::displaySmybolAsButton(ICON_FA_TRASH))
	{
		this->m_currentSpriteInInspector->collider.resetBoxCollider();
	}
	else
	{
		ImGui::SetCursorPos(ImVec2(x -= 320, y += 4));
		if (ImGui::TreeNode("BoxCollider"))
		{
			ImGui::Dummy(ImVec2(0, 4));
			float x = ImGui::GetCursorPos().x;
			float y = ImGui::GetCursorPos().y;

			this->m_collider.edit(x, y);
			this->m_collider.solid(x, y, this->m_currentSpriteInInspector);
			this->m_collider.width(x, y, this->m_currentSpriteInInspector);
			this->m_collider.height(this->m_currentSpriteInInspector);

			ImGui::TreePop();

			//Transparent since we open the boxcollider and we want to open the colider (rec)
			s2d::GameObject::rects[0].setOutlineColor(sf::Color(0, 0, 255, 0));
			this->m_collider.drawBoxCollider(this->m_currentSpriteInInspector);

			ImGui::Dummy(ImVec2(0, 9));
		}
	}

	//Setting props
}

void s2d::UIInspector::physicsBodyComponent()
{
	float x = ImGui::GetCursorPosX();
	float y = ImGui::GetCursorPosY();

	ImGui::SetCursorPos(ImVec2(x += 320, y -= 5));
	if (s2d::FontManager::displaySmybolAsButton(ICON_FA_TRASH "##Physicsbody"))
	{
		this->m_currentSpriteInInspector->physicsBody.resetPhysicsBody();
	}
	else
	{
		ImGui::SetCursorPos(ImVec2(x -= 320, y += 5));

		if (ImGui::TreeNode("PhysicsBody"))
		{
			ImGui::Dummy(ImVec2(0, 5));

			float x = ImGui::GetCursorPosX();
			float y = ImGui::GetCursorPosY();

			ImGui::Text("Gravity");
			ImGui::PushItemWidth(55);
			ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize - 0.2f);
			ImGui::SetCursorPos(ImVec2(x += 125, y -= 6.0f));
			ImGui::InputFloat("##gravity", &this->m_currentSpriteInInspector->physicsBody.gravity, 0, 0, "%g");
			ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);
			ImGui::PopItemWidth();

			ImGui::Dummy(ImVec2(0, 5));

			ImGui::SetCursorPosX(x - 122);
			ImGui::Text("Mass");
			ImGui::PushItemWidth(55);
			ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize - 0.2f);
			ImGui::SetCursorPos(ImVec2(x, y += 45.0f));
			ImGui::InputFloat("##mass", &this->m_currentSpriteInInspector->physicsBody.mass, 0, 0, "%g");
			ImGui::PopItemWidth();
			ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);

			ImGui::TreePop();
		}
	}
}

void s2d::UIInspector::componentSelector()
{
	ImGui::SetCursorPos(ImVec2(14, 766));
	static const char* components[] =
	{
		"BoxCollider",
		"PhysicsBody",
		"Animator",
		"Prefab"
	};

	if (ImGui::BeginCombo("##Components", "Add Components"))
	{
		for (int n = 0; n < IM_ARRAYSIZE(components); n++)
		{
			bool is_selected = (m_currentComponentSelected == components[n]);
			if (ImGui::Selectable(components[n], is_selected))
			{
				m_currentComponentSelected = components[n];
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	
	ImGui::Dummy(ImVec2(0, 7));
	ImGui::Separator();
}

void s2d::UIInspector::animatorComponent()
{
	float y = ImGui::GetCursorPos().y;
	float x = ImGui::GetCursorPos().x;
	ImGui::SetCursorPos(ImVec2(x += 320, y -= 4));
	if (s2d::FontManager::displaySmybolAsButton(ICON_FA_TRASH "##animator"))
	{
		this->m_currentSpriteInInspector->animator.resetComponent();
	}
	else
	{
		ImGui::SetCursorPos(ImVec2(x -= 320, y += 4));

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
				if (s2d::UIInfo::srenderAssetFolder)
					s2d::UIInfo::srenderAssetFolder = false;
				else
					s2d::UIInfo::srenderAssetFolder = true;
			}

			ImGui::TreePop();
		}
	}
}

void s2d::UIInspector::prefabComponent()
{
	float x = ImGui::GetCursorPosX();
	float y = ImGui::GetCursorPosY();

	ImGui::SetCursorPos(ImVec2(x += 320, y -= 5));
	if (s2d::FontManager::displaySmybolAsButton(ICON_FA_TRASH "##Prefab"))
	{
		this->m_currentSpriteInInspector->prefab.resetPrefab();
	}
	else
	{
		ImGui::SetCursorPos(ImVec2(x -= 320, y += 4));

		if (ImGui::TreeNode("Prefab"))
		{
			ImGui::Text("File location");
			ImGui::InputText("##prbl", &this->m_pathToPrefab[0], CHAR_MAX);
			ImGui::TreePop();
		}
	}
}

void s2d::UIInspector::setCompontents()
{
	if (this->m_currentComponentSelected == "BoxCollider")
	{
		this->m_currentSpriteInInspector->collider.exists = true;
		this->m_currentComponentSelected = " ";
	}
	if (this->m_currentComponentSelected == "PhysicsBody")
	{
		this->m_currentSpriteInInspector->physicsBody.exists = true;
		this->m_currentComponentSelected = " ";
	}
	if (this->m_currentComponentSelected == "Animator")
	{
		this->m_currentSpriteInInspector->animator.exists = true;
		this->m_currentComponentSelected = " ";
	}
	if (this->m_currentComponentSelected == "Prefab")
	{
		this->m_currentSpriteInInspector->prefab.exists = true;
		this->m_currentComponentSelected = " ";
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
	//Pushing transperany
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.1f);

	ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

	this->render();

	ImGui::End();

	ImGui::PopStyleVar();
}


