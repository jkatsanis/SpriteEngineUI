#include "UIInspectorBoxCollider.h"

// Constructor / Destructor

spe::UIInspectorBoxCollider::UIInspectorBoxCollider()
{	
	for (size_t i = 0; i < SCALE_DOTTS_COLLIDER; i++)
	{
		this->m_ScaleDotts[i] = spe::ScaleDott();
	}
	this->m_EditMode = false;
}

// Private functions

void spe::UIInspectorBoxCollider::Reset()
{
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		for (int i = 0; i < SCALE_DOTTS_COLLIDER; i++)
		{
			this->m_ScaleDotts[i].Clicked = false;
		}
	}
}

void spe::UIInspectorBoxCollider::RenderScaleDotts(spe::Sprite* sprite)
{
	const float scale_x = (sprite->Transform.GetScale().X < 0)
		? sprite->Transform.GetScale().X * -1
		: sprite->Transform.GetScale().X;

	const float scale_y = (sprite->Transform.GetScale().Y < 0)
		? sprite->Transform.GetScale().Y * -1
		: sprite->Transform.GetScale().Y;


	const float left = sprite->Transform.GetOrigininalPosition().X;
	const float right = sprite->Transform.GetOrigininalPosition().X + sprite->Transform.GetDefaultTextureSize().X * scale_x;
	const float middle_y = (sprite->Transform.GetOrigininalPosition().Y + (sprite->Transform.GetDefaultTextureSize().Y * scale_y) / 2) ;
	const float bottom = sprite->Transform.GetOrigininalPosition().Y + sprite->Transform.GetDefaultTextureSize().Y * scale_y;
	const float middle_x = sprite->Transform.GetOrigininalPosition().X + (sprite->Transform.GetDefaultTextureSize().X * scale_x) / 2;
	const float top = sprite->Transform.GetOrigininalPosition().Y;

	const sf::Vector2f pos_width_y = sf::Vector2f(right + sprite->Collider.Width.Y, middle_y);
	const sf::Vector2f pos_width_x = sf::Vector2f(left - DEFAULT_DOTT_SCALE + sprite->Collider.Width.X, middle_y);
		
	const sf::Vector2f pos_height_x = sf::Vector2f(middle_x - DEFAULT_DOTT_SCALE, top + sprite->Collider.Height.X - DEFAULT_DOTT_SCALE);
	const sf::Vector2f pos_height_y = sf::Vector2f(middle_x - DEFAULT_DOTT_SCALE, top + sprite->Collider.Height.Y + sprite->Transform.GetDefaultTextureSize().Y * scale_y);

	this->m_ScaleDotts[0].ptr_ScalingRec->Shape.setPosition(pos_width_y);
	this->m_ScaleDotts[1].ptr_ScalingRec->Shape.setPosition(pos_width_x);
	this->m_ScaleDotts[2].ptr_ScalingRec->Shape.setPosition(pos_height_x);
	this->m_ScaleDotts[3].ptr_ScalingRec->Shape.setPosition(pos_height_y);

	const sf::Vector2f temp_scale_dott_pos_zero = this->m_ScaleDotts[0].ptr_ScalingRec->Shape.getPosition();
	const sf::Vector2f temp_scale_dott_pos_one = this->m_ScaleDotts[1].ptr_ScalingRec->Shape.getPosition();
	const sf::Vector2f temp_scale_dott_pos_two = this->m_ScaleDotts[2].ptr_ScalingRec->Shape.getPosition();
	const sf::Vector2f temp_scale_dott_pos_three = this->m_ScaleDotts[3].ptr_ScalingRec->Shape.getPosition();

	const float new_scale_width_y = spe::UIUtility::xScaleChanger(this->m_ScaleDotts[0], sprite->Transform.GetDefaultTextureSize().X,0);
	const float new_scale_width_x = spe::UIUtility::xScaleChanger(this->m_ScaleDotts[1], sprite->Transform.GetDefaultTextureSize().X, 0);

	const float new_scale_height_x = spe::UIUtility::yScaleChanger(this->m_ScaleDotts[2], sprite->Transform.GetDefaultTextureSize().X, 0);
	const float new_scale_height_y = spe::UIUtility::yScaleChanger(this->m_ScaleDotts[3], sprite->Transform.GetDefaultTextureSize().X, 0);


	bool must_return = false;

	// Resetting right rectangle
	if (this->m_ScaleDotts[0].ptr_ScalingRec->Shape.getPosition().x
		< DEFAULT_DOTT_SCALE + INVALID_AERA + this->m_ScaleDotts[1].ptr_ScalingRec->Shape.getPosition().x)
	{
		this->m_ScaleDotts[0].ptr_ScalingRec->Shape.
			setPosition(temp_scale_dott_pos_zero);
		must_return = true;
	}

	// Resetting left rectangle
	if (this->m_ScaleDotts[1].ptr_ScalingRec->Shape.getPosition().x + DEFAULT_DOTT_SCALE
		>  INVALID_AERA + this->m_ScaleDotts[0].ptr_ScalingRec->Shape.getPosition().x)
	{
		this->m_ScaleDotts[1].ptr_ScalingRec->Shape.
			setPosition(temp_scale_dott_pos_one);
		must_return = true;
	}

	// Resetting top rectangle
	if (this->m_ScaleDotts[2].ptr_ScalingRec->Shape.getPosition().y + DEFAULT_DOTT_SCALE
	> INVALID_AERA + this->m_ScaleDotts[3].ptr_ScalingRec->Shape.getPosition().y)
	{
		this->m_ScaleDotts[2].ptr_ScalingRec->Shape.
			setPosition(temp_scale_dott_pos_two);
		must_return = true;
	}

	// Resetting bottom rectangle
	if (this->m_ScaleDotts[3].ptr_ScalingRec->Shape.getPosition().y
	< INVALID_AERA + DEFAULT_DOTT_SCALE + this->m_ScaleDotts[2].ptr_ScalingRec->Shape.getPosition().y)
	{
		this->m_ScaleDotts[3].ptr_ScalingRec->Shape.
			setPosition(temp_scale_dott_pos_three);
		must_return = true;
	}

	if (must_return)
	{
		return;
	}

	// Width
	if (new_scale_width_y != INVALID_SCALE)
	{
		const float scale_dott_pos_x = this->m_ScaleDotts[0].ptr_ScalingRec->Shape.getPosition().x; 
		const float width = scale_dott_pos_x - right;
		sprite->Collider.Width.Y = width;
	}
	if (new_scale_width_x != INVALID_SCALE)
	{
		const float scale_dott_pos_x = this->m_ScaleDotts[1].ptr_ScalingRec->Shape.getPosition().x;
		const float width = scale_dott_pos_x - left + DEFAULT_DOTT_SCALE;

		sprite->Collider.Width.X = width;
	}

	// Height
	if (new_scale_height_x != INVALID_SCALE) 
	{
		const float scale_dott_pos_y = this->m_ScaleDotts[2].ptr_ScalingRec->Shape.getPosition().y;
		const float width = scale_dott_pos_y - top + DEFAULT_DOTT_SCALE;
		sprite->Collider.Height.X = width;
	}
	if (new_scale_height_y != INVALID_SCALE)
	{
		const float scale_dott_pos_y = this->m_ScaleDotts[3].ptr_ScalingRec->Shape.getPosition().y;
		const float width = scale_dott_pos_y - bottom;
		sprite->Collider.Height.Y = width;
	}

	this->Reset();
}

