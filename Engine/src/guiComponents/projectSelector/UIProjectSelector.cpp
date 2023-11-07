#include "UIProjectSelector.h"

// Constructor / Destructor

s2d::UIProjectSelector::UIProjectSelector()
{
    this->m_ptr_render_window = new sf::RenderWindow(sf::VideoMode(960, 540), "SpriteEngine", sf::Style::Titlebar | sf::Style::Close);
	this->m_renderWindowEvents.type = sf::Event::GainedFocus;
	this->m_userLocation = s2d::UIProjectSelectorLocation::Projects;
	this->m_projectSection = s2d::UIProjectSelectorProjectSection(&this->m_userLocation);

	ImGui::SFML::Init(*this->m_ptr_render_window);
	ImGuiIO& io = ImGui::GetIO();
	s2d::FontManager::InitFonts(io);


	sf::Image icon64;

	if (icon64.loadFromFile("ressources/Icons/icon.png"))
	{
		this->m_ptr_render_window->setIcon(icon64.getSize().x, icon64.getSize().y, icon64.getPixelsPtr());
	}

	// When the project selector gets constructed the not known projects will automaticly get deletet from the file
	s2d::flc::removeInvalidPathsFromFile();
}

s2d::UIProjectSelector::~UIProjectSelector()
{
	s2d::Time::reset();
	delete this->m_ptr_render_window;
}

// Private functions

void s2d::UIProjectSelector::pollEvents()
{
	while (this->m_ptr_render_window->pollEvent(this->m_renderWindowEvents))
	{
		ImGui::SFML::ProcessEvent(this->m_renderWindowEvents);

		if (this->m_renderWindowEvents.type == sf::Event::Closed)
		{
			this->m_ptr_render_window->close();
		}
	}
	ImGui::SFML::Update(*this->m_ptr_render_window, s2d::Time::s_delta_clock.restart());
}

void s2d::UIProjectSelector::render()
{
	this->m_userLocation = this->getUserInputForDataToRender();
	this->renderProjectDataOrDocs();
}


void s2d::UIProjectSelector::docsData()
{
	ImGui::Text("Docs lol");
}

void s2d::UIProjectSelector::renderProjectDataOrDocs()
{
	if (ImGui::Begin("Project Selector / Documentaions", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::SetCursorPosY(DATA_PADDING_TOP);

		// project data
		this->m_projectSection.update();

		// else render doc's data
		if (this->m_userLocation == s2d::UIProjectSelectorLocation::Docs)
		{
			this->docsData();
		}

		ImGui::SetWindowPos(ImVec2(192, 0));
		ImGui::SetWindowFontScale(1);
		ImGui::SetWindowSize(ImVec2(960 - 192, 540));
		ImGui::End();
	}
}

s2d::UIProjectSelectorLocation s2d::UIProjectSelector::getUserInputForDataToRender()
{
	s2d::UIProjectSelectorLocation loc = this->m_userLocation;
	ImGui::PushStyleColor(ImGuiCol_Button, SELECTOR_LEFT_BUTTONS_COLOR);

	if (ImGui::Begin("Left buttons", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
	{
		ImGui::SetCursorPosY(DATA_PADDING_TOP);

		if (ImGui::Button("Projects", ImVec2(180, 50)))
		{
			loc = s2d::UIProjectSelectorLocation::Projects;
		}

		if (ImGui::Button("Documentation ", ImVec2(180, 50)))
		{
			loc = s2d::UIProjectSelectorLocation::Docs;
		}

		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowSize(ImVec2(192, 540));
		ImGui::SetCursorPos(ImVec2(4, 35));

		ImGui::End();
	}
	ImGui::PopStyleColor();

	return loc;
}

// Public functions

void s2d::UIProjectSelector::update()
{
	if (s2d::EngineData::s_path_to_user_project != "")
	{
		this->m_ptr_render_window->close();
	}

	this->pollEvents();
	ImGuiStyle& style = ImGui::GetStyle();
	s2d::UIWindow::renderStyle(&style);

	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, SELECTOR_BUTTON_HOVERED);
	ImGui::PushFont(s2d::FontManager::s_default_font);

	// RENDER
	this->render();

    ImGui::PopFont();
	ImGui::PopStyleColor();

	this->m_ptr_render_window->clear();
	ImGui::SFML::Render(*this->m_ptr_render_window);
	this->m_ptr_render_window->display();

	s2d::Time::update();
}
