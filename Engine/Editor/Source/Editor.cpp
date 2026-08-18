#include "Editor.h"

// Ctor / dctor
 
spe::Editor::Editor()
{
	this->m_Window = spe::GameWindow(spe::Vector2(1920, 1080), "SpriteEngine");
	
	// Load the image for the window icon
	sf::Image icon;
	if (!icon.loadFromFile(std::string("Editor") + PATH_SYMBOL + "Ressources" + PATH_SYMBOL + "Icons" + PATH_SYMBOL + "icon.png")) {
		spe::Log::LogString("Couldnt load icon!!");
	}

	// Set the window icon
	this->m_Window.GetRenderWindow()->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	// Loading the sprites from the user directory will be permaent here!
	spe::Utility::SetCurrentDir(spe::EngineData::s_PathUserProject);
	this->m_SceneHandler.Init(PATH_TO_LIGHT_SHADER);


	// Sprite/Background/Camera
	this->Init();

	this->m_Window.SetCamera(&this->m_GUIRepository.Camera);
	this->m_Window.SetBackgroundColor(&this->m_GUIRepository.BackgroundColor);

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

	spe::BoxCollider::InitCameraCollider(this->m_SceneHandler.LightRepository);
}

spe::Editor::~Editor()
{
	spe::BoxCollider::DeleteCameraCollider();
	this->m_Window.Shutdown();
	this->m_SceneHandler.SpriteRepository.CleanUp();
}

// Private

void spe::Editor::Init()
{
	spe::Initializer::InitTags(this->m_SceneHandler.SpriteRepository, PATH_TO_TAG_FILE);
	spe::Initializer::InitScenes(this->m_SceneHandler, PATH_TO_SCENE_FILE);
	spe::Initializer::IntiHighestSpriteID(this->m_SceneHandler.SpriteRepository, PATH_TO_HIGHEST_INDEX);

	// Load the first scene in the file
	this->m_SceneHandler.LoadScene(this->m_SceneHandler.TotalScenes[0], this->m_GUIRepository.Camera, this->m_GUIRepository.BackgroundColor);
}

void spe::Editor::UpdateUI()
{
	ImGui::PushFont(spe::Style::s_DefaultFont);
	spe::UIUtility::UpdateCursor();
	this->m_UIWindow.Update();

	if (this->m_Window.ContainsCursor() && this->m_Window.GetRenderWindow()->hasFocus())
	{
		this->m_UIRealTimeEditor.Update();
	}

	ImGui::PopFont();
}

void spe::Editor::UpdateComponents()
{
	this->m_Window.PollEvents();

	this->UpdateUI();

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
    this->m_GUIRepository.Camera.Update(&this->m_SceneHandler.LightRepository);

	for (auto it = sprites.begin(); it != sprites.end(); ++it)
	{
		spe::Sprite* sprite = *it;

		if (!spe::BoxCollider::ProcessSprite(sprite, this->m_GUIRepository.Camera))
		{
			sprite->DisableProcess();
			continue;
		} 
		sprite->EnableProcess();

		this->m_SceneHandler.LightRepository.UpdateLightSource(sprite, &this->m_GUIRepository.Camera);
		sprite->Animator.Update();
		sprite->Particles.Update();

		if (this->m_GUIRepository.SimulatePhysics)
		{
			sprite->Collider.Update(this->m_SceneHandler.SpriteRepository);
			sprite->Physicsbody.Update();
		}

		this->m_Window.DrawEngine(sprite, &this->m_SceneHandler.LightRepository.GetShader(), this->m_GUIRepository.RenderAlwaysWithoutLight);
		this->m_Window.DrawParticles(sprite, &this->m_SceneHandler.LightRepository.GetShader(), this->m_GUIRepository.RenderAlwaysWithoutLight);
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
    // Removed m_GUIRepository.Camera.Update from here
}
