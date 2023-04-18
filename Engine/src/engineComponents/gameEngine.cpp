#include "gameEngine.h"

//constructor

s2d::GameEngine::GameEngine()
{
     this->m_UIWindow.init(this->m_spriteRepository);
    this->m_close = false;
    this->ptr_renderWindow = new sf::RenderWindow(sf::VideoMode(1920, 1080), "SpriteEngine", sf::Style::Default);
    this->windowEvent.type = sf::Event::GainedFocus;
    this->m_renderer = s2d::Renderer(this->ptr_renderWindow, &this->m_UIWindow.getInspector().backgroundColor, this->m_spriteRepository);

    auto desktop = sf::VideoMode::getDesktopMode();
    this->ptr_renderWindow->setPosition(sf::Vector2i(desktop.width / 2 - this->ptr_renderWindow->getSize().x / 2, 0));
    this->m_isWindowFullScreen = false;

    this->m_UIRealTimeEditor = s2d::UIRealTimeEditor(*ptr_renderWindow, &this->windowEvent, &this->m_UIWindow.areAnyUIWindowsHovered,
        &this->m_UIWindow.getInspector().state, &this->event, this->m_spriteRepository);
    
    //Setting other classes
    s2d::Initializer::initSprites(this->m_spriteRepository);
    s2d::Initializer::initAnimations(this->m_spriteRepository);
    s2d::Initializer::initBackground(this->m_UIWindow.getInspector().backgroundColor);
    s2d::Input::setEvent(&this->event);
    s2d::Initializer::initIds(this->m_spriteRepository.highestSpriteId);
    s2d::UI::setRenderWindow(this->ptr_renderWindow);
    s2d::UI::setS2DEvent(&this->event);
    //End

    ImGui::SFML::Init(*this->ptr_renderWindow);

    this->ptr_renderWindow->setKeyRepeatEnabled(false);

    this->m_spriteRepository.isFullScreened = &this->m_isWindowFullScreen;
}

s2d::GameEngine::~GameEngine()
{
    delete this->ptr_renderWindow;

    ImGui::SFML::Shutdown();
}

//private functions

void s2d::GameEngine::pollEngineEvents()
{
    for (int i = 0; i < this->m_spriteRepository.amount(); i++)
    {
        s2d::Sprite* const sprite = this->m_spriteRepository.readAt(i);
        if (sprite->transform.position != sprite->transform.nextPos)
        {
            //Fire on pos event
            Transform::onPositionChange(sprite);
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
            this->m_close = true;
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

void s2d::GameEngine::saveDialoge()
{
    static const s2d::Vector2 SAVE_MENU_SIZE = s2d::Vector2(250, 55);

    if (this->m_close)
    {
        if (ImGui::Begin("Close", NULL,
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar))
        {
            const ImVec2 CURSOR_POS = ImGui::GetCursorPos();
            if (ImGui::Button("Save"))
            {
                s2d::flc::saveEverything(this->m_UIWindow.getInspector().backgroundColor, this->m_spriteRepository);
                this->ptr_renderWindow->close();
                return;
            }
            ImGui::SameLine();
            if (ImGui::Button("Exit"))
            {
                this->ptr_renderWindow->close();
            }
            ImGui::SetCursorPos(ImVec2(CURSOR_POS.x + SAVE_MENU_SIZE.x - 50, CURSOR_POS.y));
            if (ImGui::Button("x"))
            {
                this->m_close = false;
            }

            s2d::UI::setWindowScreenMiddle(SAVE_MENU_SIZE);
            ImGui::End();
        }
    }
}

//public functions

void s2d::GameEngine::update()
{
    // Fullscreen / Not Fullscreen
    this->updateWindowStyle();

    // Renderere / window events
    this->pollEvents();

    // UIWindow (Engine)
    ImGui::PushFont(s2d::FontManager::defaultFont);
    this->m_UIWindow.update();
    this->m_UIRealTimeEditor.update();
    this->saveDialoge();
    ImGui::PopFont();

    s2d::Animation::updateAllAnimations(this->m_spriteRepository);

    // Engine event
    this->pollEngineEvents();

    this->m_renderer.render();

    // Other classes
    s2d::Time::update();
}