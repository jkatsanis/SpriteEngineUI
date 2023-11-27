#include "Input.h"

bool spe::Input::onKeyRelease(spe::KeyBoardCode key)
{
	return spe::Input::event->type == spe::Event::KeyReleased && spe::Input::event->key == key;
}

bool spe::Input::onKeyPress(spe::KeyBoardCode key)
{
	return spe::Input::event->type == spe::Event::KeyPressed && spe::Input::event->key == key;
}

bool spe::Input::onKeyHold(spe::KeyBoardCode key)
{
	sf::Keyboard::Key keyd = static_cast<sf::Keyboard::Key>(static_cast<spe::KeyBoardCode>(key));

	return sf::Keyboard::isKeyPressed(keyd);
}

void spe::Input::setEvent(spe::Event* event)
{
	spe::Input::event = event;
}


spe::Event* spe::Input::event = nullptr;