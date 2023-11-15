#include "gameEngine.h"

//constructor

s2d::GameEngine::GameEngine()
{
    s2d::LightRepository::init();

    this->m_ui_window.init(this->m_sprite_repository, &this->event, &this->windowEvent, this->m_scene_names);
    this->m_close = false;
    this->ptr_render_window = new sf::RenderWindow(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "SpriteEngine", sf::Style::Default);
    this->windowEvent.type = sf::Event::GainedFocus;
    this->m_renderer = s2d::Renderer(this->ptr_render_window, this->m_sprite_repository, this->m_ui_window.gui_repository);

    auto desktop = sf::VideoMode::getDesktopMode();
    this->ptr_render_window->setPosition(sf::Vector2i(desktop.width / 2 - this->ptr_render_window->getSize().x / 2, 0));
    this->m_is_window_full_screen = false;

    this->m_ui_real_time_editor = s2d::UIRealTimeEditor(*ptr_render_window, &this->windowEvent, &this->m_ui_window.ary_any_windows_hovered,
        &this->m_ui_window.getInspector().state, &this->event, this->m_sprite_repository, this->m_ui_window.gui_repository);
    this->m_ui_window.gui_repository.camera = s2d::Camera(this->ptr_render_window, this->m_sprite_repository);

    this->windowBounds = sf::IntRect(0, 0, this->ptr_render_window->getSize().x, this->ptr_render_window->getSize().y);

    sf::Image icon64;
  
    if (icon64.loadFromFile("ressources/Icons/icon.png"))
    {
        this->ptr_render_window->setIcon(icon64.getSize().x, icon64.getSize().y, icon64.getPixelsPtr());
    }

    //Setting other classes
    s2d::Initializer::initTags(this->m_sprite_repository);
    s2d::Initializer::initScenes(this->m_scene_names);
    this->initOtherClasses();
    s2d::Input::setEvent(&this->event);
    s2d::UI::setRenderWindow(this->ptr_render_window);
    s2d::UI::setS2DEvent(&this->event);
    //End

    ImGui::SFML::Init(*this->ptr_render_window);

    this->ptr_render_window->setKeyRepeatEnabled(false);
    this->m_sprite_repository.isFullScreened = &this->m_is_window_full_screen;
    this->m_current_scene = s2d::EngineData::s_scene;
    this->m_sprite_repository.initialied = true;

    s2d::flc::cleanUp(this->m_sprite_repository, false);
    // m_sprite_repository.readAt(12)->light.enable();
}

s2d::GameEngine::~GameEngine()
{
    delete this->ptr_render_window;

    ImGui::SFML::Shutdown();
}

void s2d::GameEngine::initOtherClasses()
{
    s2d::Initializer::initSprites(this->m_sprite_repository);
    s2d::Initializer::initAnimations(this->m_sprite_repository);
    s2d::Initializer::initBackground(this->m_ui_window.gui_repository.background_color);
    s2d::Initializer::initIds(this->m_sprite_repository.highest_sprite_id);
  
    s2d::Initializer::initCamera(this->m_ui_window.gui_repository);
}

//private functions

void s2d::GameEngine::pollEngineEvents()
{
    for (int i = 0; i < this->m_sprite_repository.amount(); i++)
    {
        s2d::Sprite* const sprite = this->m_sprite_repository.readAt(i);
        if (sprite->transform.position != sprite->transform.next_pos)
        {
            //Fire on pos event
            Transform::onPositionChange(sprite);
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
    ImGui::SFML::Update(*ptr_render_window, Time::s_delta_clock.restart());
}

void s2d::GameEngine::updateWindowStyle()
{
    if (!this->m_is_window_full_screen)
    {
        if (s2d::Input::onKeyRelease(s2d::KeyBoardCode::F11))
        {
            this->m_is_window_full_screen = true;
            this->ptr_render_window->create(sf::VideoMode(1920, 1080), "SpriteEngine", sf::Style::Fullscreen);
        }
    }
    else if (m_is_window_full_screen)
    {
        if (s2d::Input::onKeyRelease(s2d::KeyBoardCode::F11))
        {
            this->m_is_window_full_screen = false;
            this->ptr_render_window->create(sf::VideoMode(1920, 1080), "SpriteEngine", sf::Style::Default);
        }
    }
    if (Input::onKeyHold(s2d::KeyBoardCode::LControl) && Input::onKeyRelease(s2d::KeyBoardCode::F4))
    {
        this->ptr_render_window->close();
    }
}

void s2d::GameEngine::saveDialoge()
{
    static const s2d::Vector2 SAVE_MENU_SIZE = s2d::Vector2(250, 55);

    if (this->m_close)
    {
        s2d::Event::s_handle_other_events = false;
       
        ImGui::SetNextWindowFocus();
        if (ImGui::Begin("Close", NULL,
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar))
        {
            const ImVec2 CURSOR_POS = ImGui::GetCursorPos();
            if (ImGui::Button("Save"))
            {
                this->onEngineClose(true);
                return;
            }
            ImGui::SameLine();
            if (ImGui::Button("Exit"))
            {
                this->onEngineClose(false);
                return;
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
    else
    {
        s2d::Event::s_handle_other_events = true;
    }
}

void s2d::GameEngine::onEngineClose(bool save)
{
    s2d::Event::s_handle_other_events = false;

    // The animations are permanent thats why in cleanUp there are getting saved
    s2d::flc::cleanUp(this->m_sprite_repository, save);
    if (save)
    {
        s2d::flc::saveEverything(this->m_ui_window.gui_repository.background_color, this->m_sprite_repository, this->m_ui_window.gui_repository, this->m_scene_names);
    } 
    this->ptr_render_window->close();
}

void s2d::GameEngine::loadScene(const std::string& scene_name)
{
    s2d::EngineData::s_scene = scene_name;
    this->m_current_scene = scene_name;
    this->clearEngineUpBeforeSceneLoad();
    this->initOtherClasses();
}

void s2d::GameEngine::updateComponents()
{
    for (int i = 0; i < this->m_sprite_repository.amount(); i++)
    {
        s2d::Sprite* const sprite = this->m_sprite_repository.readAt(i);
        sprite->animator.update();
        s2d::LightRepository::updateLightSource(sprite, &this->m_ui_window.gui_repository.camera);
    }
    s2d::LightRepository::updateArrays();
}

void s2d::GameEngine::clearEngineUpBeforeSceneLoad()
{
    this->m_sprite_repository.cleanUp();
}


//public functions

void s2d::GameEngine::update()
{
    // Fullscreen / Not Fullscreen
    this->updateWindowStyle();

    // Renderer / window events
    this->pollEvents();

    // UIWindow (Engine)
    ImGui::PushFont(s2d::FontManager::s_default_font);
    this->m_ui_window.update();

    sf::Vector2i mousePosition = sf::Mouse::getPosition(*this->ptr_render_window);
    if (windowBounds.contains(mousePosition))
    {
        this->m_ui_real_time_editor.update();
    }

    this->saveDialoge();
    ImGui::PopFont();

    this->updateComponents();

    // Engine event
    this->pollEngineEvents();

    this->m_renderer.render();

    // Other classes
    s2d::Time::update();

    if (this->m_current_scene != s2d::EngineData::s_scene)
    {
        this->loadScene(s2d::EngineData::s_scene);
    }
}