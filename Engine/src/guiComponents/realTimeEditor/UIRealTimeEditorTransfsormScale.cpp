#include "UIRealTimeEditorTransfsormScale.h"

// Constructor


s2d::UIRealTimeEditorTransfsormScale::UIRealTimeEditorTransfsormScale()
{
	this->m_event = nullptr;
}

s2d::UIRealTimeEditorTransfsormScale::UIRealTimeEditorTransfsormScale(s2d::Event* event)
{
	sf::RectangleShape shape;
	shape.setSize(sf::Vector2f(DEFAULT_DOLL_SCALE, DEFAULT_DOLL_SCALE));
	shape.setFillColor(sf::Color(255, 255, 255));

	for (int i = 0; i < SCALE_DOTTS; i++)
	{
		this->m_scaleDotts[i].vecpos = (short)s2d::GameObject::rects.size();
		this->m_scaleDotts[i].shape = shape;
		this->m_scaleDotts[i].clicked = false;

		s2d::GameObject::rects.push_back(this->m_scaleDotts->shape);
	}

	this->m_scale = s2d::Vector2(1, 1);
	this->unrenderDolls();

	this->m_event = event;
}

// Public functions

void s2d::UIRealTimeEditorTransfsormScale::update()
{
	s2d::Sprite* focusedSprite = s2d::UIHirachy::s_selectedSprite;
	if (focusedSprite != nullptr)
	{
		sf::Vector2f pos[SCALE_DOTTS];
		this->getPos(focusedSprite, pos);
		this->setPos(pos);
		this->scaleChanger(focusedSprite);
		this->reset();

		this->m_currentCursorPos = s2d::UI::getWorldCordinates();
	}
}

void s2d::UIRealTimeEditorTransfsormScale::renderDolls()
{
	if (this->m_scale == s2d::Vector2(1, 1))
	{
		return;
	}
	this->m_scale = s2d::Vector2(1, 1);
	for (int i = 0; i < SCALE_DOTTS; i++)
	{
		s2d::GameObject::rects[this->m_scaleDotts[i].vecpos].setScale(1, 1);
	}
}

void s2d::UIRealTimeEditorTransfsormScale::unrenderDolls()
{
	if (this->m_scale == s2d::Vector2(0, 0))
	{
		return;
	}
	this->m_scale = s2d::Vector2(0, 0);
	for (int i = 0; i < SCALE_DOTTS; i++)
	{
		s2d::GameObject::rects[this->m_scaleDotts[i].vecpos].setScale(0, 0);
	}
}

// Private functions

void s2d::UIRealTimeEditorTransfsormScale::scaleChanger(s2d::Sprite* focusedSprite)
{
	int i = 0;
	while (i < SCALE_DOTTS)
	{
		this->xScaleChanger(focusedSprite, this->m_scaleDotts[i]);
		i++;
		this->yScaleChanger(focusedSprite, this->m_scaleDotts[i]);
		i++;
	}
}

void s2d::UIRealTimeEditorTransfsormScale::xScaleChanger(s2d::Sprite* focusedSprite, ScaleDott& dott)
{
	if (s2d::UI::isCursorClickedOnRectangle(s2d::GameObject::rects[dott.vecpos]))
	{
		dott.clicked = true;
	}
	if (dott.clicked && sf::Mouse::isButtonPressed(sf::Mouse::Left) && focusedSprite != nullptr)
	{
		sf::Vector2f pos = sf::Vector2f(s2d::UI::getWorldCordinates().x, s2d::GameObject::rects[dott.vecpos].getPosition().y);

		s2d::GameObject::rects[dott.vecpos].setPosition(pos);

		pos.x -= 960;

		float scale = (pos.x - focusedSprite->transform.position.x) / (focusedSprite->transform.getDefaultTextureSize().x / 2);

		focusedSprite->transform.setScale(s2d::Vector2(scale, focusedSprite->transform.getScale().y));
	}
}

void s2d::UIRealTimeEditorTransfsormScale::yScaleChanger(s2d::Sprite* focusedSprite, ScaleDott& dott)
{
	if (s2d::UI::isCursorClickedOnRectangle(s2d::GameObject::rects[dott.vecpos]))
	{
		dott.clicked = true;
	}
	if (dott.clicked && sf::Mouse::isButtonPressed(sf::Mouse::Left) && focusedSprite != nullptr)
	{
		sf::Vector2f pos = sf::Vector2f(s2d::GameObject::rects[dott.vecpos].getPosition().x, s2d::UI::getWorldCordinates().y);

		s2d::GameObject::rects[dott.vecpos].setPosition(pos);

		pos.y -= 540;

		float scale = (pos.y + focusedSprite->transform.position.y) / (focusedSprite->transform.getDefaultTextureSize().y / 2);

		focusedSprite->transform.setScale(s2d::Vector2(focusedSprite->transform.getScale().x, scale));
	}
}

void s2d::UIRealTimeEditorTransfsormScale::reset()
{
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		this->m_event->type = s2d::Event::None;
		for (int i = 0; i < SCALE_DOTTS; i++)
		{
			this->m_scaleDotts[i].clicked = false;
		}
	}
}

void s2d::UIRealTimeEditorTransfsormScale::getPos(const s2d::Sprite* focusedSprite, sf::Vector2f pos[])
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

void s2d::UIRealTimeEditorTransfsormScale::setPos(const sf::Vector2f pos[])
{
	for (int i = 0; i < SCALE_DOTTS; i++)
	{
		if (!this->m_scaleDotts[i].clicked)
		{
			s2d::GameObject::rects[this->m_scaleDotts[i].vecpos].setPosition(pos[i]);
		}
	}
}