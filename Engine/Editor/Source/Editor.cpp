#include "Editor.h"

spe::Editor::Editor()
{
	spe::EngineData::s_PathUserProject = "C:\\Dev\\Refactor\\SpriteEngineUI\\Engine\\Template";
	spe::EngineData::s_Scene = "scene 1";

	std::string path = PATH_TO_SPRITES;
	this->m_Window = spe::GameWindow(spe::Vector2(1920, 1080), "SpriteEngine");
    spe::Initializer::initSprites(this->m_SpriteRepository, path);
}

void spe::Editor::Update()
{
	this->UpdateComponents();
}

void spe::Editor::UpdateComponents()
{
	this->m_Window.Clear();

	std::list<spe::Sprite*>& sprites = this->m_SpriteRepository.getSprites();

	for (auto it = sprites.begin(); it != sprites.end(); ++it)
	{
		spe::Sprite* sprite = *it;

		this->m_Window.Draw(sprite);
	}

	this->m_Window.Display();
}

