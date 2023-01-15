#include <iostream>
#include <SFML/Graphics.hpp>
#include <engineComponents/gameEngine.h>

int main()
{
	s2d::GameEngine game;
	game.start();

	while (game.isGameRunning())
	{
		game.update();
	}
}

