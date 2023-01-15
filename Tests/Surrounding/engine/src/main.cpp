#include <iostream>
#include <SFML/Graphics.hpp>
#include <engineComponents/game.h>

int main()
{
	s2d::Game game;
	game.start();

	while (game.isGameRunning())
	{
		game.update();
	}
}

