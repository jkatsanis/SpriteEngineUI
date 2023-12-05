#include "UIRealTimeEditorTransform.h"

// Ctor

void spe::UIRealTimeEditorTransform::Init()
{
	for (size_t i = 0; i < SCALE_DOTTS; i++)
	{
		this->m_ScaleDotts[i] = spe::ScaleDott();
	}

	const sf::Vector2f size = sf::Vector2f(DEFAULT_DOTT_SCALE, DEFAULT_DOTT_SCALE);

	for (int i = 0; i < SCALE_DOTTS; i++)
	{
		const std::string name = "scale-dott-scale " + std::to_string(i);
		spe::Rectangle* rec = new spe::Rectangle(sf::Vector2f(0, 0),
			size, sf::Color(255, 255, 255), 2.0f, PATH_TO_TRANSPARENT_PIC, name);
		this->m_ptr_GUIRepo->Add(rec);
		this->m_ScaleDotts[i].ptr_ScalingRec = this->m_ptr_GUIRepo->GetByName(name);
		this->m_ScaleDotts[i].Clicked = false;
	}

	// this->unrenderDolls();
}

// Private

void spe::UIRealTimeEditorTransform::MoveComponent()
{
	if (this->m_ptr_ClickedSprite == nullptr) return;
	float x = this->m_CursorWorldPos.X - 960;
	float y = -(this->m_CursorWorldPos.Y - 540);

	float m = x - this->m_ptr_ClickedSprite->Transform.GetPosition().X;
	float my = y - this->m_ptr_ClickedSprite->Transform.GetPosition().Y;

	if (spe::UIUtility::GUICursor.PositionChanged)
	{
		spe::Vector2 moved = spe::UIUtility::GUICursor.LastPosition - spe::UIUtility::GUICursor.Position;
		m += moved.X * this->m_ptr_GUIRepo->Camera.GetZoom();
		my -= moved.Y * this->m_ptr_GUIRepo->Camera.GetZoom();
	}

	spe::Vector2 newPosition = spe::Vector2(x - m, y - my);

	this->m_ptr_ClickedSprite->Transform.SetPosition(newPosition);
}

bool spe::UIRealTimeEditorTransform::CheckClick(spe::Sprite* const sprite)
{
	bool collied = spe::UIUtility::IsCursorClickedOnSprite(sprite);

	if (collied && this->m_ptr_Window->Event.Type == spe::Event::Type::MousePressedLeft)
	{
		this->m_ClickedSpriteId = sprite->GetId();

		this->m_RealeasedCursorOnSprite = true;
		return true;
	}
	if (this->m_ptr_Window->Event.Type == spe::Event::MouseReleasedLeft)
	{
		this->m_RealeasedCursorOnSprite = false;
		return false;
	}

	return false;
}

spe::Sprite* spe::UIRealTimeEditorTransform::CheckIfMouseClickedOnSprite()
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
		if (this->m_RealeasedCursorOnSprite && sf::Mouse::isButtonPressed(sf::Mouse::Left)
			&& sprite->GetId() == this->m_ClickedSpriteId)
		{
			return sprite;
		}

		if (CheckClick(sprite))
		{
			spr.push_back(sprite);
		}
	}

	for (spe::Sprite* sp : spr)
	{
		if (sp->SpriteRenderer.SortinLayerIdx >= highest)
		{
			name = sp->Name;
			highest = sp->SpriteRenderer.SortinLayerIdx;
		}
	}

	if (highest != -1)
	{
		if (name == "")
		{
			return nullptr;
		}
		this->m_ptr_ClickedSprite = this->m_ptr_Repo->GetByName(name);
		this->m_ptr_GUIRepo->InspectorSprite = this->m_ptr_ClickedSprite;

		return this->m_ptr_ClickedSprite;
	}

	return nullptr;
}

#pragma region SCALE

void spe::UIRealTimeEditorTransform::RenderDolls()
{
	for (int i = 0; i < SCALE_DOTTS; i++)
	{
		this->m_ScaleDotts[i].ptr_ScalingRec->Render = true;
	}
}

void spe::UIRealTimeEditorTransform::RnrenderDolls()
{
	for (int i = 0; i < SCALE_DOTTS; i++)
	{
		this->m_ScaleDotts[i].ptr_ScalingRec->Render = false;
	}
}

