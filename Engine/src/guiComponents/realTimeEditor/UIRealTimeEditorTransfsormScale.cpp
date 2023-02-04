#include "UIRealTimeEditorTransfsormScale.h"

// Constructor

s2d::UIRealTimeEditorTransfsormScale::UIRealTimeEditorTransfsormScale()
{
	s2d::GameObject::rects.push_back(sf::RectangleShape());

	s2d::GameObject::rects[2].setSize(sf::Vector2f(20, 20));
	s2d::GameObject::rects[2].setOutlineColor(sf::Color(124, 252, 0));
	s2d::GameObject::rects[2].setOutlineThickness(3.5f);
}

// Public functions

void s2d::UIRealTimeEditorTransfsormScale::update()
{

}