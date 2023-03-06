#include "gameEngine.h"

//constructor

s2d::GameEngine::GameEngine()
{
    this->ptr_renderWindow = new sf::RenderWindow(sf::VideoMode(1920, 1080), "SpriteEngine", sf::Style::Default);
    this->windowEvent.type = sf::Event::GainedFocus;
    this->m_renderer = s2d::Renderer(this->ptr_renderWindow, &this->m_UIWindow.getInspector().backgroundColor);

    auto desktop = sf::VideoMode::getDesktopMode();
    this->ptr_renderWindow->setPosition(sf::Vector2i(desktop.width / 2 - this->ptr_renderWindow->getSize().x / 2, 0));
    this->m_isWindowFullScreen = false;

    this->m_UIRealTimeEditor = s2d::UIRealTimeEditor(*ptr_renderWindow, &this->windowEvent, &this->m_UIWindow.isAnyUIWindowHovered, 
        &this->m_UIWindow.getInspector().state, &this->event, &this->m_UIWindow.getTools().editorTools);

    //Setting other classes
    s2d::Sprite::initActiveSprites();
    s2d::Input::setEvent(&this->event);
    s2d::SpriteData::highestSpriteID = s2d::SpriteData::getHighestIndex();
    s2d::UI::setRenderWindow(this->ptr_renderWindow);
    s2d::UI::setS2DEvent(&this->event);
    //End

    ImGui::SFML::Init(*this->ptr_renderWindow);

    this->ptr_renderWindow->setKeyRepeatEnabled(false);
}

s2d::GameEngine::~GameEngine()
{
    for (s2d::Sprite* sprite : s2d::Sprite::activeSprites)
    {
        delete sprite;
    }

    delete this->ptr_renderWindow;

    ImGui::SFML::Shutdown();
}

//private functions

void s2d::GameEngine::pollEngineEvents()
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

void s2d::GameEngine::pollEvents()
{
    bool eventChanged = false;
    while (this->ptr_renderWindow->pollEvent(this->windowEvent))
    {
        ImGui::SFML::ProcessEvent(this->windowEvent);
        if (this->windowEvent.type == sf::Event::Closed)
        {
            //Stopping all animations so there wont be saved the current path of the sprite
            s2d::Animator::stopAllAnimations();

            //We need to save our data | Dont forget to save it also in "UIToolButtons"
            s2d::flc::createSaveFile(s2d::Sprite::activeSprites);
            s2d::flc::createWindowBackgroundSaveFile(this->m_UIWindow.getInspector().backgroundColor);
            s2d::flc::createCameraSaveFile(*s2d::GameObject::ptr_camera_tRealTimeEditor);
            s2d::flc::createIndexSaveFile();
            s2d::flc::createAnimtionSaveFile();
            // Known projects file gets created in project selector

            this->ptr_renderWindow->close();
        }
        if (!eventChanged)
        {
            if (this->windowEvent.type == sf::Event::KeyReleased)
            {
                event.type = s2d::Event::KeyReleased;
                eventChanged = true;
            }
            else if (this->windowEvent.type == sf::Event::KeyPressed)
            {
                eventChanged = true;
                event.type = s2d::Event::KeyPressed;
            }
            else if (this->windowEvent.type == sf::Event::MouseButtonPressed)
            {      
                if (this->windowEvent.mouseButton.button == sf::Mouse::Left)
                {
                    event.type = s2d::Event::MousePressedLeft;
                    eventChanged = true;
                }
            }
            else if (this->windowEvent.type == sf::Event::MouseButtonReleased)
            {
                if (this->windowEvent.mouseButton.button == sf::Mouse::Left)
                {
                    event.type = s2d::Event::MouseReleasedLeft;
                    eventChanged = true;
                }
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

//public functions

void s2d::GameEngine::update()
{
    //Fullscreen / Not Fullscreen
    this->updateWindowStyle();

    //Renderere / window events
    this->pollEvents();

    //UIWindow (Engine)
    ImGui::PushFont(s2d::FontManager::defaultFont);
    this->m_UIWindow.update();
    this->m_UIRealTimeEditor.update();
    ImGui::PopFont();

    s2d::Animation::updateAllAnimations();

    //Engine event
    this->pollEngineEvents();

    this->m_renderer.render();

    //Other classes
    s2d::Time::update();

}