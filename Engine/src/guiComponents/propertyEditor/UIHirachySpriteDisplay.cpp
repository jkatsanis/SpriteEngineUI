#include "UIHirachySpriteDisplay.h"
#include <UIHirachy.h>

// Constructor

s2d::UIHirachySpriteDisplayer::UIHirachySpriteDisplayer()
{
	this->m_spriteSelectedColor = ImVec4(139.0f / 255.0f,
		180.0f / 255.0f,
		234.0f / 255.0f,
		1.0f);
	this->childSelectedToParent = nullptr;
	this->deleteSprite = nullptr;
}

// Public method

void s2d::UIHirachySpriteDisplayer::addSpritesToHirachy()
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
			s2d::UIHirachy::selectedSprite = nullptr;
		}

		ImGui::Spacing();
	}

	//Setting drag and drop sprite to nullptr (outide the for, fixed bug)
	if (ImGui::IsMouseReleased(0))
	{
		this->childSelectedToParent = nullptr;
	}

	ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);
}

void s2d::UIHirachySpriteDisplayer::displayWindowWhenChildIsGettingDragged()
{
	if (this->childSelectedToParent != nullptr)
	{
		ImVec2 cursor = ImGui::GetCursorPos();
		ImVec2 pos = ImVec2(float(sf::Mouse::getPosition().x) - 100.0f, float(sf::Mouse::getPosition().y) + 10.0f);

		if (ImGui::Begin("##Drag", NULL, ImGuiWindowFlags_NoTitleBar))
		{
			ImGui::SetWindowPos(pos);
			ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);


			ImGui::Text(this->childSelectedToParent->name.c_str());
			ImGui::End();
		}
	}
}

// Private methods

void s2d::UIHirachySpriteDisplayer::childSystem(s2d::Sprite* sprite, bool isHEader)
{
	//Setting child sprite (drag and drop)
	if (this->childSelectedToParent == nullptr)
	{
		if (ImGui::IsItemClicked())
		{
			this->childSelectedToParent = sprite;
		}
	}

	//When holding a sprite and dragging it to a parent this will be set ehre
	if (ImGui::IsMouseReleased(0) && this->childSelectedToParent != nullptr && this->childSelectedToParent->name != sprite->name && ImGui::IsItemHovered())
	{
		if (this->childSelectedToParent->containsChild(sprite))
		{
			return;
		}

		for (s2d::Sprite* parent : s2d::Sprite::activeSprites)
		{
			for (s2d::Sprite* child : parent->childs)
			{
				if (child->getId() == this->childSelectedToParent->getId())
				{
					//When dragging a new sprite as child we need to remove the child from the current sprite 
					child->resetChildData();
				}
			}
		}
		this->childSelectedToParent->setParent(sprite);
	}
}


void s2d::UIHirachySpriteDisplayer::displayChildsRecursivly(s2d::Sprite* sprite)
{
	//Setting sprite name
	std::string s_empty = " ";

	const char* name = sprite->name.c_str();
	bool popStyle = false;

	if (sprite->childs.size() > 0)
	{
		this->displayTreeNode(sprite, popStyle);
	}
	else
	{
		this->displayMenuItem(sprite, popStyle);
	}
}


void s2d::UIHirachySpriteDisplayer::displayTreeNode(s2d::Sprite* sprite, bool& popStyle)
{
	const char* name = sprite->name.c_str();

	if (s2d::UIHirachy::selectedSprite != nullptr && s2d::UIHirachy::selectedSprite->getId() == sprite->getId())
	{
		popStyle = true;
		ImGui::PushStyleColor(ImGuiCol_Text, this->m_spriteSelectedColor);
	}

	// Iterating recursivly over the sprites, if we select 1 which gets not displayed in the 
	// Hirachy this will open the trees for it
	if (s2d::UIHirachy::selectedSprite != nullptr)
	{
		if (sprite->containsChild(s2d::UIHirachy::selectedSprite))
		{
			ImGui::SetNextItemOpen(true);
		}

	}

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 10);
	// Displaying the name
	if (ImGui::TreeNode(name))
	{
		if (popStyle)
		{
			popStyle = false;
			ImGui::PopStyleColor();
		}
		//Tree needs to be collapsed to delete the Sprite ( parent ) 
		if (ImGui::IsItemClicked(0))
		{
			s2d::UIHirachy::selectedSprite = sprite;
		}
		//Setting sprit which will be deletet when we right click and dlcik button delete
		if (ImGui::IsItemClicked(1))
		{
			deleteSprite = sprite;
		}

		this->childSystem(sprite, false);

		for (s2d::Sprite* child : sprite->childs)
		{
			displayChildsRecursivly(child);
		}
		ImGui::TreePop();
	}
	if (popStyle)
	{
		popStyle = false;
		ImGui::PopStyleColor();
	}
}

void s2d::UIHirachySpriteDisplayer::displayMenuItem(s2d::Sprite* sprite, bool popStyle)
{
	const char* name = sprite->name.c_str();

	if (sprite->parent != nullptr)
	{
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 40, ImGui::GetCursorPosY()));
	}

	if (s2d::UIHirachy::selectedSprite != nullptr && s2d::UIHirachy::selectedSprite->getId() == sprite->getId())
	{
		popStyle = true;
		ImGui::PushStyleColor(ImGuiCol_Text, this->m_spriteSelectedColor);
	}

	// Displaying the sprite name
	bool clicked = ImGui::MenuItem(name);
	if (popStyle)
	{
		ImGui::PopStyleColor();
	}

	//Setting sprit which will be deletet when we right click and dlcik button delete
	if (ImGui::IsItemClicked(1))
	{
		deleteSprite = sprite;
	}

	this->childSystem(sprite, false);

	if (clicked)
	{
		//Setting  the sprite to selected when we click it in the HY
		s2d::UIHirachy::selectedSprite = sprite;
	}
}