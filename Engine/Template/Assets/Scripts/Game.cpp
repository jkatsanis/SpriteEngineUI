#include "Game.h"

void Game::Start()
{
	this->m_PlayerController.Start(this->EngineConfig);
}

void Game::Update()
{	
	std::string fps = std::to_string(spe::Time::s_fps);
	ImGui::Text(fps.c_str());
	this->m_PlayerController.Update();
}
