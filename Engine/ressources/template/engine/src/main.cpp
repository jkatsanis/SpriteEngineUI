#include <iostream>
#include <SFML/Graphics.hpp>
#include <engineComponents/gameEngine.h>

int main()
{
	s2d::GameEngine game;
	game.start();

	if (game.ptr_render_window == nullptr)
	{
		std::cout << "Window was nullptr, please rebuild!!" << std::endl;
		return -1;
	}

	while (game.isGameRunning())
	{
		game.update();
	}
}

