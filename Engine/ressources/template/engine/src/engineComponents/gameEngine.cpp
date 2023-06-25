#include "gameEngine.h"

s2d::GameEngine::GameEngine()
{
	this->m_timePassed = 2.0f;
	this->windowEvent.type = sf::Event::GainedFocus;
	this->ptr_render_window = nullptr;
	this->m_isWindowFullScreen = false;
}

s2d::GameEngine::~GameEngine()
{
	delete this->ptr_render_window;
}


bool s2d::GameEngine::isGameRunning()
{
	return this->ptr_render_window->isOpen();
}

void s2d::GameEngine::pollEngineEvents()
{
	for (int i = 0; i < this->m_sprite_repository.amount(); i++)
	{
		s2d::Sprite* const sprite = this->m_sprite_repository.readAt(i);
		
#ifdef CHILDSYSTEM
			Transform::onPositionChange(sprite);
#endif
		
	}
}

void s2d::GameEngine::pollEvents()
{
	bool eventChanged = false;

	while (this->ptr_render_window->pollEvent(this->windowEvent))
	{
		ImGui::SFML::ProcessEvent(this->windowEvent);

		if (this->windowEvent.type == sf::Event::Closed)
		{
			this->ptr_render_window->close();
		}
		if (!eventChanged)
		{
			if (this->windowEvent.type == sf::Event::KeyReleased)
			{
				event.type = s2d::Event::Released;
				eventChanged = true;
			}
			else if (this->windowEvent.type == sf::Event::KeyPressed)
			{
				eventChanged = true;
				event.type = s2d::Event::Pressed;
			}
			else
			{
				eventChanged = true;
				event.type = s2d::Event::None;
			}
			event.key = static_cast<s2d::KeyBoardCode>(static_cast<sf::Keyboard::Key>(this->windowEvent.key.code));
		}
	}
	ImGui::SFML::Update(*ptr_render_window, Time::deltaClock.restart());
}

void s2d::GameEngine::updateUserScriptsAndGUI()
{
#ifdef GUI
	s2d::GUIManager::update();
	ImGui::Begin("##MainWindow", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
#endif

	//User update!
	m_game.update();

#ifdef GUI 
	ImGui::SetWindowSize(ImVec2(1920, 1080));
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowFontScale(s2d::GUIManager::s_fontSize);
	ImGui::End();
#endif
}

void s2d::GameEngine::updateWindowStyle()
{
	if (!m_isWindowFullScreen)
	{
		if (s2d::Input::onKeyRelease(s2d::KeyBoardCode::F11))
		{
			this->m_isWindowFullScreen = true;
			this->ptr_render_window->create(sf::VideoMode(1920, 1080), "SpriteEngine", sf::Style::Fullscreen);
		}
	}
	else if (m_isWindowFullScreen)
	{
		if (s2d::Input::onKeyRelease(s2d::KeyBoardCode::F11))
		{
			this->m_isWindowFullScreen = false;
			this->ptr_render_window->create(sf::VideoMode(1920, 1080), "SpriteEngine", sf::Style::Default);
		}
	}
	if (Input::onKeyHold(s2d::KeyBoardCode::LControl) && Input::onKeyRelease(s2d::KeyBoardCode::F4))
	{
		this->ptr_render_window->close();
	}
}

void s2d::GameEngine::clearEngineUpBeforeSceneLoad()
{
	this->m_sprite_repository.cleanUp();
}

void s2d::GameEngine::loadScene(const std::string& scene_name)
{
	s2d::GameData::s_scene = scene_name;
	this->m_current_scene = scene_name;
	this->clearEngineUpBeforeSceneLoad();
	this->initOtherClasses();
}

// Public functions

void s2d::GameEngine::update()
{
	//Engine update
	s2d::Time::update();

	this->m_timePassed += s2d::Time::s_delta_time;

	//Renderer / Events
	this->pollEvents();

	// Loading everything for 1s
	if (s2d::Time::timePassed > 2.5f)
	{
#ifdef PHYSICS
		s2d::Physics::update(this->m_sprite_repository);
#endif
		this->updateWindowStyle();
		this->updateUserScriptsAndGUI();

#ifdef COLLISION
		s2d::BoxCollider::checkCollisions(this->m_sprite_repository);
#endif

#ifdef ANIMATION
		s2d::Animation::updateAllAnimations(this->m_sprite_repository);
#endif

#ifdef CAMERA
		s2d::GameObject::camera.update();
#endif

#ifdef SCENE
		if (this->m_current_scene != s2d::GameData::s_scene)
		{
			this->loadScene(s2d::GameData::s_scene);
		}
#endif
	}

	//Engine event
	this->pollEngineEvents();

	this->m_renderer.render();
}

void s2d::GameEngine::start()
{	
	//Engine 
	this->windowEvent.type = sf::Event::GainedFocus;
	this->ptr_render_window = new sf::RenderWindow(sf::VideoMode(1920, 1080), s2d::GameData::name, sf::Style::Default);

	s2d::GameObject::camera = s2d::Camera(this->ptr_render_window);

	this->ptr_render_window->setKeyRepeatEnabled(false);

	ImGui::SFML::Init(*this->ptr_render_window);

	this->m_renderer = s2d::Renderer(this->ptr_render_window);
	this->m_renderer.setSpriteRepository(this->m_sprite_repository);

	this->m_game.config.ptr_sprites = &this->m_sprite_repository;

	//Engine 
	s2d::Initializer::initScenes(this->m_scene_names);;
	this->loadScene(s2d::GameData::s_scene);
	s2d::Initializer::loadPrefabsInMemory();
	s2d::Input::setEvent(&this->event);

	this->m_current_scene = s2d::GameData::s_scene;
	this->m_game.config.setGameEngine(this);
	this->m_sprite_repository.main_content_iniitialied = true;

	// user code
	this->m_game.start();
}

void s2d::GameEngine::initOtherClasses()
{
	s2d::Initializer::initBackground(this->m_renderer.background_color);
	s2d::Initializer::initIds(this->m_sprite_repository.highest_sprite_id);
	s2d::Initializer::initSprites(this->m_sprite_repository);
	s2d::Initializer::initAnimations(this->m_sprite_repository);
}