// Public functions

void spe::UIInspectorBoxCollider::LeaveEditMode()
{
	this->UnrenderDotts();
	this->m_EditMode = false;
}

void spe::UIInspectorBoxCollider::Edit(float& x, float& y)
{
	ImGui::SetCursorPosX(x += 10);
	ImGui::Text("Edit");
	ImGui::SetCursorPos(ImVec2(x += 53.5f, y -= 2.5f));
	if (spe::Style::DisplaySmybolAsButton(ICON_FA_EDIT))
	{
		this->m_EditMode = !this->m_EditMode;
	}
	x += 19;

	x -= 12.5f;
}

void spe::UIInspectorBoxCollider::Solid(float& x, float& y, spe::Sprite* sprite)
{
	ImGui::SetCursorPosX(x -= 60.4f);
	ImGui::Text("Solid");
	ImGui::SameLine();
	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
	ImGui::Checkbox("##Solid", &sprite->Collider.IsSolid);
	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
	ImGui::Dummy(ImVec2(0, 5));

	y += 40;
}

void spe::UIInspectorBoxCollider::Width(float x, float y, spe::Sprite* sprite)
{
	ImGui::SetCursorPosX(x -= 11);
	if (ImGui::TreeNode("Width"))
	{
		ImGui::SetCursorPos(ImVec2(x += 30, y += 80));
		ImGui::Text("L");

		ImGui::SetCursorPos(ImVec2(x += 30, y));
		ImGui::SliderFloat("##WidthL", &sprite->Collider.Width.X, -500, 500);
		ImGui::SameLine();

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 7);
		if (spe::Style::DisplaySmybolAsButton(ICON_FA_RETWEET, spe::Style::s_DefaultFontSize + 0.02f))
		{
			sprite->Collider.Width.X = 0;
		}

		ImGui::SetCursorPos(ImVec2(x -= 28.4f, y += 40));
		ImGui::Text("R");

		ImGui::SetCursorPos(ImVec2(x += 29, y));
		ImGui::SliderFloat("##WidthR", &sprite->Collider.Width.Y, -500, 500);
		ImGui::SameLine();

		// If we wouldn use the ## the name would still be just retweet. But now it has a different 
		// name and it acts like a different button, ImGui does not render '##'

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 7);
		if (spe::Style::DisplaySmybolAsButton(ICON_FA_RETWEET "##", spe::Style::s_DefaultFontSize + 0.02f))
		{
			sprite->Collider.Width.Y = 0;
		}
		ImGui::TreePop();
	}
}

