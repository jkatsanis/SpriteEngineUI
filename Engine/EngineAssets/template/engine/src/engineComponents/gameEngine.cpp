#include "gameEngine.h"

s2d::GameEngine::GameEngine()
{
	this->m_timePassed = 2.0f;
	this->windowEvent.type = sf::Event::GainedFocus;
	this->ptr_renderWindow = nullptr;
	this->m_isWindowFullScreen = false;
}

s2d::GameEngine::~GameEngine()
{
	for (s2d::Sprite* sprite : s2d::Sprite::activeSprites)
	{
		delete sprite;
	}

	delete this->ptr_renderWindow;
}


bool s2d::GameEngine::isGameRunning()
{
	return this->ptr_renderWindow->isOpen();
}

void s2d::GameEngine::pollEngineEvents()
{
	for (s2d::Sprite* ptr_sprite : s2d::Sprite::activeSprites)
	{
		if (ptr_sprite->transform.position != ptr_sprite->transform.nextPos)
		{
			//Fire on pos event
#ifdef CHILDSYSTEM
			Transform::onPositionChange(ptr_sprite);
#endif
		}
	}
}

void s2d::GameEngine::pollEvents()
{
	bool eventChanged = false;

	while (this->ptr_renderWindow->pollEvent(this->windowEvent))
	{
		ImGui::SFML::ProcessEvent(this->windowEvent);

		if (this->windowEvent.type == sf::Event::Closed)
		{
			this->ptr_renderWindow->close();
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
	ImGui::SFML::Update(*ptr_renderWindow, Time::deltaClock.restart());
}

void s2d::GameEngine::updateUserScriptsAndGUI()
{
#ifdef GUI
	s2d::GUIManager::update();
	ImGui::Begin("##MainWindow", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
#endif

	//User update!
	game.update();

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
			this->ptr_renderWindow->create(sf::VideoMode(1920, 1080), "SpriteEngine", sf::Style::Fullscreen);
		}
	}
	else if (m_isWindowFullScreen)
	{
		if (s2d::Input::onKeyRelease(s2d::KeyBoardCode::F11))
		{
			this->m_isWindowFullScreen = false;
			this->ptr_renderWindow->create(sf::VideoMode(1920, 1080), "SpriteEngine", sf::Style::Default);
		}
	}
	if (Input::onKeyHold(s2d::KeyBoardCode::LControl) && Input::onKeyRelease(s2d::KeyBoardCode::F4))
	{
		this->ptr_renderWindow->close();
	}
}

// Public functions

void s2d::GameEngine::update()
{
	//Engine update
	s2d::Time::update();

	this->m_timePassed += s2d::Time::deltaTime;

	//Renderer / Events
	this->pollEvents();

	// Loading everything for 1s
	if (s2d::Time::timePassed > 2.5f)
	{
#ifdef COLLISION
		s2d::BoxCollider::checkCollisions();
#endif
		this->updateWindowStyle();
		this->updateUserScriptsAndGUI();

#ifdef PHYSICS
		s2d::Physics::update();
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
	s2d::Sprite::initActiveSprites();
	s2d::Input::setEvent(&this->event);
	s2d::FileData::setWindowBackground();

	//User start!
	game.start();

	//Engine 
	this->windowEvent.type = sf::Event::GainedFocus;
	this->ptr_renderWindow = new sf::RenderWindow(sf::VideoMode(1920, 1080), s2d::GameData::name, sf::Style::Default);

	s2d::GameObject::camera = s2d::Camera(this->ptr_renderWindow);

	this->ptr_renderWindow->setKeyRepeatEnabled(false);

	ImGui::SFML::Init(*this->ptr_renderWindow);

	this->m_renderer = s2d::Renderer(this->ptr_renderWindow);
}

