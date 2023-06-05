#include "UIProjectSelector.h"

// Constructor / Destructor

s2d::UIProjectSelector::UIProjectSelector()
{
    this->m_ptr_toRenderWindow = new sf::RenderWindow(sf::VideoMode(960, 540), "SpriteEngine", sf::Style::Titlebar | sf::Style::Close);
	this->m_renderWindowEvents.type = sf::Event::GainedFocus;
	this->m_userLocation = s2d::UIProjectSelectorLocation::None;
	this->m_projectSection = s2d::UIProjectSelectorProjectSection(&this->m_userLocation);

	ImGui::SFML::Init(*this->m_ptr_toRenderWindow);
	ImGuiIO& io = ImGui::GetIO();
	s2d::FontManager::InitFonts(io);

	// When the project selector gets constructed the not known projects will automaticly get deletet from the file
	s2d::flc::removeInvalidPathsFromFile();
}

s2d::UIProjectSelector::~UIProjectSelector()
{
	s2d::Time::reset();
	delete this->m_ptr_toRenderWindow;
}

// Private functions

void s2d::UIProjectSelector::pollEvents()
{
	while (this->m_ptr_toRenderWindow->pollEvent(this->m_renderWindowEvents))
	{
		ImGui::SFML::ProcessEvent(this->m_renderWindowEvents);

		if (this->m_renderWindowEvents.type == sf::Event::Closed)
		{
			this->m_ptr_toRenderWindow->close();
		}
	}
	ImGui::SFML::Update(*this->m_ptr_toRenderWindow, s2d::Time::deltaClock.restart());
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
	if (ImGui::Begin("Project Selector / Documentaions", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
	{
		// project data
		this->m_projectSection.update();

		// else render doc's data
		if (this->m_userLocation == s2d::UIProjectSelectorLocation::Docs)
			this->docsData();

		ImGui::SetWindowPos(ImVec2(192, 0));
		ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);
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
		ImGui::SetCursorPosY(35);

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
	if (s2d::EngineData::s_pathToUserProject != "")
	{
		this->m_ptr_toRenderWindow->close();
	}

	this->pollEvents();

	s2d::UIWindow::renderStyle(&ImGui::GetStyle());
	
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, SELECTOR_BUTTON_HOVERED);
	ImGui::PushFont(s2d::FontManager::defaultFont);
	this->render();
    ImGui::PopFont();
	ImGui::PopStyleColor();

	this->m_ptr_toRenderWindow->clear();
	ImGui::SFML::Render(*this->m_ptr_toRenderWindow);
	this->m_ptr_toRenderWindow->display();

	s2d::Time::update();
}
