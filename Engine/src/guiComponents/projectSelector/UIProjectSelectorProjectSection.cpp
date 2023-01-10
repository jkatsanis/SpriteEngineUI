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

	this->m_createButtonsColor = ImVec4(49.0f / 255.0f,
		49.0f / 255.0f,
		76.0f / 255.0f,
		1.0f);

	this->m_listBoxHeaderBackground = ImVec4(22.0f / 255.0f,
		22.0f / 255.0f,
		32.0f / 255.0f,
		1.0f);
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
	static float paddingBetweenInfo = 60; // 75
	static ImGuiTextFilter filter;

	// search box
	filter.Draw("Search", 340);

	ImGui::PushStyleColor(ImGuiCol_Button, this->m_createButtonsColor);

	this->m_currentFileDialoge = this->getFileDialogeWhichUserWantsToMake();
	this->renderInfoOverProjects(paddingBetweenInfo);

	ImGui::PushStyleColor(ImGuiCol_FrameBg, this->m_listBoxHeaderBackground);
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

				std::string fullData = filterName + emptyBetweenNameAndCreation + this->m_projects[i].lastOpened + emptyBetweenCreationAndPath + this->m_projects[i].absulutePath;

				// CHECK IF WE OPEN THE PROJECT
				if (ImGui::MenuItem(fullData.c_str()))
				{
					s2d::EngineData::s_pathToUserProject = this->m_projects[i].relativePath;
					s2d::EngineData::s_nameOfUserProject = this->m_projects[i].name;
				}
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
		ImGui::SetNextWindowFocus();
		ImGui::Begin("##CreateFinalStep", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

		//const std::string CREATE_PROJECT_AT_PATH = 

		//// When the user cancels or closes the window the pathclicked will be resetet
		//ImGui::Text("")

		ImGui::SetWindowSize(ImVec2(400, 200));
		ImGui::End();
	}
}

std::vector<s2d::UserProjectInfo> s2d::UIProjectSelectorProjectSection::readProjectInfosFromFile()
{
	std::vector<s2d::UserProjectInfo> userProjects;

	std::fstream userProjectFile;

	//opening the file where all sprite data is

	userProjectFile.open(PATH_TO_KNOWN_PROJECTS, std::ios::in);
	if (userProjectFile.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(userProjectFile, line))
		{
			cnt++;
			//First line is the header so we dont need to check for it
			if (cnt == 1)
			{
				continue;
			}

			//Splitting line
			std::string delimiter = ";";
			std::string* propertys = std::splitString(line, delimiter);

			//INITIIALIZING PROPS
			userProjects.push_back(s2d::UserProjectInfo(propertys[0], propertys[1], propertys[2], propertys[3]));
		}
		userProjectFile.close();

	}
	return userProjects;
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
