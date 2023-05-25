#include "UIInspectorBoxCollider.h"

// Constructor / Destructor

s2d::UIInspectorBoxCollider::UIInspectorBoxCollider()
{	
	for (size_t i = 0; i < SCALE_DOTTS_COLLIDER; i++)
	{
		this->m_box_collider_scale_dotts[i] = s2d::ScaleDott();
	}
	this->m_ptr_event = nullptr;
	this->m_edit_mode = false;
}

// Private functions

void s2d::UIInspectorBoxCollider::reset()
{
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		this->m_ptr_event->type = s2d::Event::None;
		for (int i = 0; i < SCALE_DOTTS_COLLIDER; i++)
		{
			this->m_box_collider_scale_dotts[i].clicked = false;
		}
	}
}

void s2d::UIInspectorBoxCollider::renderScaleDotts(s2d::Sprite* sprite, s2d::Rectangle* ptr_box_collider_rec)
{
	const float scale_x = (sprite->transform.getScale().x < 0)
		? sprite->transform.getScale().x * -1
		: sprite->transform.getScale().x;

	const float scale_y = (sprite->transform.getScale().y < 0)
		? sprite->transform.getScale().y * -1
		: sprite->transform.getScale().y;


	const float left = sprite->getOrigininalPosition().x;
	const float right = sprite->getOrigininalPosition().x + sprite->transform.getDefaultTextureSize().x * scale_x;
	const float middle_y = (sprite->getOrigininalPosition().y + (sprite->transform.getDefaultTextureSize().y * scale_y) / 2) ;
	const float bottom = sprite->getOrigininalPosition().y + sprite->transform.getDefaultTextureSize().y * scale_y;
	const float middle_x = sprite->getOrigininalPosition().x + (sprite->transform.getDefaultTextureSize().x * scale_x) / 2;
	const float top = sprite->getOrigininalPosition().y;

	const sf::Vector2f pos_width_y = sf::Vector2f(right + sprite->collider.box_collider_width.y, middle_y);
	const sf::Vector2f pos_width_x = sf::Vector2f(left - DEFAULT_DOTT_SCALE + sprite->collider.box_collider_width.x, middle_y);
		
	const sf::Vector2f pos_height_x = sf::Vector2f(middle_x - DEFAULT_DOTT_SCALE, top + sprite->collider.box_collider_height.x - DEFAULT_DOTT_SCALE);
	const sf::Vector2f pos_height_y = sf::Vector2f(middle_x - DEFAULT_DOTT_SCALE, top + sprite->collider.box_collider_height.y + sprite->transform.getDefaultTextureSize().y * scale_y);

	this->m_box_collider_scale_dotts[0].ptr_scaling_rectangle->shape.setPosition(pos_width_y);
	this->m_box_collider_scale_dotts[1].ptr_scaling_rectangle->shape.setPosition(pos_width_x);
	this->m_box_collider_scale_dotts[2].ptr_scaling_rectangle->shape.setPosition(pos_height_x);
	this->m_box_collider_scale_dotts[3].ptr_scaling_rectangle->shape.setPosition(pos_height_y);

	const sf::Vector2f temp_scale_dott_pos_zero = this->m_box_collider_scale_dotts[0].ptr_scaling_rectangle->shape.getPosition();
	const sf::Vector2f temp_scale_dott_pos_one = this->m_box_collider_scale_dotts[1].ptr_scaling_rectangle->shape.getPosition();
	const sf::Vector2f temp_scale_dott_pos_two = this->m_box_collider_scale_dotts[2].ptr_scaling_rectangle->shape.getPosition();
	const sf::Vector2f temp_scale_dott_pos_three = this->m_box_collider_scale_dotts[3].ptr_scaling_rectangle->shape.getPosition();

	const float new_scale_width_y = s2d::UI::xScaleChanger(this->m_box_collider_scale_dotts[0], sprite->transform.getDefaultTextureSize().x,0);
	const float new_scale_width_x = s2d::UI::xScaleChanger(this->m_box_collider_scale_dotts[1], sprite->transform.getDefaultTextureSize().x, 0);

	const float new_scale_height_x = s2d::UI::yScaleChanger(this->m_box_collider_scale_dotts[2], sprite->transform.getDefaultTextureSize().x, 0);
	const float new_scale_height_y = s2d::UI::yScaleChanger(this->m_box_collider_scale_dotts[3], sprite->transform.getDefaultTextureSize().x, 0);


	bool must_return = false;

	// Resetting right rectangle
	if (this->m_box_collider_scale_dotts[0].ptr_scaling_rectangle->shape.getPosition().x
		< DEFAULT_DOTT_SCALE + INVALID_AERA + this->m_box_collider_scale_dotts[1].ptr_scaling_rectangle->shape.getPosition().x)
	{
		this->m_box_collider_scale_dotts[0].ptr_scaling_rectangle->shape.
			setPosition(temp_scale_dott_pos_zero);
		must_return = true;
	}

	// Resetting left rectangle
	if (this->m_box_collider_scale_dotts[1].ptr_scaling_rectangle->shape.getPosition().x + DEFAULT_DOTT_SCALE
		>  INVALID_AERA + this->m_box_collider_scale_dotts[0].ptr_scaling_rectangle->shape.getPosition().x)
	{
		this->m_box_collider_scale_dotts[1].ptr_scaling_rectangle->shape.
			setPosition(temp_scale_dott_pos_one);
		must_return = true;
	}

	// Resetting top rectangle
	if (this->m_box_collider_scale_dotts[2].ptr_scaling_rectangle->shape.getPosition().y + DEFAULT_DOTT_SCALE
	> INVALID_AERA + this->m_box_collider_scale_dotts[3].ptr_scaling_rectangle->shape.getPosition().y)
	{
		this->m_box_collider_scale_dotts[2].ptr_scaling_rectangle->shape.
			setPosition(temp_scale_dott_pos_two);
		must_return = true;
	}

	// Resetting bottom rectangle
	if (this->m_box_collider_scale_dotts[3].ptr_scaling_rectangle->shape.getPosition().y
	< INVALID_AERA + DEFAULT_DOTT_SCALE + this->m_box_collider_scale_dotts[2].ptr_scaling_rectangle->shape.getPosition().y)
	{
		this->m_box_collider_scale_dotts[3].ptr_scaling_rectangle->shape.
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
		const float scale_dott_pos_x = this->m_box_collider_scale_dotts[0].ptr_scaling_rectangle->shape.getPosition().x; 
		const float width = scale_dott_pos_x - right;
		sprite->collider.box_collider_width.y = width;
	}
	if (new_scale_width_x != INVALID_SCALE)
	{
		const float scale_dott_pos_x = this->m_box_collider_scale_dotts[1].ptr_scaling_rectangle->shape.getPosition().x;
		const float width = scale_dott_pos_x - left + DEFAULT_DOTT_SCALE;

		sprite->collider.box_collider_width.x = width;
	}

	// Height
	if (new_scale_height_x != INVALID_SCALE) 
	{
		const float scale_dott_pos_y = this->m_box_collider_scale_dotts[2].ptr_scaling_rectangle->shape.getPosition().y;
		const float width = scale_dott_pos_y - top + DEFAULT_DOTT_SCALE;
		sprite->collider.box_collider_height.x = width;
	}
	if (new_scale_height_y != INVALID_SCALE)
	{
		const float scale_dott_pos_y = this->m_box_collider_scale_dotts[3].ptr_scaling_rectangle->shape.getPosition().y;
		const float width = scale_dott_pos_y - bottom;
		sprite->collider.box_collider_height.y = width;
	}

	this->reset();
}

