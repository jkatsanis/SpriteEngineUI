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
		if (sprite->transform.position != sprite->transform.next_pos)
		{
#ifdef CHILDSYSTEM
			Transform::onPositionChange(sprite);
#endif
		}
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
#ifdef COLLISION
		s2d::BoxCollider::checkCollisions(this->m_sprite_repository);
#endif
		this->updateWindowStyle();
		this->updateUserScriptsAndGUI();

#ifdef ANIMATION
		s2d::Animation::updateAllAnimations(this->m_sprite_repository);
#endif

#ifdef PHYSICS
		s2d::Physics::update(this->m_sprite_repository);
#endif

#ifdef CAMERA
		s2d::GameObject::camera.update();
#endif
	}

	//Engine event
	this->pollEngineEvents();

	this->m_renderer.render();
}

void s2d::GameEngine::start()
{	
	//Engine 
	s2d::Initializer::initIds(this->m_sprite_repository.highestSpriteId);
	s2d::Initializer::initSprites(this->m_sprite_repository);
	s2d::Initializer::initAnimations(this->m_sprite_repository);
	s2d::Input::setEvent(&this->event);
	s2d::FileData::setWindowBackground();

	//Engine 
	this->windowEvent.type = sf::Event::GainedFocus;
	this->ptr_render_window = new sf::RenderWindow(sf::VideoMode(1920, 1080), s2d::GameData::name, sf::Style::Default);

	s2d::GameObject::camera = s2d::Camera(this->ptr_render_window);

	this->ptr_render_window->setKeyRepeatEnabled(false);

	ImGui::SFML::Init(*this->ptr_render_window);

	this->m_renderer = s2d::Renderer(this->ptr_render_window);
	this->m_renderer.setSpriteRepository(this->m_sprite_repository);

	this->m_game.config.ptr_sprites = &this->m_sprite_repository;
	
	// user code
	this->m_game.start();
}

