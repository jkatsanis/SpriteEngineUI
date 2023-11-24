#include "Editor.h"

// Ctor / dctor
 
spe::Editor::Editor()
{
	this->m_Window = spe::GameWindow(spe::Vector2(1920, 1080), "SpriteEngine");

	// Sprite/Background/Camera
	this->Init();

	this->m_Window.SetCamera(&this->m_GUIRepository.Camera);
	this->m_Window.SetBackgroundColor(&this->m_GUIRepository.background_color);

	this->m_UIWindow.SetRepos(this->m_GUIRepository, this->m_SceneHandler.SpriteRepository, this->m_SceneHandler);

	spe::Style::Init();
	spe::Style::RenderStyle();

	spe::Input::setEvent(&this->m_Window.Event);
}

spe::Editor::~Editor()
{
	this->m_Window.Shutdown();
}

// Private

void spe::Editor::Init()
{
	// Changing the dir, to 1 layer above, because here are all ressources
	const std::string path = spe::Utility::GetCurrentDir();
	spe::Utility::SetCurrentDir(path);

	// Loading the sprites from the user directory
	spe::Utility::SetCurrentDir(spe::EngineData::s_PathUserProject);

	spe::Initializer::InitTags(this->m_SceneHandler.SpriteRepository, PATH_TO_TAG_FILE);
	spe::Initializer::InitScenes(this->m_SceneHandler, PATH_TO_SCENE_FILE);

	// Load the first scene in the file
	this->m_SceneHandler.LoadScene(this->m_SceneHandler.TotalScenes[0], this->m_GUIRepository.Camera, this->m_GUIRepository.background_color);

	// Resetting the directory after loading the user-data
	spe::Utility::SetCurrentDir(path);
	std::cout << spe::Utility::GetCurrentDir();
}

void spe::Editor::UpdateUI()
{
	ImGui::PushFont(spe::Style::s_DefaultFont);
	spe::UIUtility::UpdateCursor();
	this->m_UIWindow.Update();
	ImGui::PopFont();
}

void spe::Editor::UpdateComponents()
{
	this->m_Window.PollEvents();

	this->UpdateUI();

	std::list<spe::Sprite*>& sprites = this->m_SceneHandler.SpriteRepository.GetSprites();

	this->m_Window.Clear();

	for (auto it = sprites.begin(); it != sprites.end(); ++it)
	{
		spe::Sprite* sprite = *it;

		this->m_Window.Draw(sprite);
	}
	const float moveSpeed = 500.0f;

	if (spe::Input::onKeyHold(spe::KeyBoardCode::W))
	{
		spe::Vector2 pos = this->m_GUIRepository.Camera.Transform.GetPosition();
		this->m_GUIRepository.Camera.Transform.SetPosition(spe::Vector2(0, 0));
	}

	// Check if the 'A' key is held
	if (spe::Input::onKeyHold(spe::KeyBoardCode::A))
	{
		spe::Vector2 pos = this->m_GUIRepository.Camera.Transform.GetPosition();
		this->m_GUIRepository.Camera.Transform.SetPosition(spe::Vector2(pos.x - moveSpeed * spe::Time::s_delta_time, pos.y));
	}

	// Check if the 'S' key is held
	if (spe::Input::onKeyHold(spe::KeyBoardCode::S))
	{
		spe::Vector2 pos = this->m_GUIRepository.Camera.Transform.GetPosition();
		this->m_GUIRepository.Camera.Transform.SetPosition(spe::Vector2(pos.x, pos.y - moveSpeed * spe::Time::s_delta_time));
	}

	// Check if the 'D' key is held
	if (spe::Input::onKeyHold(spe::KeyBoardCode::D))
	{
		spe::Vector2 pos = this->m_GUIRepository.Camera.Transform.GetPosition();
		this->m_GUIRepository.Camera.Transform.SetPosition(spe::Vector2(pos.x + moveSpeed * spe::Time::s_delta_time, pos.y));
	}

	this->m_Window.Display();
}

// Public

void spe::Editor::Update()
{
	spe::Time::update();
	this->UpdateComponents();
	this->m_GUIRepository.Camera.Update();
}

