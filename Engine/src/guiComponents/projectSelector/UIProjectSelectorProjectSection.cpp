#include "UIProjectSelectorProjectSection.h"

// Constructor / init

s2d::UIProjectSelectorProjectSection::UIProjectSelectorProjectSection()
{
	this->init();
}

s2d::UIProjectSelectorProjectSection::UIProjectSelectorProjectSection(const s2d::UIProjectSelectorLocation* location)
	:m_ptr_UILocation(location)
{
	this->init();
}

void s2d::UIProjectSelectorProjectSection::init()
{
	this->m_createWindowSize = ImVec2(900, 450);
	this->m_createFileDialoge = s2d::FileDialog("C:\\", ICON_FA_PLUS, "Select where you want to create a project", this->m_createWindowSize);

	// Reads project data from a CSV filed
	this->m_projects = this->readProjectInfosFromFile();
}


// Public methods

void s2d::UIProjectSelectorProjectSection::update()
{
	if (*this->m_ptr_UILocation == s2d::UIProjectSelectorLocation::Projects)
	{
		this->renderProjectData();
		this->renderFileDialogs();
		this->createPopupToCreateProject();
	}
}

// Private methods

void s2d::UIProjectSelectorProjectSection::renderInfoOverProjects(float padding)
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
}

void s2d::UIProjectSelectorProjectSection::renderProjectData()
{
	ImGui::PushStyleColor(ImGuiCol_Button, PROJECT_SECTION_BUTTON_COLOR);

	static float paddingBetweenInfo = 60; // 75
	static ImGuiTextFilter filter;

	// search box
	filter.Draw("Search", 340);


	this->m_currentFileDialoge = this->getFileDialogeWhichUserWantsToMake();
	this->renderInfoOverProjects(paddingBetweenInfo);

	ImGui::PushStyleColor(ImGuiCol_FrameBg, LIST_BOX_BACKGROUND_COLOR);
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

void s2d::UIProjectSelectorProjectSection::renderFileDialogs()
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
	}
}

void s2d::UIProjectSelectorProjectSection::createPopupToCreateProject()
{
	if (this->m_createFileDialoge.pathClicked != "")
	{
		// Setting the focus from the file dialoge to false, since we render a new important pop up 
		// over it, there should be the focus
		this->m_createFileDialoge.windowFocus = false;

		ImGui::SetNextWindowFocus();
		ImGui::Begin("##CreateFinalStep", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

		const std::string CREATE_PROJECT_AT = "Create project at - " + this->m_createFileDialoge.pathClicked;
		const float WINDOW_SIZE_X = ImGui::CalcTextSize(CREATE_PROJECT_AT.c_str()).x + 150;

		// When the user cancels or closes the window the pathclicked will be resetet
		ImGui::Text(CREATE_PROJECT_AT.c_str());
		ImGui::SameLine(10);

		ImVec2 cursorPos = ImVec2(ImGui::GetCursorPosX() + WINDOW_SIZE_X - 40, ImGui::GetCursorPosY() - 8);
		ImGui::SetCursorPos(cursorPos);

		// Setting the path to be empty since the user doesnt want to create
		// at this dir a project 
		const std::string BUTTON_NAME = "x" + std::string("##") + this->m_createFileDialoge.pathClicked;


		if(ImGui::Button(BUTTON_NAME.c_str()))
		{
			this->m_createFileDialoge.reset();
		}

		ImGui::PushStyleColor(ImGuiCol_Button, PROJECT_SECTION_BUTTON_COLOR);

		ImGui::Separator();

		ImGui::Dummy(ImVec2(100, 50));
		ImGui::Text("Name: ");
		ImGui::SameLine();
		ImGui::InputText("##name", &name[0], sizeof(std::string));

		float x = ImGui::GetCursorPosX();
		float y = ImGui::GetCursorPosY() + 25;

		ImGui::SetCursorPosY(y);
		ImGui::SetCursorPosX(x += WINDOW_SIZE_X - 210);
		if (ImGui::Button("Cancel"))
		{
			this->m_createFileDialoge.reset();
		}

		ImGui::SetCursorPosY(y);
		ImGui::SetCursorPosX(x += 100);
		if (ImGui::Button("Create"))
		{
			// Create a porject with the name given and the current path
			this->createProject();
		}

		ImGui::PopStyleColor();


	
		ImGui::SetWindowSize(ImVec2(WINDOW_SIZE_X, this->m_createWindowSize.y / 2));
		ImGui::End();
	}
}

void s2d::UIProjectSelectorProjectSection::createProject()
{
	s2d::flc::copyDir("template", this->m_createFileDialoge.pathClicked, this->name);
	this->name = "";
	this->m_createFileDialoge.reset();
}

std::vector<s2d::ProjectInfo> s2d::UIProjectSelectorProjectSection::readProjectInfosFromFile()
{
	return
	{
		s2d::ProjectInfo("Dasynce", "..\\User\\Dasynce", "13/1/2022"),
		s2d::ProjectInfo("Game", "..\\User\\Game", "13/12/2020")
	};
}

s2d::CurrentFileDialog s2d::UIProjectSelectorProjectSection::getFileDialogeWhichUserWantsToMake()
{
	s2d::CurrentFileDialog where = this->m_currentFileDialoge;

	// create project
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 670, ImGui::GetCursorPosY() - 40.0f));

	if (ImGui::Button("Create"))
	{
		where = s2d::CurrentFileDialog::Create;
	}

	// Open project
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 590, ImGui::GetCursorPosY() - 40.0f));

	if (ImGui::Button("Open"))
	{
		where = s2d::CurrentFileDialog::Open;
	}

	ImGui::PopStyleColor();

	return where;
}
