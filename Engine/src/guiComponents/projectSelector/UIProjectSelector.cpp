#include "UIProjectSelector.h"

// Constructor / Destructor

s2d::UIProjectSelector::UIProjectSelector()
{
	this->m_createWindowSize = ImVec2(900, 450);

    this->m_ptr_toRenderWindow = new sf::RenderWindow(sf::VideoMode(960, 540), "SpriteEngine", sf::Style::Titlebar | sf::Style::Close);
	this->m_renderWindowEvents.type = sf::Event::GainedFocus;
	this->m_userLocation = s2d::UIProjectSelectorLocation::None;
	this->m_currentFileDialoge = s2d::CurrentFileDialog::None;

	this->m_createFileDialoge = s2d::FileDialog("C:\\", ICON_FA_PLUS, "Select where you want to create a project", this->m_createWindowSize);

	// Reads project data from a CSV filed
	this->m_projects = this->readProjectInfosFromFile();

	ImGui::SFML::Init(*this->m_ptr_toRenderWindow);
	ImGuiIO& io = ImGui::GetIO();
	s2d::FontManager::InitFonts(io);

	this->m_leftButtonColor = ImVec4(35.0f / 255.0f,
		35.0f / 255.0f,
		43.0f / 255.0f,
		1.0f);

	this->m_createButtonsColor = ImVec4(49.0f / 255.0f,
		49.0f / 255.0f,
		76.0f / 255.0f,
		1.0f);

	this->m_clickedButton = ImVec4(81.0f / 255.0f,
		107.0f / 255.0f,
		150.0f / 255.0f,
		1.0f);
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
	this->renderFileDialogs();
}

void s2d::UIProjectSelector::projectData()
{
	auto infoOverProjects = [](float padding)
	{
		// Small info over projects
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(18.0f / 255.0f, 18.0f / 255.0f, 22.0f / 255.0f, 1.0f));
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8);
		ImGui::BeginChild("j#", ImVec2(800, 50), false);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 13);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding);
		ImGui::Text("Project Name");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding);
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding);

		ImGui::Text("Last Opened");
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding);
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding);
		ImGui::Text("Path");

		ImGui::EndChild();
		ImGui::PopStyleColor();
	};

	static float paddingBetweenInfo = 60; // 75
	static ImGuiTextFilter filter;

	// search box
	filter.Draw("Search", 340);

	ImGui::PushStyleColor(ImGuiCol_Button, this->m_createButtonsColor);

	// create project
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 670, ImGui::GetCursorPosY() - 40.0f));

	if (ImGui::Button("Create"))
	{
		this->m_currentFileDialoge = s2d::CurrentFileDialog::Create;
	}

	// Open project
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 590, ImGui::GetCursorPosY() - 40.0f));

	if (ImGui::Button("Open"))
	{
		this->m_currentFileDialoge = s2d::CurrentFileDialog::Open;
	}

	ImGui::PopStyleColor();

	infoOverProjects(paddingBetweenInfo);


	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(22.0f / 255.0f,
		22.0f / 255.0f,
		32.0f / 255.0f,
		1.0f));

	if (ImGui::ListBoxHeader("##ProjectList", ImVec2(1000, 200)))
	{
		for (int i = 0; i < this->m_projects.size(); i++)
		{
			const char* filterName = this->m_projects[i].name.c_str();
			if (filter.PassFilter(filterName))
			{
				// from left to first sep its 

				std::string emptyBetweenNameAndCreation = s2d::FileDialog::getEmptyStringBetween("Project Name", filterName, paddingBetweenInfo);
				std::string emptyBetweenCreationAndPath = s2d::FileDialog::getEmptyStringBetween("Last Opened   ", this->m_projects[i].lastOpened, paddingBetweenInfo);

				std::string fullData = filterName + emptyBetweenNameAndCreation + this->m_projects[i].lastOpened + emptyBetweenCreationAndPath + this->m_projects[i].path;
				ImGui::MenuItem(fullData.c_str());
			}
		}
		ImGui::ListBoxFooter();
	}

	ImGui::PopStyleColor();
}

void s2d::UIProjectSelector::docsData()
{
	ImGui::Text("Docs lol");
}


void s2d::UIProjectSelector::renderProjectDataOrDocs()
{
	if (ImGui::Begin("Project Selector / Documentaions", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
	{
		// render project data
		if (this->m_userLocation == s2d::UIProjectSelectorLocation::Projects)
			this->projectData();

		// else render doc's data
		if (this->m_userLocation == s2d::UIProjectSelectorLocation::Docs)
			this->docsData();

		ImGui::SetWindowPos(ImVec2(192, 0));
		ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);
		ImGui::SetWindowSize(ImVec2(960 - 192, 540));
		ImGui::End();
	}
}

void s2d::UIProjectSelector::renderFileDialogs()
{
	if (this->m_currentFileDialoge == s2d::CurrentFileDialog::None)
	{
		return;
	}

	// Opens a file dialog, where u can create a new SpriteEngine project
	if (this->m_currentFileDialoge == s2d::CurrentFileDialog::Create)
	{		
		if (this->m_createFileDialoge.closeWindow())
		{
			this->m_currentFileDialoge = s2d::CurrentFileDialog::None;
		}
		this->m_createFileDialoge.displayNodes();
		//std::cout << this->m_fileDialoge.pathClicked << std::endl;
		
	}
}

s2d::UIProjectSelectorLocation s2d::UIProjectSelector::getUserInputForDataToRender()
{
	s2d::UIProjectSelectorLocation loc = this->m_userLocation;
	ImGui::PushStyleColor(ImGuiCol_Button, this->m_leftButtonColor);

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

std::vector<s2d::ProjectInfo> s2d::UIProjectSelector::readProjectInfosFromFile()
{
	return 
	{
		s2d::ProjectInfo("Dasynce", "..\\User\\Dasynce", "13/1/2022"),
		s2d::ProjectInfo("Game", "..\\User\\Game", "13/12/2020")
	};
}



// Public functions

void s2d::UIProjectSelector::update()
{
	this->pollEvents();

	s2d::UIWindow::renderStyle(&ImGui::GetStyle());
	
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, this->m_clickedButton);
	ImGui::PushFont(s2d::FontManager::defaultFont);
	this->render();
    ImGui::PopFont();
	ImGui::PopStyleColor();

	this->m_ptr_toRenderWindow->clear();
	ImGui::SFML::Render(*this->m_ptr_toRenderWindow);
	this->m_ptr_toRenderWindow->display();

	s2d::Time::update();
}
