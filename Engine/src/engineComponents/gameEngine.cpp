#include "gameEngine.h"

//constructor

s2d::GameEngine::GameEngine()
{
    this->m_timeToUpdateTextures = 2;
    this->m_timePassedSinceLasTextureUpdate = 0;
    this->m_timePassed = 2.0f;
    this->ptr_renderWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(1920, 1080), "SpriteEngine", sf::Style::Default);
    this->windowEvent.type = sf::Event::GainedFocus;

    auto desktop = sf::VideoMode::getDesktopMode();
    this->ptr_renderWindow->setPosition(sf::Vector2i(desktop.width / 2 - this->ptr_renderWindow->getSize().x / 2, 0));
    this->m_isWindowFullScreen = false;

    this->m_UIRealTimeEditor = s2d::UIRealTimeEditor(*ptr_renderWindow, &this->windowEvent, &this->m_UIWindow.isAnyUIWindowHovered, 
        &this->m_UIWindow.getInspector().state, &this->event);

    //Setting other classes
    s2d::Sprite::initActiveSprites();
    s2d::Input::setEvent(&this->event);
    s2d::SpriteData::highestSpriteID = s2d::SpriteData::getHighestIndex();
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
            s2d::flc::createKnownProjectDirFile();

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

void s2d::GameEngine::draw()
{
    this->drawSprites();
    this->drawRectangles();
    this->drawLines();

    ImGui::SFML::Render(*this->ptr_renderWindow);
}

void s2d::GameEngine::drawSprites()
{
    //2s passed we can update out hightest layer index
    if (m_timePassed > 2.0f)
    {
        s2d::Sprite::updateHightestLayerIndex();
        m_timePassed = 0;
    }

    if (this->m_timePassedSinceLasTextureUpdate > this->m_timeToUpdateTextures)
    {
        this->m_timePassedSinceLasTextureUpdate = 0;
        this->updateSriteTextures();
    }
    for (int i = 0; i < s2d::Sprite::highestLayerIndex + 1; i++)
    {
        for (s2d::Sprite* ptr_activeSprites : s2d::Sprite::activeSprites)
        {
            if (ptr_activeSprites->sortingLayerIndex == i)
            {
                ptr_activeSprites->updateTransformPosition();
                this->ptr_renderWindow->draw(ptr_activeSprites->getSprite());
            }
        }
    }
}

void s2d::GameEngine::updateSriteTextures()
{
    for (int i = 0; i < s2d::Sprite::activeSprites.size(); i++)
    {
        s2d::Sprite::activeSprites[i]->setSpriteTexture(s2d::Sprite::activeSprites[i]->path);
    }
}

void s2d::GameEngine::drawLines()
{
    for (int i = 0; i < s2d::GameObject::lines.size(); i++)
    {
        this->ptr_renderWindow->draw(s2d::GameObject::lines[i].line, 2, sf::Lines);
    }
}

void s2d::GameEngine::drawRectangles()
{
    for (sf::RectangleShape shape : s2d::GameObject::rects)
    {
        this->ptr_renderWindow->draw(shape);
    }
}

void s2d::GameEngine::updateWindowStyle()
{
    if (s2d::Input::onKeyRelease(s2d::KeyBoardCode::F11) && this->m_isWindowFullScreen)
    {
        this->m_isWindowFullScreen = false;
        this->ptr_renderWindow->create(sf::VideoMode(1920, 1080), "SpriteEngine", sf::Style::Default);
    }
    if (s2d::Input::onKeyRelease(s2d::KeyBoardCode::F10) && !this->m_isWindowFullScreen)
    {
        this->m_isWindowFullScreen = true;
        this->ptr_renderWindow->create(sf::VideoMode(1920, 1080), "SpriteEngine", sf::Style::Fullscreen);
    }
    if (Input::onKeyHold(s2d::KeyBoardCode::LControl) && Input::onKeyRelease(s2d::KeyBoardCode::F4))
    {
        this->ptr_renderWindow->close();
    }
}

void s2d::GameEngine::render()
{
    this->ptr_renderWindow->clear(sf::Color(sf::Uint8(this->m_UIWindow.getInspector().backgroundColor.x), sf::Uint8(this->m_UIWindow.getInspector().backgroundColor.y), sf::Uint8(this->m_UIWindow.getInspector().backgroundColor.z)));
    this->draw();
    this->ptr_renderWindow->display();
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

    this->render();

    //Other classes
    s2d::Time::update();

    m_timePassed += s2d::Time::deltaTime;
    this->m_timePassedSinceLasTextureUpdate += s2d::Time::deltaTime;

}