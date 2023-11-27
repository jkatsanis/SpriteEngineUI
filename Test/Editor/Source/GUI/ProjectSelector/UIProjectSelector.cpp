#include "UIProjectSelector.h"

// Ctor

spe::UIProjectSelector::UIProjectSelector()
{
	const std::string path = spe::Utility::GetDefaultDir();
	spe::Utility::SetCurrentDir(path);

	// Getting the directory where the editor/core is
	spe::EngineData::s_PathOfEngine = spe::Utility::GetCurrentDir();

	this->m_Window = spe::GameWindow(SELECTOR_WINDOW_SIZE, "SpriteEngine");
	spe::Style::Init();
	spe::Style::RenderStyle();

	this->m_CreateFileDialoge = spe::FileDialog("C:\\", ICON_FA_PLUS, "Select where you want to create a project", SELECTOR_FILE_DIALOG_SIZE, false, SELECTOR_FONT_SCALE);
	this->m_OpenFileDialog = spe::FileDialog("C:\\", ICON_FA_EDIT, "Select where you want to open a project", SELECTOR_FILE_DIALOG_SIZE, false, SELECTOR_FONT_SCALE);
	this->m_IsOpen = true;

	this->m_Projects = this->ReadProjectData();
}

// Public

void spe::UIProjectSelector::Update()
{
	this->m_Window.PollEvents();

	ImGui::PushFont(spe::Style::s_DefaultFont);
	this->RenderWindow();
	ImGui::PopFont();

	this->m_Window.Clear();
	this->m_Window.Display();
}

void spe::UIProjectSelector::Shutdown()
{
	this->m_Window.Shutdown();
	spe::Savesystem::SaveProjects(this->m_Projects);
}

// Private

void spe::UIProjectSelector::RenderWindow()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	if (ImGui::Begin("Project Selector / Documentaions", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar))
	{
		this->RenderFileDialogAndButtons();
		this->RenderProjectData();
	
		if (!this->m_Window.IsOpen())
		{
			this->m_IsOpen = false;
		}

		ImGui::SetWindowSize(SELECTOR_WINDOW_SIZE);
		ImGui::SetWindowFontScale(SELECTOR_FONT_SCALE);
		ImGui::End();
	}
}

bool spe::UIProjectSelector::IsProjectPathValid(const std::string& path)
{
	// TODO logic to chck if verify.vsn is valid, just simple now
	std::ifstream file(path);
	bool good = file.good();
	file.close();
	return good;
}

void spe::UIProjectSelector::RenderFileDialogAndButtons()
{
	const ImVec2 temp = ImGui::GetCursorPos();
	ImGui::PushStyleColor(ImGuiCol_Button, SELECTOR_FILEDIALOGE_BUTTON_COLOR);
	ImGui::SetCursorPos(ImVec2(880, 10));

	if (ImGui::Button("Create"))
	{
		this->m_CreateFileDialoge.enableWindow();
	}
	if (this->m_CreateFileDialoge.itemClicked == "")
	{
		this->m_CreateFileDialoge.displayNodes();
	}
	else
	{
		this->CreatePopupToCreateProject();
	}

	// Open project
	ImGui::SetCursorPos(ImVec2(800, 10));

	if (ImGui::Button("Open"))
	{
		this->m_OpenFileDialog.enableWindow();
	}
	if (this->m_OpenFileDialog.itemClicked == "")
	{
		this->m_OpenFileDialog.displayNodes();
	}
	else
	{
		if (!this->TryToOpenProject())
		{
			spe::Log::LogString("Failed to open project");
			this->m_OpenFileDialog.disableWindow();
		}
	}

	ImGui::SetCursorPos(temp);

	ImGui::PopStyleColor();
}

