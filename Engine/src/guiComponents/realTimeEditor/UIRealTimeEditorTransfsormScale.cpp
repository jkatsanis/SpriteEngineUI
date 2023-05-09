#include "UIRealTimeEditorTransfsormScale.h"

// Constructor


s2d::UIRealTimeEditorTransfsormScale::UIRealTimeEditorTransfsormScale()
{
	for (size_t i = 0; i < SCALE_DOTTS; i++)
	{
		this->m_scale_dotts[i] = s2d::ScaleDott();
	}
	this->m_ptr_gui_repo = nullptr;
	this->m_ptr_repo = nullptr;
	this->m_event = nullptr;
}

s2d::UIRealTimeEditorTransfsormScale::UIRealTimeEditorTransfsormScale(s2d::Event* event, s2d::SpriteRepository& repo, s2d::GUIRepository& gui_repo)
{
	this->m_ptr_gui_repo = &gui_repo;
	this->m_ptr_repo = &repo;

	const sf::Vector2f size = sf::Vector2f(DEFAULT_DOLL_SCALE, DEFAULT_DOLL_SCALE);

	for (int i = 0; i < SCALE_DOTTS; i++)
	{
		const std::string name = "scale-dott-scale " + std::to_string(i);
		this->m_ptr_gui_repo->add(sf::Vector2f(0, 0),
			size, sf::Color(255, 255, 255), 2.0f, PATH_TO_TRANSPARENT_PIC, name);
		this->m_scale_dotts[i].ptr_scaling_rectangle = this->m_ptr_gui_repo->getByName(name);
		this->m_scale_dotts[i].clicked = false;
	}

	this->m_scale = s2d::Vector2(1, 1);
	this->unrenderDolls();
	this->m_event = event;
}

// Public functions

void s2d::UIRealTimeEditorTransfsormScale::update()
{
	s2d::Sprite* focusedSprite = this->m_ptr_repo->sprite_in_inspector;
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
		this->m_scale_dotts[i].ptr_scaling_rectangle->render = true;
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
		this->m_scale_dotts[i].ptr_scaling_rectangle->render = false;
	}
}

// Private functions

void s2d::UIRealTimeEditorTransfsormScale::scaleChanger(s2d::Sprite* focusedSprite)
{
	float scale_x = s2d::UI::xScaleChanger(this->m_scale_dotts[0], focusedSprite->transform.getDefaultTextureSize().x,
		focusedSprite->transform.position.x);
	if (scale_x != INVALID_SCALE)
		focusedSprite->transform.setScale(s2d::Vector2(scale_x, focusedSprite->transform.getScale().y));

	float scale_y = s2d::UI::yScaleChanger(this->m_scale_dotts[1], focusedSprite->transform.getDefaultTextureSize().y,
		focusedSprite->transform.position.y);
	if (scale_y != INVALID_SCALE)
		focusedSprite->transform.setScale(s2d::Vector2(focusedSprite->transform.getScale().x, scale_y));
}


void s2d::UIRealTimeEditorTransfsormScale::reset()
{
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		this->m_event->type = s2d::Event::None;
		for (int i = 0; i < SCALE_DOTTS; i++)
		{
			this->m_scale_dotts[i].clicked = false;
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
		if (!this->m_scale_dotts[i].clicked)
		{
			this->m_scale_dotts[i].ptr_scaling_rectangle->shape.setPosition(pos[i]);
		}
	}
}