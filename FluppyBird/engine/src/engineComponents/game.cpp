#include "game.h"

s2d::Game::Game()
{
	this->m_timePassed = 2.0f;
	this->windowEvent.type = sf::Event::GainedFocus;
	this->ptr_renderWindow = nullptr;
}

s2d::Game::~Game()
{
	for (s2d::Sprite* sprite : s2d::Sprite::activeSprites)
	{
		delete sprite;
	}
}


bool s2d::Game::isGameRunning()
{
	return this->ptr_renderWindow->isOpen();
}

void s2d::Game::pollEngineEvents()
{
	for (s2d::Sprite* ptr_sprite : s2d::Sprite::activeSprites)
	{
		if (ptr_sprite->transform.position != ptr_sprite->transform.nextPos)
		{
			//Fire on pos event
			Transform::onPositionChange(ptr_sprite);
		}
	}
}


void s2d::Game::pollEvents()
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

void s2d::Game::render()
{
	float x = s2d::FileData::windowBackground.x;
	float y = s2d::FileData::windowBackground.y;
	float z = s2d::FileData::windowBackground.z;

	this->ptr_renderWindow->clear(sf::Color(sf::Uint8(x), sf::Uint8(y), sf::Uint8(z)));
	this->draw();
	this->ptr_renderWindow->display();
}

void s2d::Game::updateUserScriptsAndGUI()
{
	s2d::GUIManager::update();
	ImGui::Begin("##MainWindow", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

	//User update!
	testing.update();

	ImGui::SetWindowSize(ImVec2(1920, 1080));
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowFontScale(s2d::GUIManager::s_fontSize);

	ImGui::End();

}

void s2d::Game::draw()
{
	this->drawSprites();    
	ImGui::SFML::Render(*this->ptr_renderWindow);
}

void s2d::Game::drawSprites()
{  
	//2s passed we can update out hightest layer index
	if (m_timePassed > 2)
	{
		s2d::Sprite::updateHightestLayerIndex();
		m_timePassed = 0;
	}
	for (int i = 0; i < s2d::Sprite::highteLayerIndex + 1; i++)
	{
		for (s2d::Sprite* sprite : s2d::Sprite::activeSprites)
		{
			if (sprite->sortingLayerIndex == i)
			{
				sprite->setSpritePosition();
				this->ptr_renderWindow->draw(sprite->getSprite());
			}
		}
	}
}

void s2d::Game::update()
{
	//Engine update
	s2d::Time::update();

	this->m_timePassed += s2d::Time::deltaTime;

	//Renderer / Events
	this->pollEvents();

	// Loading everything for 1s
	if (s2d::Time::timePassed > 1)
	{
		this->updateUserScriptsAndGUI();

		s2d::BoxCollider::checkCollisions();
		s2d::Physics::update();

		s2d::GameObject::camera.update();
	}

	//Engine event
	this->pollEngineEvents();
	this->render();
}

void s2d::Game::start()
{	
	//Engine 
	s2d::Sprite::initActiveSprites();
	s2d::Input::setEvent(&this->event);
	s2d::FileData::setWindowBackground();

	//User start!
	testing.start();

	//Engine 
	this->windowEvent.type = sf::Event::GainedFocus;
	this->ptr_renderWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(1920, 1080), s2d::GameData::name, sf::Style::Default);

	s2d::GameObject::camera = s2d::Camera(this->ptr_renderWindow.get());

	this->ptr_renderWindow->setKeyRepeatEnabled(false);

	ImGui::SFML::Init(*this->ptr_renderWindow);
}

