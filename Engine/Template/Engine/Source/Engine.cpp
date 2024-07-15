#include "Engine.h"

// Ctor / Dtor

spe::Engine::Engine()
{
	this->m_Window = spe::GameWindow(spe::Vector2(1920, 1080), "Game");

	this->Init();

	this->m_Window.SetCamera(&this->m_Camera);
	this->m_Window.SetBackgroundColor(&this->m_BackgroundColor);

	this->m_Game.EngineConfig = spe::EngineConfig(&this->m_SceneHandler, &this->m_Camera, &this->m_BackgroundColor);

	spe::Input::SetEvent(&this->m_Window.Event);
	spe::Style::RenderStyle();

	spe::GUI::SetCamera(&this->m_Camera);

	this->m_Game.Start();
}

spe::Engine::~Engine()
{
	spe::BoxCollider::DeleteCameraCollider();
	this->m_SceneHandler.SpriteRepository.DeleteAll();
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

	spe::PrefabRepository::LoadPrefabsInMemory(this->m_SceneHandler.LightRepository);

	this->m_SceneHandler.LoadScene(this->m_SceneHandler.TotalScenes[0], this->m_Camera, this->m_BackgroundColor);

	spe::BoxCollider::InitCameraCollider(this->m_SceneHandler.LightRepository);
	// this->m_Camera.reset();
}

// Private

void spe::Engine::UpdateComponents()
{
	this->m_Window.PollEvents();

	// Updating the user here
	ImGui::Begin("##MainWindow", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
	this->m_Game.Update();	

	ImGui::SetWindowSize(ImVec2(1920, 1080));
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize + 0.5f);
	ImGui::End();

	std::list<spe::Sprite*>& sprites = this->m_SceneHandler.SpriteRepository.GetSprites();

	this->m_Window.Clear();

	for (auto it = sprites.begin(); it != sprites.end(); ++it)
	{
		spe::Sprite* sprite = *it;
	
		if (!spe::BoxCollider::ProcessSprite(sprite, this->m_Camera))
		{
			sprite->Process = false;
			continue;
		}
		sprite->Process = true;

		this->m_SceneHandler.LightRepository.UpdateLightSource(sprite, &this->m_Camera);

		sprite->Animator.Update();
		sprite->Collider.Update(this->m_SceneHandler.SpriteRepository);
		sprite->Physicsbody.Update();

		this->m_Window.DrawGame(sprite, &this->m_SceneHandler.LightRepository.GetShader(), false);
	}
	this->m_SceneHandler.LightRepository.UpdateArrays();

	this->m_Window.Display();
}


// Public

void spe::Engine::Update()
{
	spe::Time::Update();
	if (spe::Time::s_TimePassed <= 1.0f)
	{
		return;
	}
	this->UpdateComponents();
	this->m_Camera.Update(&this->m_SceneHandler.LightRepository);
}