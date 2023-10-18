#include "folderDialog.h"

// Constructor

s2d::FolderDialog::FolderDialog()
{
    this->m_closeWindow = false;
    this->m_displayTitle = true;
    this->pathClicked = "";
    this->folderClicked = "";
    this->windowFocus = true;
    this->is_open = false;
}

s2d::FolderDialog::FolderDialog(std::string path, std::string icon, std::string title, ImVec2 windowSize)
{
    this->m_firstNodeText = path;
    this->m_path = path;
    this->m_windowSize = windowSize;
    this->m_title = title;
    this->m_displayTitle = true;
    this->pathClicked = "";
    this->folderClicked = "";
    this->m_icon = icon;
    this->windowFocus = true;
    this->is_open = false;
}

// Public methods

void s2d::FolderDialog::disableWindow()
{
    this->pathClicked = "";
    this->windowFocus = true;
    this->m_displayTitle = true;
    this->folderClicked = "";
    this->m_closeWindow = true;
    this->is_open = false;
}

void s2d::FolderDialog::displayNodes()
{
    if (this->m_closeWindow)
        return;
    ImGui::Begin("##FolderDialoge", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);

    this->m_displayTitle = true;
    auto displayTitle = [](const std::string& title)
    {
        const float PADDING_Y = 5;
        ImGui::Text(title.c_str());
        ImGui::Dummy(ImVec2(0, PADDING_Y));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, PADDING_Y));
    };


    std::string name = "x" + std::string("##") + this->m_path;
    ImVec2 old = ImGui::GetCursorPos();
    ImVec2 closeCursorPos = ImVec2(ImGui::GetCursorPosX() + this->m_windowSize.x - 60, ImGui::GetCursorPosY() - 5);
    ImGui::SetCursorPos(closeCursorPos);

    // Clicked at the "x", stop displaying the file dialoge
    if (ImGui::Button(name.c_str()))
    {
        this->disableWindow();
    }

    ImGui::SetCursorPos(old);

    if (this->m_displayTitle)
    {
        displayTitle(this->m_title);
        this->m_displayTitle = false;

        const std::string BUTTON_NAME = this->m_icon + "##" + this->m_path;

        // clicked on the icon of the right
        if (this->displaySymbol(BUTTON_NAME, this->m_windowSize.x))
        {
            this->pathClicked = this->m_path;
        }
        ImGui::SetNextItemOpen(true);

        if (ImGui::TreeNode(this->m_firstNodeText.c_str()))
        {
            this->openFile(this->m_path.c_str());
            ImGui::TreePop();
        }
    }

    ImGui::SetWindowSize(this->m_windowSize);
    ImGui::End();
}


void s2d::FolderDialog::enableWindow()
{
    this->windowFocus = true;
    this->m_closeWindow = false;
    this->is_open = true;
}


void s2d::FolderDialog::enableWindow(const std::string& title)
{
    this->enableWindow();
    this->m_title = title;
}

void s2d::FolderDialog::update()
{
    if (this->is_open)
    {
        if (this->pathClicked == ""
            && !this->isWindowClosed())
        {
            this->enableWindow();
            this->displayNodes();
        }
    }
}

bool s2d::FolderDialog::IsItemSelected()
{
    return this->pathClicked != "";
}

bool s2d::FolderDialog::isWindowClosed()
{
    return this->m_closeWindow;
}

// private methods

void s2d::FolderDialog::openFile(const char* dir_path)
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
        std::string path = dir_path + std::string(entry->d_name) + "\\";

        // If the entry is a directory, recursively traverse it
        if (entry->d_type == DT_DIR)
        {
            // clicked on the icon of the right
            const std::string BUTTON_NAME = this->m_icon + "##" + path;

            if (this->displaySymbol(BUTTON_NAME, this->m_windowSize.x))
            {
                this->pathClicked = path;
                this->folderClicked = entry->d_name;
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
    this->m_displayTitle = true;
}

// static methods

bool s2d::FolderDialog::displaySymbol(const std::string& icon, float windowSizeX)
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

std::string s2d::FolderDialog::getEmptyStringBetween(const std::string& content, const std::string& name, float padding)
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

bool s2d::FolderDialog::checkIfADirHasSubDirs(const std::string& dirPath)
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

