#include "Editor.h"

// Ctor / dctor
 
spe::Editor::Editor()
{
	this->m_Window = spe::GameWindow(spe::Vector2(1920, 1080), "SpriteEngine");
	
	// Loading the sprites from the user directory will be permaent here!
	spe::Utility::SetCurrentDir(spe::EngineData::s_PathUserProject);

	this->m_SceneHandler.Init(PATH_TO_LIGHT_SHADER);

	// Sprite/Background/Camera
	this->Init();

	this->m_Window.SetCamera(&this->m_GUIRepository.Camera);
	this->m_Window.SetBackgroundColor(&this->m_GUIRepository.background_color);

	this->m_UIWindow.SetRepos(this->m_GUIRepository, this->m_SceneHandler.SpriteRepository, this->m_SceneHandler, this->m_SceneHandler.LightRepository);

	spe::Style::Init();
	spe::Style::RenderStyle();

	spe::UIUtility::SetEvent(&this->m_Window.Event);
	spe::UIUtility::SetRenderWinodw(this->m_Window.GetRenderWindow());
	spe::Input::SetEvent(&this->m_Window.Event);

	this->m_GUIRepository.ptr_Event = &this->m_Window.Event;
	this->m_GUIRepository.ptr_SFEvent = &this->m_Window.WindowEvent;

	this->m_GUIRepository.InitHierarchySprites(this->m_SceneHandler.SpriteRepository.GetSprites());
	this->m_SceneHandler.SpriteRepository.SortSpritesByLayer();

	this->m_UIRealTimeEditor = spe::UIRealTimeEditor(&this->m_Window, this->m_SceneHandler.SpriteRepository, this->m_GUIRepository);

	spe::Log::LogString("=============");
	spe::Log::LogString("Finished init...");
	spe::Log::LogString("Building the project..");
	spe::EngineData::BuildProjectFiles();
}

spe::Editor::~Editor()
{
	this->m_Window.Shutdown();
	this->m_SceneHandler.SpriteRepository.cleanUp();
}

// Private

void spe::Editor::Init()
{
	spe::Initializer::InitTags(this->m_SceneHandler.SpriteRepository, PATH_TO_TAG_FILE);
	spe::Initializer::InitScenes(this->m_SceneHandler, PATH_TO_SCENE_FILE);
	spe::Initializer::IntiHighestSpriteID(this->m_SceneHandler.SpriteRepository, PATH_TO_HIGHEST_INDEX);

	// Load the first scene in the file
	this->m_SceneHandler.LoadScene(this->m_SceneHandler.TotalScenes[0], this->m_GUIRepository.Camera, this->m_GUIRepository.background_color);
}

void spe::Editor::UpdateUI()
{
	ImGui::PushFont(spe::Style::s_DefaultFont);
	spe::UIUtility::UpdateCursor();
	this->m_UIWindow.Update();

	if (this->m_Window.ContainsCursor())
	{
		this->m_UIRealTimeEditor.update();
	}

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

		sprite->Animator.Update();

		if (this->m_GUIRepository.SimulatePhysics)
		{
			sprite->Collider.Update(this->m_SceneHandler.SpriteRepository);
			sprite->Physicsbody.Update();
		}

		this->m_SceneHandler.LightRepository.UpdateLightSource(sprite, &this->m_GUIRepository.Camera);

		this->m_Window.Draw(sprite, &this->m_SceneHandler.LightRepository.GetShader());
	}
	this->m_SceneHandler.LightRepository.UpdateArrays();

	this->m_GUIRepository.Render(this->m_Window.GetRenderWindow());
	this->m_Window.Display();
}

// Public

void spe::Editor::Update()
{
	spe::Time::Update();
	this->UpdateComponents();
	this->m_GUIRepository.Camera.Update(&this->m_SceneHandler.LightRepository);
}

