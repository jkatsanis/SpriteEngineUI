#include "Engine.h"

// Ctor / Dtor

spe::Engine::Engine()
{
	this->m_Window = spe::GameWindow(spe::Vector2(1920, 1080), "Game");

	this->Init();

	this->m_Window.SetCamera(&this->m_Camera);
	this->m_Window.SetBackgroundColor(&this->m_BackgroundColor);

	this->m_Game.EngineConfig = spe::EngineConfig(&this->m_SceneHandler, &this->m_Camera, &this->m_BackgroundColor, this->m_Window.GetRenderWindow());

	spe::Input::SetEvent(&this->m_Window.Event);
	spe::Style::RenderStyle();

	spe::GUI::SetCamera(&this->m_Camera);
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

	this->ReadEnginePath();

	this->m_SceneHandler.Init("Engine/Ressources/shaders/circulaer_gradient.frag");

	spe::Initializer::InitTags(this->m_SceneHandler.SpriteRepository, PATH_TO_TAG_FILE);
	spe::Initializer::InitScenes(this->m_SceneHandler, PATH_TO_SCENE_FILE);
	spe::Initializer::IntiHighestSpriteID(this->m_SceneHandler.SpriteRepository, PATH_TO_HIGHEST_INDEX);
	spe::PrefabRepository::LoadPrefabsInMemory(this->m_SceneHandler.LightRepository);

	this->m_SceneHandler.LoadScene(this->m_SceneHandler.TotalScenes[0], this->m_Camera, this->m_BackgroundColor);

	spe::BoxCollider::InitCameraCollider(this->m_SceneHandler.LightRepository);
	// this->m_Camera.reset();

}

void spe::Engine::ReadEnginePath()
{
	std::fstream file;
	OPEN_FILE(file, "Engine/Saves/enginepath.txt", std::ios::in);

	if (!file) {
		throw std::runtime_error("Failed to open file.");
	}

	std::string firstLine;
	if (std::getline(file, firstLine)) {
		spe::EngineData::s_PathOfEngine = firstLine;
	} else {
		throw std::runtime_error("File is empty or failed to read the first line.");
	}
}

// Private

void spe::Engine::UpdatePhysics()
{
	std::list<spe::Sprite*>& sprites = this->m_SceneHandler.SpriteRepository.GetSprites();

	for (auto it = sprites.begin(); it != sprites.end(); ++it)
	{
		spe::Sprite* sprite = *it;

		if (!sprite->Process)
		{
			continue;
		}

		sprite->Collider.Update(this->m_SceneHandler.SpriteRepository);
		sprite->Physicsbody.Update();
	}
}

void spe::Engine::UpdateComponents()
{
	this->m_Window.PollEvents();

	// Updating the user here
	ImGui::Begin("##MainWindow", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
	this->m_Game.Update();	

	sf::Vector2u windowSize = this->m_Window.GetRenderWindow()->getSize();
	ImGui::SetWindowSize(ImVec2((float)windowSize.x, (float)windowSize.y));
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize + 0.5f);
	ImGui::End();

	std::list<spe::Sprite*>& sprites = this->m_SceneHandler.SpriteRepository.GetSprites();

	this->m_Window.Clear();

	// Force update transforms if window resized
	if (spe::EngineData::s_WindowResized)
	{
		for (auto it = sprites.begin(); it != sprites.end(); ++it)
		{
			(*it)->Transform.RefreshScreenPosition();
		}
		spe::EngineData::s_WindowResized = false;
	}

    // Update Camera and Shader Uniforms BEFORE drawing
    this->m_Camera.Update(&this->m_SceneHandler.LightRepository);

	for (auto it = sprites.begin(); it != sprites.end(); ++it)
	{
		spe::Sprite* sprite = *it;

		if (!spe::BoxCollider::ProcessSprite(sprite, this->m_Camera))
		{
			sprite->DisableProcess();
			continue;
		}
		sprite->EnableProcess();


		this->m_SceneHandler.LightRepository.UpdateLightSource(sprite, &this->m_Camera);

		sprite->Particles.Update();
		sprite->Animator.Update();
		// physics and collissions are now handled in updatephysics with fixed timestep

		this->m_Window.DrawGame(sprite, &this->m_SceneHandler.LightRepository.GetShader(), false);
		this->m_Window.DrawParticles(sprite, &this->m_SceneHandler.LightRepository.GetShader(), false);
	}

	this->m_SceneHandler.LightRepository.UpdateArrays();

	this->m_Window.Display();
}


// Public

void spe::Engine::Update()
{
	if (this->m_SceneHandler.SceneChanged)
	{
		this->m_Game.Start(this->m_SceneHandler.CurrentScene);
		this->m_SceneHandler.SceneChanged = false;
	}

	spe::Time::Update();

	// clamp time to avoid spiral of death (e.g. during debugging or lag spikes)
	float frameTime = spe::Time::s_DeltaTime;
	if (frameTime > spe::Time::MAX_FRAME_TIME)
	{
		frameTime = spe::Time::MAX_FRAME_TIME;
	}

	// accumulate time for fixed timestep physics
	spe::Time::s_Accumulator += frameTime;

	while (spe::Time::s_Accumulator >= spe::Time::FIXED_DELTA_TIME)
	{
		spe::Time::s_FixedDeltaTime = spe::Time::FIXED_DELTA_TIME;

		this->UpdatePhysics();

		spe::Time::s_Accumulator -= spe::Time::FIXED_DELTA_TIME;
	}
	this->UpdateComponents();
}
