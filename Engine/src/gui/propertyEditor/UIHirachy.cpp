
#include "UIHirachy.h"

//Constructor

s2d::UIHirachy::UIHirachy()
{
	this->m_deleteSprite = nullptr;
	this->m_tempChildSprite = nullptr;
	this->isHovered = false;
	this->m_clickedValidSprite = false;
	this->m_isAnySpriteRightClicked = false;
	this->m_isPopUpOpen = false;
	this->m_menuName = "menu";
	this->m_width = 250.0f;
}

//Public functions

void s2d::UIHirachy::createHirachyWindow()
{
	//Pushing transperany
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.1f);

	this->m_clickedValidSprite = false;
	if (ImGui::Begin("Hirachy", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
	{
		this->render();
		this->isHovered = ImGui::IsWindowHovered() || this->m_isPopUpOpen;
		ImGui::End();
	}

	ImGui::PopStyleVar();

	if (this->m_tempChildSprite != nullptr)
	{
		ImVec2 cursor = ImGui::GetCursorPos();
		ImVec2 pos = ImVec2(sf::Mouse::getPosition().x - 100, sf::Mouse::getPosition().y + 10);

		if (ImGui::Begin("##Drag", NULL, ImGuiWindowFlags_NoTitleBar))
		{
			ImGui::SetWindowPos(pos);
			ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);

			ImGui::Text(this->m_tempChildSprite->name.c_str());
			ImGui::End();
		}
	}
}

//private functions

void s2d::UIHirachy::render()
{
	//Setting HY window size
	ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetWindowSize(ImVec2(this->m_width, 1080.0f));

	//Creating additonal windows
	this->createMenuPopup();

	//Adding sprites to hirachy
	this->addSpritesToHirachy();
}

void s2d::UIHirachy::createMenuPopup()
{
	//Checking inputs to open the pop up
	if (ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered() && !this->m_isPopUpOpen)
	{
		ImGui::OpenPopup(this->m_menuName);
	}

	ImGui::SetNextWindowSize(ImVec2(150, 200));

	//Rendering the popup
	renderMenuPopup();

	//The pop up needs to shut down when we right click again -> alr done
	if (ImGui::IsMouseClicked(1) && this->m_isPopUpOpen)
	{
		ImGui::CloseCurrentPopup();
	}
	this->m_isPopUpOpen = ImGui::IsPopupOpen(this->m_menuName);
}

void s2d::UIHirachy::renderMenuPopup()
{
	//Pop up, only the window this aint the menu
	if (ImGui::BeginPopup(this->m_menuName))
	{
		this->createButton();
		this->deleteButton();

		ImGui::EndPopup();
	}
}
void s2d::UIHirachy::deleteChildsRecursivly(s2d::Sprite* toDelete)
{
	for (s2d::Sprite* child : toDelete->childs)
	{
		//Erasing it from the vector - deletet sprites because if we delete 1 sprite our list already pops back so we need to move 1 to
		s2d::Sprite::activeSprites.erase(s2d::Sprite::activeSprites.begin() + child->getVectorPosition() - 1);

		for (s2d::Sprite* spr : s2d::Sprite::activeSprites)
		{
			//Decrement the vector position so we dont have to rememer the deletet sprites
			if (spr->getVectorPosition() > child->getVectorPosition())
				spr->setVectorPosition(spr->getVectorPosition() - 1);
		}

		if (child->childs.size() > 0)
		{
			this->deleteChildsRecursivly(child);
		}
		delete child;
	}

	//Clearing the childs since the whole parent gets deletet
	toDelete->childs.clear();
}

void s2d::UIHirachy::createButton()
{
	ImGui::SetNextWindowSize(ImVec2(100, 40));

	//Right click menu in the HY 
	if (ImGui::BeginMenu("Create"))
	{
		//Setting the new font scale since the old is to small
		ImGui::SetWindowFontScale(1.8f);

		//Creating the Menu item "sprite" to create sprites
		if (ImGui::MenuItem("Sprite"))
		{
			s2d::SpriteData::highestIndex++;

			//Adding a new sprite to the hirachy + game scene	

			int vectorPos = s2d::Sprite::activeSprites.size() + 1;
			std::string temp = "Sprite " + std::to_string(vectorPos) + " id: " + std::to_string(s2d::SpriteData::highestIndex);

			s2d::Sprite* sprite = new s2d::Sprite(temp, s2d::Vector2(0, 0), s2d::SpriteData::defaultSpritePath);
			sprite->setId(s2d::SpriteData::highestIndex);

			sprite->addSpriteToScene();

		}
		ImGui::EndMenu();
	}
	ImGui::SetCursorPos(ImVec2(0, 35));

}

