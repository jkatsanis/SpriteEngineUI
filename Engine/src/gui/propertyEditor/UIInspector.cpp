#include "UIInspector.h"

s2d::UIInspector::UIInspector()
{
	this->m_currentSpriteInInspector = new s2d::Sprite();
	this->m_menuName = "menu";
	this->m_windowSizeWidth = 390.0f;
	s2d::GameObject::rects.push_back(m_rectangle);
	s2d::GameObject::rects.push_back(this->m_boxCollider);

	this->m_collider = s2d::UIInspectorBoxCollider();
}

//Private functions

void s2d::UIInspector::render()
{
	//Setting it here transparent because if we go down and out box collider is actually getting used it will update to green automatic
	s2d::GameObject::rects[1].setOutlineColor(sf::Color(0, 0, 255, 0));

	if (s2d::UIHirachy::selectedSprite != nullptr)
	{
		this->m_currentSpriteInInspector = s2d::UIHirachy::selectedSprite;
	}
	if (isInspectorSpriteEqualToActiveSprites() && s2d::UIHirachy::selectedSprite != nullptr)
	{
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
		//Setting the rect to transparent because we dont want to show it when we deletet a sprite/´display default insp view
		s2d::GameObject::rects[0].setOutlineColor(sf::Color(0, 0, 255, 0));

		this->displayDefaultInspectorView();
	}

	//Setting Inspector window size
	ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);
	ImGui::SetWindowPos(ImVec2(1530.0f, 0.0f));
	ImGui::SetWindowSize(ImVec2(this->m_windowSizeWidth, 1080.0f));

	this->isHovered = ImGui::IsWindowHovered();
}

void s2d::UIInspector::checkDupeName()
{
	for (s2d::Sprite* spr : s2d::Sprite::activeSprites)
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
	if (ImGui::TreeNode("Background"))
	{
		ImGui::Dummy(ImVec2(0, 16));

		ImVec2 cursorPs = ImGui::GetCursorPos();

		static ImVec4 clearColor = ImVec4(this->backgroundColor.x / 255, this->backgroundColor.y / 255, this->backgroundColor.z / 255, 1.0f);
		ImGui::ColorEdit3("Background", (float*)&clearColor);

		this->backgroundColor.x = clearColor.x * 255;
		this->backgroundColor.y = clearColor.y * 255;
		this->backgroundColor.z = clearColor.z * 255;
		ImGui::Dummy(ImVec2(0, 12));
		ImGui::TreePop();
	}
}

void s2d::UIInspector::gameEngineViewSetting()
{
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 300, ImGui::GetCursorPosY() - 3));

	if (s2d::FontManager::displaySmybolAsButton(ICON_FA_RETWEET))
	{
		s2d::GameObject::ptr_camera_tRealTimeEditor->reset();
	}
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 41 + 3);
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
		ImGui::SliderFloat("##Zoom", &s2d::GameObject::ptr_camera_tRealTimeEditor->cameraZoom, 0.1f, 10.0f, "%g");

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

	this->componentSelector();
	this->setCompontents();
}


void s2d::UIInspector::drawRectangleOverCurrentObject()
{
	s2d::GameObject::rects[0].setSize(sf::Vector2f(this->m_currentSpriteInInspector->transform.scale.x + 1.2f, this->m_currentSpriteInInspector->transform.scale.y + 1.2f));
	s2d::GameObject::rects[0].setOutlineColor(sf::Color(0, 0, 0));
	s2d::GameObject::rects[0].setOutlineThickness(3.5f);
	s2d::GameObject::rects[0].setPosition(this->m_currentSpriteInInspector->getOrigininalPosition().x, this->m_currentSpriteInInspector->getOrigininalPosition().y);

	if (m_texture.loadFromFile("EngineAssets/Sprites/transparent.png"))
	{
		s2d::GameObject::rects[0].setTexture(&m_texture);
	}
}

void s2d::UIInspector::transformComponent()
{
	//Setting UI transform
	if (ImGui::TreeNode("Transform"))
	{
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + 20));
		ImGui::SetCursorPos(ImVec2(40, 150));
		ImGui::Text("X");
		ImGui::PushItemWidth(100);
		ImGui::SetCursorPos(ImVec2(80 - 5, 145));
		ImGui::InputFloat("##x", &this->m_currentSpriteInInspector->transform.position.x, 0, 0, "%g");
		ImGui::PopItemWidth();

		ImGui::SetCursorPos(ImVec2(200, 146));
		ImGui::Text("Y");
		ImGui::PushItemWidth(100);
		ImGui::SetCursorPos(ImVec2(240 - 5, 145));
		ImGui::InputFloat("##y", &this->m_currentSpriteInInspector->transform.position.y, 0, 0, "%g");
		ImGui::Dummy(ImVec2(0, 12));
		ImGui::PopItemWidth();
		ImGui::TreePop();

	}
}

void s2d::UIInspector::spriteRendererComponent()
{
	//Setting Sprite Renderer Component
	if (ImGui::TreeNode("Sprite Renderer"))
	{
		char* input = s2d::UIInspector::getNamePathSplit(this->m_currentSpriteInInspector->path);
		this->m_spriteName = this->m_currentSpriteInInspector->name;

		float y = ImGui::GetCursorPos().y;
		float x = ImGui::GetCursorPos().x;

		ImGui::SetCursorPos(ImVec2(x += 15.0f, y += 10));
		ImGui::Text("sprite");

		// -5 Because we need our sprite renderer to be cool
		ImGui::SetCursorPos(ImVec2(x += 100, y - 5));
		ImGui::InputText("##spriteRenderer", input, CHAR_MAX);

		//std::cout << s2d::UIAssetFolder::dragAndDropPath;

		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(0) && s2d::UIAssetFolder::dragAndDropPath != " ")
		{
			std::cout << "ih";
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
	if (s2d::FontManager::displaySmybolAsButton(ICON_FA_TRASH))
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
	const char* components[] =
	{
		"BoxCollider",
		"PhysicsBody",
		"Animator"
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
	if (s2d::FontManager::displaySmybolAsButton(ICON_FA_TRASH))
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

void s2d::UIInspector::setCompontents()
{
	//Dont forget to add an option to remove a component

	if (this->m_currentComponentSelected == nullptr)
		return;
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
}

#pragma endregion

bool s2d::UIInspector::isInspectorSpriteEqualToActiveSprites()
{
	for (const Sprite* sprite : s2d::Sprite::activeSprites)
	{
		if (m_currentSpriteInInspector->name == sprite->name)
		{
			return true;
		}
	}
	return false;
}

//static functions

char* s2d::UIInspector::getNamePathSplit(std::string path)
{
	int val;
	int val2;

	std::string* pick;
	int valPick;

	std::string* splittetSring = std::splitString(path, "/", val);
	std::string* splittetSrin2 = std::splitString(path, "\\", val2);

	pick = val > val2 ? splittetSring : splittetSrin2;
	valPick = val > val2 ? val : val2;

	char* input = const_cast<char*>(pick[valPick].c_str());
	return input;
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