void spe::UIRealTimeEditorTransform::ScaleChanger(spe::Sprite* focusedSprite)
{
	float scale_x = spe::UIUtility::xScaleChanger(this->m_ScaleDotts[0], focusedSprite->Transform.GetDefaultTextureSize().X,
		focusedSprite->Transform.GetPosition().X);
	if (scale_x != INVALID_SCALE)
		focusedSprite->Transform.SetScale(spe::Vector2(scale_x, focusedSprite->Transform.GetScale().Y));

	float scale_y = spe::UIUtility::yScaleChanger(this->m_ScaleDotts[1], focusedSprite->Transform.GetDefaultTextureSize().Y,
		focusedSprite->Transform.GetPosition().Y);
	if (scale_y != INVALID_SCALE)
		focusedSprite->Transform.SetScale(spe::Vector2(focusedSprite->Transform.GetScale().X, scale_y));
}


void spe::UIRealTimeEditorTransform::Reset()
{
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		this->m_ptr_Window->Event.Type = spe::Event::None;
		for (int i = 0; i < SCALE_DOTTS; i++)
		{
			this->m_ScaleDotts[i].Clicked = false;
		}
	}
}

void spe::UIRealTimeEditorTransform::GetPos(const spe::Sprite* focusedSprite, sf::Vector2f pos[])
{
	spe::Vector2 originalPos = focusedSprite->Transform.GetOrigininalPosition();
	spe::Vector2 textureSize = focusedSprite->Transform.TextureSize;

	if (focusedSprite->Transform.GetScale().X < 0)
	{
		pos[0] = sf::Vector2f(originalPos.X, originalPos.Y + textureSize.Y / 2);
	}
	else
	{
		pos[0] = sf::Vector2f(originalPos.X + textureSize.X, originalPos.Y + textureSize.Y / 2);
	}
	if (focusedSprite->Transform.GetScale().Y < 0)
	{
		textureSize.Y = 0;
	}
	pos[1] = sf::Vector2f(originalPos.X + textureSize.X / 2 - DEFAULT_DOTT_SCALE / 2, originalPos.Y + textureSize.Y);
}

void spe::UIRealTimeEditorTransform::SetPos(const sf::Vector2f pos[])
{
	for (int i = 0; i < SCALE_DOTTS; i++)
	{
		if (!this->m_ScaleDotts[i].Clicked)
		{
			this->m_ScaleDotts[i].ptr_ScalingRec->Shape.setPosition(pos[i]);
		}
	}
}

#pragma endregion

// Public

void spe::UIRealTimeEditorTransform::Render()
{
	if (this->m_ptr_GUIRepo->Tools == spe::EditorTools::ScaleTool)
	{
		this->RenderDolls();
		// Scale
		spe::Sprite* focusedSprite = this->m_ptr_GUIRepo->InspectorSprite;
		if (focusedSprite != nullptr)
		{
			sf::Vector2f pos[SCALE_DOTTS];
			this->GetPos(focusedSprite, pos);
			this->SetPos(pos);
			this->ScaleChanger(focusedSprite);
			this->Reset();

			this->m_CurrentCursorPos = spe::UIUtility::GetWorldCordinates();
		}
	}
	else 
	{
		this->RnrenderDolls();
	}
	if (this->m_ptr_GUIRepo->InspectorSprite == nullptr)
	{
		this->RnrenderDolls();
	}

	if (this->m_ptr_GUIRepo->Tools == spe::EditorTools::PositionTool)
	{
		// Transform
		if (!spe::UIUtility::s_IsAnyHovered)
		{
			// Check if we click on a sprite in the editor
			this->m_ptr_ClickedSprite = this->CheckIfMouseClickedOnSprite();

			this->m_CursorWorldPos = spe::UIUtility::GetWorldCordinates();

			if (this->m_ptr_ClickedSprite != nullptr)
			{
				this->m_ptr_ClickedSprite = this->m_ptr_GUIRepo->InspectorSprite;
				this->MoveComponent();
			}
			else
			{
				this->m_ptr_ClickedSprite = nullptr;
				this->m_RealeasedCursorOnSprite = false;
			}
		}
		else
		{
			this->m_ptr_ClickedSprite = nullptr;
			this->m_RealeasedCursorOnSprite = false;
		}
	}
}
