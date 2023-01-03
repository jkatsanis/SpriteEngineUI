#include "Input.h"

bool s2d::Input::onKeyRelease(s2d::KeyBoardCode key)
{
	if (s2d::Input::event->type == s2d::Event::Released && s2d::Input::event->key == key)
	{
		s2d::Input::event->type = s2d::Event::None;
		return true;
	}
	return false;
}

bool s2d::Input::onKeyPress(s2d::KeyBoardCode key)
{
	if (s2d::Input::event->type == s2d::Event::Pressed && s2d::Input::event->key == key)
	{
		s2d::Input::event->type = s2d::Event::None;
		return true;
	}
	return false;
}

bool s2d::Input::onKeyHold(s2d::KeyBoardCode key)
{
	sf::Keyboard::Key keyd = static_cast<sf::Keyboard::Key>(static_cast<s2d::KeyBoardCode>(key));

	return sf::Keyboard::isKeyPressed(keyd);
}

void s2d::Input::setEvent(s2d::Event* event)
{
	s2d::Input::event = event;
}


s2d::Event* s2d::Input::event = nullptr;