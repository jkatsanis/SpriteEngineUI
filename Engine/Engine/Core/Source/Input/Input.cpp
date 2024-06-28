#include "Input.h"

bool spe::Input::OnKeyRelease(spe::KeyBoardCode key)
{
	return spe::Input::Event->Type == spe::Event::KeyReleased && spe::Input::Event->Key == key;
}

bool spe::Input::OnKeyPress(spe::KeyBoardCode key)
{
	return spe::Input::Event->Type == spe::Event::KeyPressed && spe::Input::Event->Key == key;
}

bool spe::Input::OnKeyHold(spe::KeyBoardCode key)
{
	sf::Keyboard::Key keyd = static_cast<sf::Keyboard::Key>(static_cast<spe::KeyBoardCode>(key));

	return sf::Keyboard::isKeyPressed(keyd);
}

void spe::Input::SetEvent(spe::Event* evnt) noexcept
{
	spe::Input::Event = evnt;
}

spe::Event* spe::Input::Event = nullptr;