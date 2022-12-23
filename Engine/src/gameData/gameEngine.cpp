#include <gameEngine.h>
#include <iconcpp.h>

//constructor

s2d::GameEngine::GameEngine()
{
    this->m_timePassed = 2.0f;
    this->ptr_renderWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(1920, 1080), "SpriteEngine", sf::Style::Default);
    this->windowEvent.type = sf::Event::GainedFocus;

    auto desktop = sf::VideoMode::getDesktopMode();
    this->ptr_renderWindow->setPosition(sf::Vector2i(desktop.width / 2 - this->ptr_renderWindow->getSize().x / 2, 0));
    this->m_isWindowFullScreen = false;

    this->m_UIRealTimeEditor = s2d::UIRealTimeEditor(*ptr_renderWindow, &this->windowEvent, &this->m_UIWindow.isAnyUIWindowHovered);

    //Setting other classes
    s2d::Sprite::initActiveSprites();
    s2d::Input::setEvent(&this->event);
    //End

    this->ptr_renderWindow->setKeyRepeatEnabled(false);

    ImGui::SFML::Init(*this->ptr_renderWindow);
    this->initFonts();
}

s2d::GameEngine::~GameEngine()
{
    for (s2d::Sprite* sprite : s2d::Sprite::activeSprites)
    {
        delete sprite;
    }
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

void s2d::GameEngine::initFonts()
{
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig CustomFont;
    ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
    ImFontConfig icons_config;

    io.IniFilename = nullptr;
    CustomFont.FontDataOwnedByAtlas = false;

    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 3;
    icons_config.OversampleV = 3;

    ImFontConfig config;

    static const ImWchar ranges[] =
    {
        0xf044, 0xf044, // Edit
        0xf1f8, 0xf1f8, // Trash
        0xf079, 0xf079, // Retweet
        0xf04b, 0xf04b, // Play button
        0xf03e, 0xf03e, // Image
        0,
    };

    config.GlyphRanges = ranges;

    //Add the fonts (remember to fill in the correct path of your font
    io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 12, &icons_config, icons_ranges);
    s2d::FontManager::defaultFont = io.Fonts->AddFontFromFileTTF("EngineAssets\\Fonts\\Arial.ttf", s2d::FontManager::fontSize);
    s2d::FontManager::symbolFont = io.Fonts->AddFontFromFileTTF("EngineAssets\\Fonts\\fontawesome-webfont.ttf", s2d::FontManager::fontSize - 4, &config);

    //This function is important else the program will crash with an assertion
    ImGui::SFML::UpdateFontTexture();
}


//public functions

void s2d::GameEngine::update()
{
    //Fullscreen / Not Fullscreen
    this->updateWindowStyle();

    //Renderere / window events
    this->pollEvents();

    ImGui::PushFont(s2d::FontManager::defaultFont);

    //UIWindow (Engine)
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

}


