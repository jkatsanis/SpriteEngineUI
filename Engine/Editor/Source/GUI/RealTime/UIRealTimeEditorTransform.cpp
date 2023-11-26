#include "UIRealTimeEditorTransform.h"

// Ctor

void spe::UIRealTimeEditorTransform::Init()
{
}

// Private

void spe::UIRealTimeEditorTransform::moveComponent()
{
	if (this->m_clickedSprite == nullptr) return;
	float x = this->m_cursorWorldPos.x - 960;
	float y = -(this->m_cursorWorldPos.y - 540);

	float m = x - this->m_clickedSprite->transform.GetPosition().x;
	float my = y - this->m_clickedSprite->transform.GetPosition().y;

	if (spe::UIUtility::GUICursor.PositionChanged)
	{
		spe::Vector2 moved = spe::UIUtility::GUICursor.LastPosition - spe::UIUtility::GUICursor.Position;
		m += moved.x * this->m_ptr_GUIRepo->Camera.getZoom();
		my -= moved.y * this->m_ptr_GUIRepo->Camera.getZoom();
	}

	spe::Vector2 newPosition = spe::Vector2(x - m, y - my);

	this->m_clickedSprite->transform.SetPosition(newPosition);


}

bool spe::UIRealTimeEditorTransform::checkClick(spe::Sprite* const sprite)
{
	bool collied = spe::UIUtility::isCursorClickedOnSprite(sprite);

	if (collied && this->m_ptr_Window->Event.type == spe::Event::Type::MousePressedLeft)
	{
		this->m_clickedSpriteId = sprite->getId();

		this->m_realeasedCursorOnSprite = true;
		return true;
	}
	if (this->m_ptr_Window->Event.type == spe::Event::MouseReleasedLeft)
	{
		this->m_realeasedCursorOnSprite = false;
		return false;
	}

	return false;
}

spe::Sprite* spe::UIRealTimeEditorTransform::checkIfMouseClickedOnSprite()
{
	uint32_t highest = 0;
	std::string name = "";
	std::vector<spe::Sprite*> spr;


	std::list<spe::Sprite*>& sprites = this->m_ptr_Repo->GetSprites();


	for (auto it = sprites.begin(); it != sprites.end(); ++it)
	{
		spe::Sprite* sprite = *it;

		// Checking if we have the same ID. If we wouldnt do that check it could just return
		// The next sprite in the list which would be wrong
		if (this->m_realeasedCursorOnSprite && sf::Mouse::isButtonPressed(sf::Mouse::Left)
			&& sprite->getId() == this->m_clickedSpriteId)
		{
			return sprite;
		}

		if (checkClick(sprite))
		{
			spr.push_back(sprite);
		}
	}

	this->m_ptr_Window->Event.type = spe::Event::None;

	for (spe::Sprite* sp : spr)
	{
		if (sp->sprite_renderer.sorting_layer_index >= highest)
		{
			name = sp->name;
			highest = sp->sprite_renderer.sorting_layer_index;
		}
	}

	if (highest != -1)
	{
		if (name == "")
		{
			return nullptr;
		}
		this->m_clickedSprite = this->m_ptr_Repo->GetByName(name);
		this->m_ptr_GUIRepo->sprite_in_inspector = this->m_clickedSprite;

		orgn = this->m_clickedSprite->getSprite().getOrigin();

		return this->m_clickedSprite;
	}

	return nullptr;
}

// Public

void spe::UIRealTimeEditorTransform::Render()
{
	// Transform
	if (!spe::UIUtility::s_IsAnyHovered)
	{
		// Check if we click on a sprite in the editor
		this->m_clickedSprite = this->checkIfMouseClickedOnSprite();

		this->m_cursorWorldPos = spe::UIUtility::getWorldCordinates();

		if (this->m_clickedSprite != nullptr)
		{
			this->m_clickedSprite = this->m_ptr_GUIRepo->sprite_in_inspector;
			this->moveComponent();
		}
		else
		{
			this->m_clickedSprite = nullptr;
			this->m_realeasedCursorOnSprite = false;
		}
	}
	else
	{
		this->m_clickedSprite = nullptr;
		this->m_realeasedCursorOnSprite = false;
	}
}