void spe::UIInspectorBoxCollider::Height(spe::Sprite* sprite)
{
	float x = ImGui::GetCursorPosX();
	float y = ImGui::GetCursorPosY();
	ImGui::SetCursorPosX(x -= 1.5f);

	if (ImGui::TreeNodeEx("Height"))
	{
		ImGui::SetCursorPos(ImVec2(x += 33.5f, y += 35));
		ImGui::Text("U");

		ImGui::SetCursorPos(ImVec2(x += 30, y));
		ImGui::SliderFloat("##up", &sprite->Collider.Height.X, -500, 500);
		ImGui::SameLine();
		if (spe::Style::DisplaySmybolAsButton(ICON_FA_RETWEET))
		{
			sprite->Collider.Height.X = 0;
		}

		ImGui::SetCursorPos(ImVec2(x -= 28.4f, y += 40));
		ImGui::Text("D");

		ImGui::SetCursorPos(ImVec2(x += 28, y));
		ImGui::SliderFloat("##down", &sprite->Collider.Height.Y, -500, 500);
		ImGui::SameLine();
		if (spe::Style::DisplaySmybolAsButton(ICON_FA_RETWEET "##"))
		{
			sprite->Collider.Height.Y = 0;
		}
		ImGui::TreePop();
	}
}

void spe::UIInspectorBoxCollider::DrawBoxCollider(spe::Sprite* sprite, spe::Rectangle* ptr_rectangle)
{
	const sf::Vector2f size = sf::Vector2f(sprite->Transform.TextureSize.X + (-sprite->Collider.Width.X + sprite->Collider.Width.Y),
		sprite->Transform.TextureSize.Y + (-sprite->Collider.Height.X + sprite->Collider.Height.Y));

	sf::RectangleShape* ptr_shape = &ptr_rectangle->Shape;

	ptr_shape->setSize(size);
	ptr_shape->setPosition(sf::Vector2f(sprite->Transform.GetOrigininalPosition().X + sprite->Collider.Width.X, sprite->Transform.GetOrigininalPosition().Y + sprite->Collider.Height.X));

	ptr_rectangle->Render = true;

	if (this->m_EditMode)
	{
		this->RenderDotts();
		this->RenderScaleDotts(sprite);
	}
	else
	{
		this->UnrenderDotts();
	}
}

void spe::UIInspectorBoxCollider::InitScaleDottsUI(spe::GUIRepository& repo)
{
	const sf::Vector2f size = sf::Vector2f(DEFAULT_DOTT_SCALE, DEFAULT_DOTT_SCALE);

	for (int i = 0; i < SCALE_DOTTS_COLLIDER; i++)
	{
		const std::string name = "scale-dott-box-collider " + std::to_string(i);

		spe::Rectangle* rec = new spe::Rectangle(sf::Vector2f(0, 0),
			size, sf::Color(255, 0, 0), 2.0f, PATH_TO_TRANSPARENT_PIC, name);

		repo.Add(rec);
		this->m_ScaleDotts[i].ptr_ScalingRec = repo.GetByName(name);
		this->m_ScaleDotts[i].Clicked = false;
	}

	this->UnrenderDotts();
}

void spe::UIInspectorBoxCollider::RenderDotts()
{
	for (int i = 0; i < SCALE_DOTTS_COLLIDER; i++)
	{
		this->m_ScaleDotts[i].ptr_ScalingRec->Render = true;
	}
}

void spe::UIInspectorBoxCollider::UnrenderDotts()
{
	for (int i = 0; i < SCALE_DOTTS_COLLIDER; i++)
	{
		this->m_ScaleDotts[i].Clicked = false;
		this->m_ScaleDotts[i].ptr_ScalingRec->Render = false;
	}
}

