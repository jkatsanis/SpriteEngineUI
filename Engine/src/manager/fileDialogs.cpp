#include "fileDialogs.h"

// Constructor

s2d::FileDialog::FileDialog()
{
    this->m_closeWindow = false;
    this->m_displayedTitle = true;
    this->pathClicked = "";
}

s2d::FileDialog::FileDialog(std::string path, std::string icon, std::string title, ImVec2 windowSize)
{
    this->m_PATH = path;
    this->m_WINDOW_SIZE = windowSize;
    this->m_TITLE = title;
    this->m_displayedTitle = true;
    this->pathClicked = "";
    this->m_ICON = icon;
}

// Public methods

void s2d::FileDialog::displayNodes()
{
    this->m_closeWindow = false;

    ImGui::SetNextWindowFocus();
    ImGui::Begin("##FileDialoge", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);


    this->m_displayedTitle = true;
    auto displayTitle = [](const std::string& title)
    {
        const float PADDING_Y = 5;
        ImGui::Text(title.c_str());
        ImGui::Dummy(ImVec2(0, PADDING_Y));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, PADDING_Y));
    };


    std::string name = "x" + std::string("##") + this->m_PATH;
    ImVec2 old = ImGui::GetCursorPos();
    ImVec2 closeCursorPos = ImVec2(ImGui::GetCursorPosX() + this->m_WINDOW_SIZE.x - 60, ImGui::GetCursorPosY() - 5);
    ImGui::SetCursorPos(closeCursorPos);

    // Clicked at the "x", stop displaying the file dialoge
    if (ImGui::Button(name.c_str()))
    {
        this->m_closeWindow = true;
    }

    ImGui::SetCursorPos(old);

    if (this->m_displayedTitle)
    {
        displayTitle(this->m_TITLE);
        this->m_displayedTitle = false;

        // clicked on the icon of the right
        if (this->displaySymbol(this->m_ICON, this->m_WINDOW_SIZE.x))
        {
            this->pathClicked = this->m_PATH;
        }
        ImGui::SetNextItemOpen(true);

        if (ImGui::TreeNode(this->m_PATH.c_str()))
        {
            this->openFile(this->m_PATH.c_str());
            ImGui::TreePop();
        }
    }

    ImGui::SetWindowSize(this->m_WINDOW_SIZE);
    ImGui::End();
}

// private methods

bool s2d::FileDialog::checkIfADirHasSubDirs(const std::string& dirPath)
{
    bool value = false;
    DIR* dir = opendir(dirPath.c_str());
    if (dir == NULL)
        return false;

    // Read each entry in the directory
    dirent* entry;
    int cnt = 0;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) 
            {
                continue;
            }
            value = true;
        }
    }

    closedir(dir);

    return value;
}

void s2d::FileDialog::openFile(const char* dir_path)
{
    DIR* dir = opendir(dir_path);
    if (dir == NULL)
        return;

    dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0
            || std::string(entry->d_name).find('$') != std::string::npos)
        {
            continue;
        }

        // Compute the full path of the entry
        std::string path = dir_path + std::string("\\") + entry->d_name;

        // If the entry is a directory, recursively traverse it
        if (entry->d_type == DT_DIR)
        {
            // clicked on the icon of the right
            const std::string BUTTON_NAME = this->m_ICON + "##" + path;

            if (this->displaySymbol(BUTTON_NAME, this->m_WINDOW_SIZE.x))
            {
                std::cout << path;
                this->pathClicked = entry->d_name;
            }

            if (!checkIfADirHasSubDirs(path))
            {
                std::string name = "       " + std::string(entry->d_name);
                ImGui::MenuItem(name.c_str());
            }
            else if (ImGui::TreeNode(entry->d_name))
            {
                openFile(path.c_str());
                ImGui::TreePop();
            }

        }
    }

    // Close the directory
    closedir(dir);
    this->m_displayedTitle = true;
}

// static methods

bool s2d::FileDialog::displaySymbol(const std::string& icon, float windowSizeX)
{
    bool clicked = false;
    ImVec2 old = ImGui::GetCursorPos();
    ImVec2 cursorPos = ImVec2(windowSizeX - 55, ImGui::GetCursorPosY() - 6);

    // The icon right of the window displayed, when it got clicked the
    // 'pathClicked' prop will be set to 'dir_path'
    clicked = s2d::FontManager::displaySmybolAsButton(icon.c_str(), cursorPos);

    ImGui::SetCursorPos(old);
    return clicked;
}

std::string s2d::FileDialog::getEmptyStringBetween(const std::string& content, const std::string& name, float padding)
{
    std::string empty = "";

    float condi = ((ImGui::CalcTextSize(content.c_str()).x + (padding * 2)) - 1) - ImGui::CalcTextSize(name.c_str()).x;

    for (int i = 0; i < condi; i++)
    {
        if (ImGui::CalcTextSize(empty.c_str()).x >= condi)
        {
            break;
        }
        empty += " ";

    }
    return empty;
}