void spe::UIProjectSelector::CreatePopupToCreateProject()
{
	const std::string CREATE_PROJECT_AT = "Create project at - " + this->m_CreateFileDialoge.pathClicked;
	const float WINDOW_SIZE_X = ImGui::CalcTextSize(CREATE_PROJECT_AT.c_str()).x + 150;
	const std::string BUTTON_NAME = "x" + std::string("##") + this->m_CreateFileDialoge.pathClicked;

	// Setting the focus from the file dialoge to false, since we render a new important pop up 
	// over it, there should be the focus
	this->m_CreateFileDialoge.windowFocus = false;

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
		this->m_CreateFileDialoge.disableWindow();
	}

	ImGui::PushStyleColor(ImGuiCol_Button, SELECTOR_LEFT_BUTTONS_COLOR);

	ImGui::Separator();
	ImGui::Text("");
	ImGui::Dummy(ImVec2(100, 0));
	ImGui::Text("Name: ");
	ImGui::SameLine();
	ImGui::InputText("##name", &this->m_CreateProjectName[0], sizeof(std::string));

	ImGui::SetWindowSize(ImVec2(400, 200));
	float x = ImGui::GetCursorPosX();
	float y = ImGui::GetCursorPosY() + 22;

	ImGui::SetCursorPosY(y);
	ImGui::SetCursorPosX(x += WINDOW_SIZE_X - 210);
	if (ImGui::Button("Cancel"))
	{
		this->m_CreateFileDialoge.disableWindow();
	}

	ImGui::SetCursorPosY(y);
	ImGui::SetCursorPosX(x += 100);
	if (ImGui::Button("Create"))
	{
		this->m_FullProjectPath = this->m_CreateFileDialoge.pathClicked + "\\" + this->m_CreateProjectName.data();
		if (!this->CheckIfprojectExists(this->m_FullProjectPath))
		{
			this->CreateProject();
		}
		else
		{
			this->m_CreateFileDialoge.disableWindow();
			std::cout << "LOG: There is already a project with that name!";
		}
	}

	ImGui::PopStyleColor();

	ImGui::SetWindowSize(ImVec2(WINDOW_SIZE_X, 200));
	ImGui::End();
}

bool spe::UIProjectSelector::CheckIfprojectExists(const std::string& path)
{
	for (size_t i = 0; i < this->m_Projects.size(); i++)
	{
		if (this->m_Projects[i].AbsulutePath == path)
		{
			return true;
		}
	}
	return false;
}

void spe::UIProjectSelector::CreateProject()
{
	char buffer[50];
	strcpy_s(buffer, this->m_CreateProjectName.c_str());

	const std::string fullpath = this->m_CreateFileDialoge.pathClicked + "\\" + std::string(buffer);
	this->AddProject(fullpath, std::string(buffer));

	const std::string path = this->m_CreateFileDialoge.pathClicked + "\\";
	const std::string tryToOpenPath = spe::Utility::CopyDir("Template", path, this->m_CreateProjectName);

	// Running the script assumes that CMake is installed
	this->CreateProjectFiles(this->m_CreateFileDialoge.pathClicked, buffer);

	// Setting the project we just selected to be displayed in the selector list
	this->m_CreateFileDialoge.pathClicked = tryToOpenPath + "\\";
	this->m_CreateFileDialoge.itemClicked = this->m_CreateProjectName.data();

	this->m_CreateProjectName = "";
	this->m_CreateFileDialoge.disableWindow();
}

void spe::UIProjectSelector::CreateProjectFiles(const std::string& path, const std::string& name)
{
	// Run cmake for the user project
	spe::Savesystem::SaveProjects(this->m_Projects);
	std::string command = "cmake ..";

	const std::string dir = spe::Utility::GetCurrentDir();
	const std::string newdir = path + +"\\" + name + "\\Build";

	spe::Utility::SetCurrentDir(newdir);

	system(command.c_str());

	spe::Utility::SetCurrentDir(dir);
}