// Public functions

void s2d::UIInspectorBoxCollider::edit(float& x, float& y)
{
	ImGui::SetCursorPosX(x += 10);
	ImGui::Text("Edit");
	ImGui::SetCursorPos(ImVec2(x += 53.5f, y -= 2.5f));
	if (s2d::FontManager::displaySmybolAsButton(ICON_FA_EDIT))
	{
		this->m_edit_mode = !this->m_edit_mode;
	}
	x += 19;

	x -= 12.5f;
}

void s2d::UIInspectorBoxCollider::solid(float& x, float& y, s2d::Sprite* sprite)
{
	ImGui::SetCursorPosX(x -= 60.4f);
	ImGui::Text("Solid");
	ImGui::SameLine();
	ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size);
	ImGui::Checkbox("##Solid", &sprite->collider.is_solid);
	ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size);
	ImGui::Dummy(ImVec2(0, 5));

	y += 40;
}

void s2d::UIInspectorBoxCollider::width(float x, float y, s2d::Sprite* sprite)
{
	ImGui::SetCursorPosX(x -= 11);
	ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size + 0.1f);
	if (ImGui::TreeNode("Width"))
	{
		ImGui::SetCursorPos(ImVec2(x += 30, y += 80));
		ImGui::Text("L");

		ImGui::SetCursorPos(ImVec2(x += 30, y));
		ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size - 0.5f);
		ImGui::SliderFloat("##WidthL", &sprite->collider.box_collider_width.x, -500, 500);
		ImGui::SameLine();

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 7);
		if (s2d::FontManager::displaySmybolAsButton(ICON_FA_RETWEET, s2d::UIInfo::s_default_font_size + 0.02f))
		{
			sprite->collider.box_collider_width.x = 0;
		}
		ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size);

		ImGui::SetCursorPos(ImVec2(x -= 28.4f, y += 40));
		ImGui::Text("R");

		ImGui::SetCursorPos(ImVec2(x += 29, y));
		ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size - 0.5f);
		ImGui::SliderFloat("##WidthR", &sprite->collider.box_collider_width.y, -500, 500);
		ImGui::SameLine();

		// If we wouldn use the ## the name would still be just retweet. But now it has a different 
		// name and it acts like a different button, ImGui does not render '##'

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 7);
		if (s2d::FontManager::displaySmybolAsButton(ICON_FA_RETWEET "##", s2d::UIInfo::s_default_font_size + 0.02f))
		{
			sprite->collider.box_collider_width.y = 0;
		}
		ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size);
		ImGui::TreePop();
	}
	ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size);
}

