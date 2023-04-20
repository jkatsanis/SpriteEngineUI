#include "UIRealTimeEditorNavigator.h"


//Constructor

s2d::UIRealTimeEditorNavigator::UIRealTimeEditorNavigator()
{
	this->m_vecPos = -1;
	this->m_isAnyUIWindowHovered = nullptr;
	this->m_ptr_event_engine = nullptr;
	this->m_ptr_renderWindow = nullptr;
	this->m_arrowSpeed = 0.0f;
	this->m_scrollSpeed = 0.0f;
}

s2d::UIRealTimeEditorNavigator::UIRealTimeEditorNavigator(sf::RenderWindow& window, sf::Event* event, bool* isAnyUIWindowHovered)
{
	this->m_ptr_renderWindow = &window;
	this->m_camera = s2d::Camera(&window);
	this->m_isAnyUIWindowHovered = isAnyUIWindowHovered;
	this->m_ptr_event_engine = event;
	this->m_arrowSpeed = 400;
	this->m_scrollSpeed = 0.15f;

	s2d::GameObject::rects.push_back(this->m_windowRectangle);
	m_windowRectangle_texture.loadFromFile(PATH_TO_RESSOURCS"/Sprites/transparent.png");
	this->m_vecPos = int(s2d::GameObject::rects.size()) - 1;
	s2d::GameObject::rects[this->m_vecPos].setSize(sf::Vector2f(1920, 1080));
	s2d::GameObject::rects[this->m_vecPos].setOutlineColor(sf::Color(255, 255, 255));
	s2d::GameObject::rects[this->m_vecPos].setOutlineThickness(4.0f);
	s2d::GameObject::rects[this->m_vecPos].setPosition(sf::Vector2f(0, 0));

	//Loading camera settings from file
	this->loadCameraSettingsFromFile();
}

//Public functions

void s2d::UIRealTimeEditorNavigator::update()
{
	this->setChangedPosition();
	this->setWhiteBox();

	//Camera update (updating every frame)
	this->m_camera.update();
	s2d::GameObject::ptr_camera_tRealTimeEditor = &this->m_camera;

	if (*this->m_isAnyUIWindowHovered) return;

	this->navigateRightClick();
	this->navigateArrows();
	this->navigateScrollWheel();

}

//Private functions

void s2d::UIRealTimeEditorNavigator::navigateRightClick()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(*this->m_ptr_renderWindow);

		if (this->m_cursor.position_changed)
		{
			s2d::Vector2 moved = this->m_cursor.lastPos - this->m_cursor.position;

			this->m_camera.transform.position += moved;
		}
	}
}

void s2d::UIRealTimeEditorNavigator::navigateScrollWheel()
{
	if (this->m_ptr_event_engine->type == sf::Event::MouseWheelScrolled)
	{
		this->m_ptr_event_engine->type = sf::Event::GainedFocus;

		if (this->m_ptr_event_engine->mouseWheel.x < 0)
		{
			if (this->m_camera.cameraZoom + this->m_scrollSpeed < 4)
			{
				this->m_camera.cameraZoom += this->m_scrollSpeed;
			}
		}
		else
		{
			if (this->m_camera.cameraZoom - this->m_scrollSpeed > 0.04)
			{
				this->m_camera.cameraZoom -= this->m_scrollSpeed;
			}
		}
	}
}

void s2d::UIRealTimeEditorNavigator::calculateScrollWheelSpeed()
{
	sf::Vector2f size = this->m_camera.cameraView.getSize();

	if (size.x > 1500)
	{
		this->m_scrollSpeed = 0.006f;
	}
	else if (size.x > 1000)
	{
		this->m_scrollSpeed = 0.005f;
	}
	else if (size.x > 750)
	{
		this->m_scrollSpeed = 0.004f;
	}
	else
	{
		this->m_scrollSpeed = 0.0008f;
	}
}

void s2d::UIRealTimeEditorNavigator::loadCameraSettingsFromFile()
{
	std::fstream cameraFile;

	//opening the file where all sprite data is
	cameraFile.open(PATH_TO_CAMERA_FILE, std::ios::in);
	if (cameraFile.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(cameraFile, line))
		{
			cnt++;
			//First line is the header so we dont need to check for it
			if (cnt == 1)
			{
				continue;
			}

			//Splitting line
			std::string delimiter = ";";
			std::vector<std::string> propertys = std::splitString(line, delimiter);

			//INITIIALIZING PROPS
			this->m_camera.transform.position.x = std::stof(propertys[0].c_str());
			this->m_camera.transform.position.y = std::stof(propertys[1].c_str());
			this->m_camera.cameraZoom = std::stof(propertys[2].c_str());
		}
		cameraFile.close();
	}
}

void s2d::UIRealTimeEditorNavigator::setChangedPosition()
{
	this->m_cursor.position = s2d::Vector2(float(sf::Mouse::getPosition(*this->m_ptr_renderWindow).x), float(sf::Mouse::getPosition(*this->m_ptr_renderWindow).y));

	this->m_cursor.setLastPosition();
}

void s2d::UIRealTimeEditorNavigator::setWhiteBox()
{
	s2d::GameObject::rects[this->m_vecPos].setTexture(&this->m_windowRectangle_texture);
}


void s2d::UIRealTimeEditorNavigator::navigateArrows()
{
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::Right))
	{
		this->m_camera.transform.position.x += this->m_arrowSpeed * s2d::Time::deltaTime;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::Left))
	{
		this->m_camera.transform.position.x -= this->m_arrowSpeed * s2d::Time::deltaTime;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::Up))
	{
		this->m_camera.transform.position.y -= this->m_arrowSpeed * s2d::Time::deltaTime;
	}
	if (s2d::Input::onKeyHold(s2d::KeyBoardCode::Down))
	{
		this->m_camera.transform.position.y += this->m_arrowSpeed * s2d::Time::deltaTime;
	}
}