void s2d::UIHirachy::deleteButton()
{
	if (ImGui::Button("Delete") && this->m_deleteSprite != nullptr)
	{
		//Erasing sprites from the list ( when deleting a child)
		s2d::Sprite* parent = s2d::Sprite::getSpriteById(this->m_deleteSprite->getParentId());
		if (parent != nullptr)
		{
			for (s2d::Sprite* child : parent->childs)
			{
				if (child->getId() == this->m_deleteSprite->getId())
				{
					//Erasing the child when deleting a child in a parent
					parent->childs.erase(parent->childs.begin() + child->getChildListPosition() - 1);
				}
			}
		}

		//Deleting the sprite(s) & chílds
		//Deleting childs from HY scene + freeing them
		this->deleteChildsRecursivly(this->m_deleteSprite);

		//deleting the deletet sprite + freeing it
		s2d::Sprite::activeSprites.erase((s2d::Sprite::activeSprites.begin() + this->m_deleteSprite->getVectorPosition() - 1));

		for (s2d::Sprite* greater : s2d::Sprite::activeSprites)
		{
			if (greater->getVectorPosition() > this->m_deleteSprite->getVectorPosition())
			{
				greater->setVectorPosition(greater->getVectorPosition() - 1);
			}
		}

		delete this->m_deleteSprite;
		this->m_deleteSprite = nullptr;
	}
}

void s2d::UIHirachy::addSpritesToHirachy()
{
	for (s2d::Sprite* sprite : s2d::Sprite::activeSprites)
	{
		if (sprite->parent != nullptr)
		{
			continue;
		}
		this->displayChildsRecursivly(sprite);

		//We clicked no item, we should display a default inspector view because we set the selected sprit to nullptr
		if (ImGui::IsMouseClicked(0) && !ImGui::IsAnyItemHovered() && ImGui::IsWindowHovered())
		{
			this->selectedSprite = nullptr;
		}

		ImGui::Spacing();
	}

	//Setting drag and drop sprite to nullptr (outide the for, fixed bug)
	if (ImGui::IsMouseReleased(0))
	{
		this->m_tempChildSprite = nullptr;
	}

	ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);
}

void s2d::UIHirachy::displayChildsRecursivly(s2d::Sprite* sprite)
{
	//Setting sprite name
	std::string s_empty = " ";
	std::string s_name = sprite->name;

	const char* name = s_name.c_str();

	if (sprite->childs.size() > 0)
	{
		// Displaying the sprite name
		s2d::FontManager::displaySmybolAsText(ICON_FA_IMAGE);
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 20, ImGui::GetCursorPosY() - 23));
		if (ImGui::TreeNode(name))
		{
			//Tree needs to be collapsed to delete the Sprite ( parent ) 
			if (ImGui::IsItemClicked(0))
			{
				m_clickedValidSprite = true;
				s2d::UIHirachy::selectedSprite = sprite;
			}
			//Setting sprit which will be deletet when we right click and dlcik button delete
			if (ImGui::IsItemClicked(1))
			{
				m_deleteSprite = sprite;
			}

			this->childSystem(sprite, true);

			for (s2d::Sprite* child : sprite->childs)
			{
				displayChildsRecursivly(child);
			}
			ImGui::TreePop();
		}
	}
	else
	{
		if (sprite->parent != nullptr)
		{
			ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 40, ImGui::GetCursorPosY()));
		}

		// Displaying the sprite name
		s2d::FontManager::displaySmybolAsText(ICON_FA_IMAGE);
		ImGui::SameLine();
		bool clicked = ImGui::MenuItem(name);

		//Setting sprit which will be deletet when we right click and dlcik button delete
		if (ImGui::IsItemClicked(1))
		{
			m_deleteSprite = sprite;
		}

		this->childSystem(sprite, false);

		if (clicked)
		{
			//Setting  the sprite to selected when we click it in the HY
			m_clickedValidSprite = true;
			s2d::UIHirachy::selectedSprite = sprite;
		}
	}
}

void s2d::UIHirachy::childSystem(s2d::Sprite* sprite, bool isHEader)
{
	//Setting child sprite (drag and drop)
	if (this->m_tempChildSprite == nullptr)
	{
		if (ImGui::IsItemClicked())
		{
			this->m_tempChildSprite = sprite;
		}
	}

	//When holding a sprite and dragging it to a parent this will be set ehre
	if (ImGui::IsMouseReleased(0) && this->m_tempChildSprite != nullptr && this->m_tempChildSprite->name != sprite->name && ImGui::IsItemHovered())
	{
		if (this->m_tempChildSprite->containsChild(sprite))
		{
			return;
		}

		for (s2d::Sprite* parent : s2d::Sprite::activeSprites)
		{
			for (s2d::Sprite* child : parent->childs)
			{
				if (child->getId() == this->m_tempChildSprite->getId())
				{
					//When dragging a new sprite as child we need to remove the child from the current sprite 
					child->resetChildData();
				}
			}
		}
		this->m_tempChildSprite->setParent(sprite);
	}
}


s2d::Sprite* s2d::UIHirachy::selectedSprite = nullptr;

