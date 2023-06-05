#include "UIHirachyMenuDisplayer.h"
#include <UIHirachySpriteDisplay.h>
#include <UIHirachy.h>


// Constructor 

s2d::UIHirachyMenuDisplayer::UIHirachyMenuDisplayer()
{
	this->m_spriteDisplayer = nullptr;
	this->isPopUpOpen = false;
	this->m_menuName = "menu";
}

s2d::UIHirachyMenuDisplayer::UIHirachyMenuDisplayer(s2d::UIHirachySpriteDisplayer* displayer)
	: m_spriteDisplayer(displayer)
{
	this->isPopUpOpen = false;
	this->m_menuName = "menu";
}

// Public functions 

void s2d::UIHirachyMenuDisplayer::createMenuPopup()
{
	//Checking inputs to open the pop up
	if (ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered() && !this->isPopUpOpen)
	{
		ImGui::OpenPopup(this->m_menuName);
	}

	//Rendering the popup
	renderMenuPopup();

	//The pop up needs to shut down when we right click again -> alr done
	if (ImGui::IsMouseClicked(1) && this->isPopUpOpen)
	{
		ImGui::CloseCurrentPopup();
	}
	this->isPopUpOpen = ImGui::IsPopupOpen(this->m_menuName);
}

// Private functions 

void s2d::UIHirachyMenuDisplayer::renderMenuPopup()
{
	//Pop up, only the window this aint the menu
	if (ImGui::BeginPopup(this->m_menuName))
	{
		this->createButton();
		this->deleteButton();

		ImGui::EndPopup();
	}
}

void s2d::UIHirachyMenuDisplayer::deleteChildsRecursivly(s2d::Sprite* toDelete)
{
	for (s2d::Sprite* child : toDelete->childs)
	{
		//Erasing it from the vector - deletet sprites because if we delete 1 sprite our list already pops back so we need to move 1 to
		s2d::Sprite::s_sprites.erase(s2d::Sprite::s_sprites.begin() + child->getVectorPosition() - 1);

		for (s2d::Sprite* spr : s2d::Sprite::s_sprites)
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

void s2d::UIHirachyMenuDisplayer::createButton()
{
	//Right click menu in the HY 
	if (ImGui::BeginMenu("Create"))
	{
		//Creating the Menu item "sprite" to create sprites
		if (ImGui::MenuItem("Sprite"))
		{
			//Adding a new sprite to the hirachy + game scene	

			int vectorPos = int(s2d::Sprite::s_sprites.size()) + 1;
			std::string name = "Sprite " + std::to_string(vectorPos) + " id: " + std::to_string(s2d::SpriteData::highestSpriteID + 1);

			s2d::Sprite* sprite = new s2d::Sprite(name, s2d::Vector2(0, 0), s2d::SpriteData::defaultSpritePath);

			// or true in the contrusctor
			sprite->addSpriteToScene();
		}
		ImGui::EndMenu();
	}
	ImGui::SetCursorPos(ImVec2(0, 35));

}

void s2d::UIHirachyMenuDisplayer::deleteButton()
{
	if (ImGui::Button("Delete") && this->m_spriteDisplayer->deleteSprite != nullptr)
	{
		//Erasing sprites from the list ( when deleting a child)
		s2d::Sprite* parent = s2d::Sprite::getSpriteById(this->m_spriteDisplayer->deleteSprite->getParentId());
		if (parent != nullptr)
		{
			for (s2d::Sprite* child : parent->childs)
			{
				if (child->getId() == this->m_spriteDisplayer->deleteSprite->getId())
				{
					//Erasing the child when deleting a child in a parent
					parent->childs.erase(parent->childs.begin() + child->getChildListPosition() - 1);
				}
			}
		}

		//Deleting the sprite(s) & chílds
		//Deleting childs from HY scene + freeing them
		this->deleteChildsRecursivly(this->m_spriteDisplayer->deleteSprite);

		//deleting the deletet sprite + freeing it
		s2d::Sprite::s_sprites.erase((s2d::Sprite::s_sprites.begin() + this->m_spriteDisplayer->deleteSprite->getVectorPosition() - 1));

		for (s2d::Sprite* greater : s2d::Sprite::s_sprites)
		{
			if (greater->getVectorPosition() > this->m_spriteDisplayer->deleteSprite->getVectorPosition())
			{
				greater->setVectorPosition(greater->getVectorPosition() - 1);
			}
		}

		delete this->m_spriteDisplayer->deleteSprite;
		this->m_spriteDisplayer->deleteSprite = nullptr;
		s2d::UIHirachy::s_selectedSprite = nullptr;
	}
}