void spe::UIProjectSelector::AddProject(const std::string& full_path, const std::string& name)
{
	std::filesystem::path current_path = std::filesystem::current_path();
	std::filesystem::path relative_path = std::filesystem::relative(full_path, current_path);

	char buffer[80];
	struct tm  timeinfo;

	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	gmtime_s(&timeinfo, &in_time_t);
	std::strftime(buffer, sizeof(buffer), "%Y/%d/%m %X", &timeinfo);
	std::string date(buffer);

	this->m_Projects.push_back(spe::UserProjectInfo(name, full_path, date, relative_path.string()));
}

bool spe::UIProjectSelector::TryToOpenProject()
{
	const std::string path = this->m_OpenFileDialog.pathClicked;
	if (!this->CheckIfprojectExists(path))
	{
		std::string pathToVerify = path + "\\Engine\\Saves\\verify.vsn";

		if (!this->IsProjectPathValid(pathToVerify))
		{
			spe::Log::LogString("verify.vsn is missing");
			return false;
		}

		std::string name = this->m_OpenFileDialog.itemClicked;

		this->AddProject(this->m_OpenFileDialog.pathClicked, name);

		this->m_OpenFileDialog.disableWindow();
		return true;
	}
	
	spe::Log::LogString("Project does already exist!");
	return false;
	
}

std::vector<spe::UserProjectInfo> spe::UIProjectSelector::ReadProjectData()
{
	std::vector<spe::UserProjectInfo> userProjects;

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
			std::vector<std::string> propertys = spe::Utility::Split(line, DELIMITER);

			if (propertys.size() < 3)
			{
				continue;
			}

			std::string pathToVerify = propertys[1] + "\\engine\\saves\\verify.vsn";

			if (this->IsProjectPathValid(pathToVerify))
			{
				//INITIIALIZING PROPS
				userProjects.push_back(spe::UserProjectInfo(propertys[0], propertys[1], propertys[2], propertys[3]));
			}
		}
		userProjectFile.close();

	}
	return userProjects;
}

void spe::UIProjectSelector::RenderProjectData()
{
	static float paddingBetweenInfo = 60; // 75
	static ImGuiTextFilter filter;

	// search box
	filter.Draw("Search", 340);

	this->RrenderInfoOverProjects(paddingBetweenInfo);

	ImGui::PushStyleColor(ImGuiCol_FrameBg, SELECTOR_LISTBOX_BACKGROUND_COLOR);
	if (ImGui::ListBoxHeader("##ProjectList", ImVec2(960, 540)))
	{
		for (int i = 0; i < this->m_Projects.size(); i++)
		{
			const char* filterName = this->m_Projects[i].name.c_str();
			if (filter.PassFilter(filterName))
			{
				// from left to first sep its 

				std::string emptyBetweenNameAndCreation = spe::FileDialog::getEmptyStringBetween("Project Name", filterName, paddingBetweenInfo);
				std::string emptyBetweenCreationAndPath = spe::FileDialog::getEmptyStringBetween("Last Opened   ", this->m_Projects[i].lastOpened, paddingBetweenInfo);

				std::string fullData = filterName + emptyBetweenNameAndCreation + this->m_Projects[i].lastOpened + emptyBetweenCreationAndPath + this->m_Projects[i].AbsulutePath;

				// CHECK IF WE OPEN THE PROJECT
				if (ImGui::MenuItem(fullData.c_str()))
				{
					spe::EngineData::s_PathUserProject = this->m_Projects[i].relativePath;
					spe::EngineData::s_NameOfUser = this->m_Projects[i].name;
					this->m_IsOpen = false;
				}
			}
		}
		ImGui::ListBoxFooter();
	}

	ImGui::PopStyleColor();
}

void spe::UIProjectSelector::RrenderInfoOverProjects(float padding)
{	
	// Small info over projects
	ImGui::PushStyleColor(ImGuiCol_ChildBg, SELECTOR_TOP_BAR_COLOR);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8);
	ImGui::BeginChild("j#", ImVec2(SELECTOR_WINDOW_SIZE.x, 50), false);
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
