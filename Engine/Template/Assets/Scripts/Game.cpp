#include "Game.h"

void Game::Start()
{

}

void Game::Update()
{	
	std::string fps = "FPS: " + std::to_string(spe::Time::s_FPS);
	ImGui::Text(fps.c_str());

}
