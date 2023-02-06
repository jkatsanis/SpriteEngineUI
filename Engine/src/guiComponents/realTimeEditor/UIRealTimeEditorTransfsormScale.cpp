#include "UIRealTimeEditorTransfsormScale.h"

// Constructor


s2d::UIRealTimeEditorTransfsormScale::UIRealTimeEditorTransfsormScale()
{
	init();
}

s2d::UIRealTimeEditorTransfsormScale::UIRealTimeEditorTransfsormScale(s2d::Event* event)
{
	init();

	this->m_x_scaleChanger.setSize(sf::Vector2f(20, 20));
	this->m_x_scaleChanger.setFillColor(sf::Color(255, 255, 255));
	this->m_x_vecpos = s2d::GameObject::rects.size();
	s2d::GameObject::rects.push_back(this->m_x_scaleChanger);

	this->m_y_scaleChanger.setSize(sf::Vector2f(20, 20));
	this->m_y_scaleChanger.setFillColor(sf::Color(255, 255, 255));
	this->m_y_vecpos = s2d::GameObject::rects.size();
	s2d::GameObject::rects.push_back(this->m_y_scaleChanger);
	this->m_event = event;
}

void s2d::UIRealTimeEditorTransfsormScale::init()
{
	this->m_event = nullptr;
	this->m_x_scaleChanger = sf::RectangleShape();
	this->m_y_scaleChanger = sf::RectangleShape();

	this->m_clicked_y = false;
	this->m_clicked_x = false;
}

// Public functions

void s2d::UIRealTimeEditorTransfsormScale::update()
{
	s2d::Sprite* focusedSprite = s2d::UIHirachy::selectedSprite;
	if (focusedSprite != nullptr)
	{
		s2d::Vector2 originalPos = focusedSprite->getOrigininalPosition();
		s2d::Vector2 textureSize = focusedSprite->transform.textureSize;

		sf::Vector2f pos_x = sf::Vector2f(originalPos.x + textureSize.x, originalPos.y + textureSize.y / 2);
		sf::Vector2f pos_y = sf::Vector2f(originalPos.x + textureSize.x / 2, originalPos.y + textureSize.y);

		if (!m_clicked_y)
		{
			s2d::GameObject::rects[m_y_vecpos].setPosition(pos_y);
		}
		if (!m_clicked_x)
		{
			s2d::GameObject::rects[m_x_vecpos].setPosition(pos_x);
		}

		this->xScaleChanger(focusedSprite);
		this->yScaleChanger(focusedSprite);
	

		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			this->m_event->type = s2d::Event::None;
			this->m_clicked_x = false;
			this->m_clicked_y = false;
		}
	
		
		this->m_currentCursorPos = s2d::UI::getWorldCordinates();
	}
}

// Private functions

void s2d::UIRealTimeEditorTransfsormScale::xScaleChanger(s2d::Sprite* focusedSprite)
{
	if (s2d::UI::isCursorClickedOnRectangle(s2d::GameObject::rects[m_x_vecpos]))
	{
		m_clicked_x = true;
	}
	if (m_clicked_x && sf::Mouse::isButtonPressed(sf::Mouse::Left) && focusedSprite != nullptr)
	{
		sf::Vector2f pos = sf::Vector2f(s2d::UI::getWorldCordinates().x, s2d::GameObject::rects[m_x_vecpos].getPosition().y);

		s2d::GameObject::rects[m_x_vecpos].setPosition(pos);

		pos.x -= 960;

		float scale = (pos.x - focusedSprite->transform.position.x) / (focusedSprite->transform.getDefaultTextureSize().x / 2);

		focusedSprite->transform.setScale(s2d::Vector2(scale, focusedSprite->transform.getScale().y));
	}
}

void s2d::UIRealTimeEditorTransfsormScale::yScaleChanger(s2d::Sprite* focusedSprite)
{
	if (s2d::UI::isCursorClickedOnRectangle(s2d::GameObject::rects[m_y_vecpos]))
	{
		m_clicked_y = true;
	}
	if (m_clicked_y && sf::Mouse::isButtonPressed(sf::Mouse::Left) && focusedSprite != nullptr)
	{
		sf::Vector2f pos = sf::Vector2f(s2d::GameObject::rects[m_y_vecpos].getPosition().x, s2d::UI::getWorldCordinates().y);

		s2d::GameObject::rects[m_y_vecpos].setPosition(pos);

		pos.y -= 540;

		float scale = (pos.y + focusedSprite->transform.position.y) / (focusedSprite->transform.getDefaultTextureSize().y / 2);

		focusedSprite->transform.setScale(s2d::Vector2(focusedSprite->transform.getScale().x, scale));
	}
}
