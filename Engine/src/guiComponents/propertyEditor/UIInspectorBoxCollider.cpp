#include "UIInspectorBoxCollider.h"

// Constructor / Destructor

s2d::UIInspectorBoxCollider::UIInspectorBoxCollider()
{	
	for (size_t i = 0; i < SCALE_DOTTS; i++)
	{
		this->m_box_collider_scale_dotts[i] = s2d::ScaleDott();
	}

	this->m_edit_mode = false;
}

// Private functions

void s2d::UIInspectorBoxCollider::renderScaleDotts(s2d::Sprite* sprite, s2d::Rectangle* ptr_box_collider_rec)
{

}

void s2d::UIInspectorBoxCollider::getPos(const s2d::Sprite* focusedSprite, sf::Vector2f pos[])
{
	s2d::Vector2 originalPos = focusedSprite->getOrigininalPosition();
	s2d::Vector2 textureSize = focusedSprite->transform.textureSize;

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
	pos[1] = sf::Vector2f(originalPos.x + textureSize.x / 2 - DEFAULT_DOLL_SCALE / 2, originalPos.y + textureSize.y);
}

void s2d::UIInspectorBoxCollider::setPos(const sf::Vector2f pos[])
{
	for (int i = 0; i < SCALE_DOTTS; i++)
	{
		if (!this->m_box_collider_scale_dotts[i].clicked)
		{
			this->m_box_collider_scale_dotts[i].ptr_scaling_rectangle->shape.setPosition(pos[i]);
		}
	}
}

// Public functions

void s2d::UIInspectorBoxCollider::edit(float& x, float& y)
{
	ImGui::SetCursorPosX(x += 10);
	ImGui::Text("Edit");
	ImGui::SetCursorPos(ImVec2(x += 53.5f, y -= 2.5f));
	if (s2d::FontManager::displaySmybolAsButton(ICON_FA_EDIT))
	{
		this->m_edit_mode = true;
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
	sf::Vector2f size = sf::Vector2f(sprite->transform.textureSize.x + (-sprite->collider.box_collider_width.x + sprite->collider.box_collider_width.y),
		sprite->transform.textureSize.y + (-sprite->collider.box_collider_height.x + sprite->collider.box_collider_height.y));

	sf::RectangleShape* ptr_shape = &ptr_rectangle->shape;

	ptr_shape->setSize(size);
	ptr_shape->setPosition(sf::Vector2f(sprite->getOrigininalPosition().x + sprite->collider.box_collider_width.x, sprite->getOrigininalPosition().y + sprite->collider.box_collider_height.x));

	ptr_rectangle->render = true;

	this->renderScaleDotts(sprite, ptr_rectangle);
}

void s2d::UIInspectorBoxCollider::initScaleDottsUI(s2d::GUIRepository& repo)
{
	const sf::Vector2f size = sf::Vector2f(DEFAULT_DOLL_SCALE, DEFAULT_DOLL_SCALE);

	for (int i = 0; i < SCALE_DOTTS; i++)
	{
		const std::string name = "scale-dott-box-collider " + std::to_string(i);
		repo.add(sf::Vector2f(0, 0),
			size, sf::Color(255, 255, 255), 2.0f, PATH_TO_TRANSPARENT_PIC, name);
		this->m_box_collider_scale_dotts[i].ptr_scaling_rectangle = repo.getByName(name);
		this->m_box_collider_scale_dotts[i].clicked = false;
	}
}

