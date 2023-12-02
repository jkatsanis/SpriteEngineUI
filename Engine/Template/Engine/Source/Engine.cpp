#include "Engine.h"

// Ctor / Dtor

spe::Engine::Engine()
{
	this->m_Window = spe::GameWindow(spe::Vector2(1920, 1080), "Game");

	this->Init();

	this->m_Window.SetCamera(&this->m_Camera);
	this->m_Window.SetBackgroundColor(&this->m_BackgroundColor);

	this->m_Game.EngineConfig = spe::EngineConfig(&this->m_SceneHandler, &this->m_Camera, &this->m_BackgroundColor);

	spe::Input::setEvent(&this->m_Window.Event);

	this->m_Game.Start();
}

spe::Engine::~Engine()
{
	this->m_Window.Shutdown();
}

void spe::Engine::Init()
{
	const std::string path = spe::Utility::GetDefaultDir(1);
	spe::Log::LogString(path);
	spe::Utility::SetCurrentDir(path);

	this->m_SceneHandler.Init("Engine\\Ressources\\Shaders\\circulaer_gradient.frag");

	spe::Initializer::InitTags(this->m_SceneHandler.SpriteRepository, PATH_TO_TAG_FILE);
	spe::Initializer::InitScenes(this->m_SceneHandler, PATH_TO_SCENE_FILE);
	spe::Initializer::IntiHighestSpriteID(this->m_SceneHandler.SpriteRepository, PATH_TO_HIGHEST_INDEX);

	this->m_SceneHandler.LoadScene(this->m_SceneHandler.TotalScenes[0], this->m_Camera, this->m_BackgroundColor);

	// this->m_Camera.reset();
}

// Private

void spe::Engine::UpdateComponents()
{
	this->m_Window.PollEvents();

	// Updating the user here

	ImGui::Begin("user-window");
	this->m_Game.Update();
	ImGui::End();

	std::list<spe::Sprite*>& sprites = this->m_SceneHandler.SpriteRepository.GetSprites();

	this->m_Window.Clear();

	for (auto it = sprites.begin(); it != sprites.end(); ++it)
	{
		spe::Sprite* sprite = *it;

		sprite->animator.update();
		sprite->collider.Update(this->m_SceneHandler.SpriteRepository);
		sprite->physicsBody.Update();

		this->m_SceneHandler.LightRepository.updateLightSource(sprite, &this->m_Camera);
		this->m_Window.Draw(sprite, &this->m_SceneHandler.LightRepository.getShader());
	}
	this->m_SceneHandler.LightRepository.updateArrays();

	this->m_Window.Display();
}

// Public

void spe::Engine::Update()
{
	spe::Time::update();
	if (spe::Time::s_time_passed <= 0.5f)
	{
		return;
	}
	this->UpdateComponents();
	this->m_Camera.Update(&this->m_SceneHandler.LightRepository);
}