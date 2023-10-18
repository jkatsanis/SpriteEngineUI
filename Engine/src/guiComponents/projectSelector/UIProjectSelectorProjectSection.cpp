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
	this->m_createProjectName = "";
	this->m_createWindowSize = ImVec2(900, 450);
	this->m_createFileDialoge = s2d::FolderDialog("C:\\", ICON_FA_PLUS, "Select where you want to create a project", this->m_createWindowSize);
	this->m_openFileDialog = s2d::FolderDialog("C:\\", ICON_FA_EDIT, "Select where you want to open a project", this->m_createWindowSize);
	this->m_currentFileDialoge = s2d::CurrentFileDialog::None;

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
		this->tryToOpenProject();
	}
}

// Private methods

void s2d::UIProjectSelectorProjectSection::createProject()
{
	const std::string tryToOpenPath = s2d::flc::copyDir(PATH_TO_RESSOURCES"\\template", this->m_createFileDialoge.pathClicked, this->m_createProjectName);

	// Setting the project we just selected to be displayed in the selector list
	this->m_openFileDialog.pathClicked = tryToOpenPath + "\\";
	this->m_openFileDialog.folderClicked = this->m_createProjectName.data();

	this->m_createProjectName = "";
	this->m_createFileDialoge.disableWindow();
}

