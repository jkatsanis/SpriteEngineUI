#include "UIRealTimeEditorTransform.h"

// Ctor

void spe::UIRealTimeEditorTransform::Init()
{
	for (size_t i = 0; i < SCALE_DOTTS; i++)
	{
		this->m_scale_dotts[i] = spe::ScaleDott();
	}

	const sf::Vector2f size = sf::Vector2f(DEFAULT_DOTT_SCALE, DEFAULT_DOTT_SCALE);

	for (int i = 0; i < SCALE_DOTTS; i++)
	{
		const std::string name = "scale-dott-scale " + std::to_string(i);
		spe::Rectangle* rec = new spe::Rectangle(sf::Vector2f(0, 0),
			size, sf::Color(255, 255, 255), 2.0f, PATH_TO_TRANSPARENT_PIC, name);
		this->m_ptr_GUIRepo->Add(rec);
		this->m_scale_dotts[i].ptr_scaling_rectangle = this->m_ptr_GUIRepo->GetByName(name);
		this->m_scale_dotts[i].clicked = false;
	}

	// this->unrenderDolls();
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

		return this->m_clickedSprite;
	}

	return nullptr;
}

#pragma region SCALE

void spe::UIRealTimeEditorTransform::renderDolls()
{
	for (int i = 0; i < SCALE_DOTTS; i++)
	{
		this->m_scale_dotts[i].ptr_scaling_rectangle->Render = true;
	}
}

void spe::UIRealTimeEditorTransform::unrenderDolls()
{
	for (int i = 0; i < SCALE_DOTTS; i++)
	{
		this->m_scale_dotts[i].ptr_scaling_rectangle->Render = false;
	}
}

void spe::UIRealTimeEditorTransform::scaleChanger(spe::Sprite* focusedSprite)
{
	float scale_x = spe::UIUtility::xScaleChanger(this->m_scale_dotts[0], focusedSprite->transform.getDefaultTextureSize().x,
		focusedSprite->transform.GetPosition().x);
	if (scale_x != INVALID_SCALE)
		focusedSprite->transform.setScale(spe::Vector2(scale_x, focusedSprite->transform.getScale().y));

	float scale_y = spe::UIUtility::yScaleChanger(this->m_scale_dotts[1], focusedSprite->transform.getDefaultTextureSize().y,
		focusedSprite->transform.GetPosition().y);
	if (scale_y != INVALID_SCALE)
		focusedSprite->transform.setScale(spe::Vector2(focusedSprite->transform.getScale().x, scale_y));
}


void spe::UIRealTimeEditorTransform::reset()
{
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		this->m_ptr_Window->Event.type = spe::Event::None;
		for (int i = 0; i < SCALE_DOTTS; i++)
		{
			this->m_scale_dotts[i].clicked = false;
		}
	}
}

void spe::UIRealTimeEditorTransform::getPos(const spe::Sprite* focusedSprite, sf::Vector2f pos[])
{
	spe::Vector2 originalPos = focusedSprite->transform.getOrigininalPosition();
	spe::Vector2 textureSize = focusedSprite->transform.texture_size;

	if (focusedSprite->transform.getScale().x < 0)
	{
		pos[0] = sf::Vector2f(originalPos.x, originalPos.y + textureSize.y / 2);
	}
	else
	{
		pos[0] = sf::Vector2f(originalPos.x + textureSize.x, originalPos.y + textureSize.y / 2);
	}
	if (focusedSprite->transform.getScale().y < 0)
	{
		textureSize.y = 0;
	}
	pos[1] = sf::Vector2f(originalPos.x + textureSize.x / 2 - DEFAULT_DOTT_SCALE / 2, originalPos.y + textureSize.y);
}

void spe::UIRealTimeEditorTransform::setPos(const sf::Vector2f pos[])
{
	for (int i = 0; i < SCALE_DOTTS; i++)
	{
		if (!this->m_scale_dotts[i].clicked)
		{
			this->m_scale_dotts[i].ptr_scaling_rectangle->Shape.setPosition(pos[i]);
		}
	}
}

#pragma endregion

// Public

void spe::UIRealTimeEditorTransform::Render()
{
	if (this->m_ptr_GUIRepo->Tools == spe::EditorTools::ScaleTool)
	{
		this->renderDolls();
		// Scale
		spe::Sprite* focusedSprite = this->m_ptr_GUIRepo->sprite_in_inspector;
		if (focusedSprite != nullptr)
		{
			sf::Vector2f pos[SCALE_DOTTS];
			this->getPos(focusedSprite, pos);
			this->setPos(pos);
			this->scaleChanger(focusedSprite);
			this->reset();

			this->m_currentCursorPos = spe::UIUtility::getWorldCordinates();
		}
	}
	else
	{
		this->unrenderDolls();
	}

	if (this->m_ptr_GUIRepo->Tools == spe::EditorTools::PositionTool)
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
}