void s2d::UIInspectorBoxCollider::height(s2d::Sprite* sprite)
{
	float x = ImGui::GetCursorPosX();
	float y = ImGui::GetCursorPosY();
	ImGui::SetCursorPosX(x -= 1.5f);

	ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size + 0.07f);
	if (ImGui::TreeNodeEx("Height"))
	{
		ImGui::SetCursorPos(ImVec2(x += 33.5f, y += 35));
		ImGui::Text("U");

		ImGui::SetCursorPos(ImVec2(x += 30, y));
		ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size - 0.5f);
		ImGui::SliderFloat("##up", &sprite->collider.box_collider_height.x, -500, 500);
		ImGui::SameLine();
		if (s2d::FontManager::displaySmybolAsButton(ICON_FA_RETWEET))
		{
			sprite->collider.box_collider_height.x = 0;
		}
		ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size);

		ImGui::SetCursorPos(ImVec2(x -= 28.4f, y += 40));
		ImGui::Text("D");

		ImGui::SetCursorPos(ImVec2(x += 28, y));
		ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size - 0.5f);
		ImGui::SliderFloat("##down", &sprite->collider.box_collider_height.y, -500, 500);
		ImGui::SameLine();
		if (s2d::FontManager::displaySmybolAsButton(ICON_FA_RETWEET "##"))
		{
			sprite->collider.box_collider_height.y = 0;
		}
		ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size);
		ImGui::TreePop();
	}
	ImGui::SetWindowFontScale(s2d::UIInfo::s_default_font_size);
}

void s2d::UIInspectorBoxCollider::drawBoxCollider(s2d::Sprite* sprite, s2d::Rectangle* ptr_rectangle)
{
	const sf::Vector2f size = sf::Vector2f(sprite->transform.textureSize.x + (-sprite->collider.box_collider_width.x + sprite->collider.box_collider_width.y),
		sprite->transform.textureSize.y + (-sprite->collider.box_collider_height.x + sprite->collider.box_collider_height.y));

	sf::RectangleShape* ptr_shape = &ptr_rectangle->shape;

	ptr_shape->setSize(size);
	ptr_shape->setPosition(sf::Vector2f(sprite->getOrigininalPosition().x + sprite->collider.box_collider_width.x, sprite->getOrigininalPosition().y + sprite->collider.box_collider_height.x));

	ptr_rectangle->render = true;

	if (this->m_edit_mode)
	{
		this->renderDotts();
		this->renderScaleDotts(sprite, ptr_rectangle);
	}
	else
	{
		this->unrenderDotts();
	}
}

void s2d::UIInspectorBoxCollider::initScaleDottsUI(s2d::GUIRepository& repo)
{
	const sf::Vector2f size = sf::Vector2f(DEFAULT_DOTT_SCALE, DEFAULT_DOTT_SCALE);

	for (int i = 0; i < SCALE_DOTTS_COLLIDER; i++)
	{
		const std::string name = "scale-dott-box-collider " + std::to_string(i);
		repo.add(sf::Vector2f(0, 0),
			size, sf::Color(255, 255, 255), 2.0f, PATH_TO_TRANSPARENT_PIC, name);
		this->m_box_collider_scale_dotts[i].ptr_scaling_rectangle = repo.getByName(name);
		this->m_box_collider_scale_dotts[i].clicked = false;
	}

	this->unrenderDotts();
}

void s2d::UIInspectorBoxCollider::renderDotts()
{
	for (int i = 0; i < SCALE_DOTTS_COLLIDER; i++)
	{
		this->m_box_collider_scale_dotts[i].ptr_scaling_rectangle->render = true;
	}
}

void s2d::UIInspectorBoxCollider::unrenderDotts()
{
	for (int i = 0; i < SCALE_DOTTS_COLLIDER; i++)
	{
		this->m_box_collider_scale_dotts[i].clicked = false;
		this->m_box_collider_scale_dotts[i].ptr_scaling_rectangle->render = false;
	}
}

