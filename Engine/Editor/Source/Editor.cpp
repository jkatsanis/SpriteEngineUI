#include "Editor.h"

// Ctor / dctor
 
spe::Editor::Editor()
{
	this->m_Window = spe::GameWindow(spe::Vector2(1920, 1080), "SpriteEngine");

	// Sprite/Background/Camera
	this->Init();

	this->m_Window.SetCamera(&this->m_GUIRepository.camera);
	this->m_Window.SetBackgroundColor(&this->m_GUIRepository.background_color);
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
	this->m_SceneHandler.LoadScene(this->m_SceneHandler.TotalScenes[0], this->m_GUIRepository.camera, this->m_GUIRepository.background_color);

	// Resetting the directory after loading the user-data
	spe::Utility::SetCurrentDir(path);
	std::cout << spe::Utility::GetCurrentDir();
}

void spe::Editor::UpdateComponents()
{
	this->m_Window.PollEvents();
	std::list<spe::Sprite*>& sprites = this->m_SceneHandler.SpriteRepository.GetSprites();

	this->m_Window.Clear();

	for (auto it = sprites.begin(); it != sprites.end(); ++it)
	{
		spe::Sprite* sprite = *it;
		this->m_Window.Draw(sprite);
	}

	this->m_Window.Display();
}

// Public

void spe::Editor::Update()
{
	this->UpdateComponents();
}