void s2d::UIProjectSelectorProjectSection::renderInfoOverProjects(float padding)
{
	// Small info over projects
	ImGui::PushStyleColor(ImGuiCol_ChildBg, SELECTOR_TOP_BAR_COLOR);
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

	ImGui::PushStyleColor(ImGuiCol_Button, SELECTOR_FILEDIALOGE_BUTTON_COLOR);

	this->m_currentFileDialoge = this->getFileDialogeWhichUserWantsToMake();
	this->renderInfoOverProjects(paddingBetweenInfo);

	ImGui::PushStyleColor(ImGuiCol_FrameBg, SELECTOR_LISTBOX_BACKGROUND_COLOR);
	if (ImGui::ListBoxHeader("##ProjectList", ImVec2(1000, 420)))
	{
		for (int i = 0; i < this->m_projects.size(); i++)
		{
			const char* filterName = this->m_projects[i].name.c_str();
			if (filter.PassFilter(filterName))
			{
				// from left to first sep its 

				std::string emptyBetweenNameAndCreation = s2d::FolderDialog::getEmptyStringBetween("Project Name", filterName, paddingBetweenInfo);
				std::string emptyBetweenCreationAndPath = s2d::FolderDialog::getEmptyStringBetween("Last Opened   ", this->m_projects[i].lastOpened, paddingBetweenInfo);

				std::string fullData = filterName + emptyBetweenNameAndCreation + this->m_projects[i].lastOpened + emptyBetweenCreationAndPath + this->m_projects[i].absulutePath;

				// CHECK IF WE OPEN THE PROJECT
				if (ImGui::MenuItem(fullData.c_str()))
				{
					s2d::EngineData::s_path_to_user_project = this->m_projects[i].relativePath;
					s2d::EngineData::s_name_of_user_project = this->m_projects[i].name;
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
		if (this->m_createFileDialoge.isWindowClosed())
		{
			this->m_currentFileDialoge = s2d::CurrentFileDialog::None;
		}
		this->m_createFileDialoge.displayNodes();
	}
	if (this->m_currentFileDialoge == s2d::CurrentFileDialog::Open)
	{
		if (this->m_openFileDialog.isWindowClosed())
		{
			this->m_currentFileDialoge = s2d::CurrentFileDialog::None;
		}
		this->m_openFileDialog.displayNodes();
	}
}

void s2d::UIProjectSelectorProjectSection::createPopupToCreateProject()
{
	if (this->m_createFileDialoge.pathClicked != "")
	{
		const std::string CREATE_PROJECT_AT = "Create project at - " + this->m_createFileDialoge.pathClicked;
		const float WINDOW_SIZE_X = ImGui::CalcTextSize(CREATE_PROJECT_AT.c_str()).x + 150;
		const std::string BUTTON_NAME = "x" + std::string("##") + this->m_createFileDialoge.pathClicked;

		// Setting the focus from the file dialoge to false, since we render a new important pop up 
		// over it, there should be the focus
		this->m_createFileDialoge.windowFocus = false;

		ImGui::SetNextWindowFocus();
		ImGui::Begin("##CreateFinalStep", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

		// When the user cancels or closes the window the pathclicked will be resetet
		ImGui::Text(CREATE_PROJECT_AT.c_str());
		ImGui::SameLine(10);

		// Setting the path to be empty since the user doesnt want to create
		// at this dir a project 

		ImVec2 cursorPos = ImVec2(ImGui::GetCursorPosX() + WINDOW_SIZE_X - 40, ImGui::GetCursorPosY() - 8);
		ImGui::SetCursorPos(cursorPos);
		if (ImGui::Button(BUTTON_NAME.c_str()))
		{
			this->m_createFileDialoge.disableWindow();
		}

		ImGui::PushStyleColor(ImGuiCol_Button, SELECTOR_LEFT_BUTTONS_COLOR);

		ImGui::Separator();
		ImGui::Text("");
		ImGui::Dummy(ImVec2(100, 0));
		ImGui::Text("Name: ");
		ImGui::SameLine();
		ImGui::InputText("##name", &this->m_createProjectName[0], sizeof(std::string));

		ImGui::SetWindowSize(ImVec2(400, 200));
		float x = ImGui::GetCursorPosX();
		float y = ImGui::GetCursorPosY() + 22;

		ImGui::SetCursorPosY(y);
		ImGui::SetCursorPosX(x += WINDOW_SIZE_X - 210);
		if (ImGui::Button("Cancel"))
		{
			this->m_createFileDialoge.disableWindow();
		}

		ImGui::SetCursorPosY(y);
		ImGui::SetCursorPosX(x += 100);
		if (ImGui::Button("Create"))
		{
			std::string s = this->m_createFileDialoge.pathClicked + this->m_createProjectName.data();
			if (!s2d::flc::checkIfProjcetAlreadyExists(s))
			{
				this->createProject();
			}
			else
			{
				this->m_createFileDialoge.disableWindow();
				std::cout << "LOG: There is already a project with that name!";
			}
		}

		ImGui::PopStyleColor();

		ImGui::SetWindowSize(ImVec2(WINDOW_SIZE_X, 200));
		ImGui::End();
	}
}

void s2d::UIProjectSelectorProjectSection::tryToOpenProject()
{
	if (this->m_openFileDialog.pathClicked != "" && !s2d::flc::checkIfProjcetAlreadyExists(this->m_openFileDialog.pathClicked))
	{
		std::string pathToVerify = this->m_openFileDialog.pathClicked + "engine\\saves\\verify.vsn";		

		if(!s2d::flc::isProjectPathValid(pathToVerify))
		{
			this->m_openFileDialog.pathClicked = "";
			return;
		}

		std::filesystem::path current_path = std::filesystem::current_path();
		std::filesystem::path relative_path = std::filesystem::relative(this->m_openFileDialog.pathClicked, current_path);

		std::string name = this->m_openFileDialog.folderClicked;

		this->m_projects.push_back(s2d::UserProjectInfo(name, this->m_openFileDialog.pathClicked, "now", relative_path.string()));

		this->m_openFileDialog.pathClicked = "";
		this->m_openFileDialog.folderClicked = "";
		this->m_currentFileDialoge = s2d::CurrentFileDialog::None;

		// Updating the file when we open or CREATE a project
		s2d::EngineData::s_name_of_user_project = name;
		s2d::EngineData::s_path_to_user_project = relative_path.string();
		s2d::flc::createKnownProjectDirFile();
		s2d::EngineData::s_name_of_user_project = "";
		s2d::EngineData::s_path_to_user_project = "";
	}
	else if(this->m_openFileDialog.pathClicked != "")
	{
		this->m_openFileDialog.disableWindow();
		std::cout << "LOG: There is already a project in the list with this name!" << std::endl;
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
			std::vector<std::string> propertys = std::splitString(line, delimiter);

			std::string pathToVerify = propertys[1] + "\\engine\\saves\\verify.vsn";

			if(s2d::flc::isProjectPathValid(pathToVerify))
			{
				//INITIIALIZING PROPS
				userProjects.push_back(s2d::UserProjectInfo(propertys[0], propertys[1], propertys[2], propertys[3]));
			}
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
		this->m_createFileDialoge.enableWindow();
		where = s2d::CurrentFileDialog::Create;
	}

	// Open project
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 590, ImGui::GetCursorPosY() - 40.0f));

	if (ImGui::Button("Open"))
	{
		this->m_openFileDialog.enableWindow();
		where = s2d::CurrentFileDialog::Open;
	}

	ImGui::PopStyleColor();

	return where;
}
