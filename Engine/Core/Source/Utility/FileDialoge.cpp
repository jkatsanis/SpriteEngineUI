#include "FileDialoge.h"

// Constructor

spe::FileDialog::FileDialog()
{
    this->m_show_files = true;
    this->m_is_open = false;
    this->m_closeWindow = false;
    this->m_displayTitle = true;
    this->pathClicked = "";
    this->itemClicked = "";
    this->windowFocus = true;
    this->m_FontScale = 1;
    this->disableWindow();
}

spe::FileDialog::FileDialog(std::string path, std::string icon, std::string title, ImVec2 windowSize, bool show_file, float fontscale)
{
    this->m_closeWindow = false;
    this->m_firstNodeText = path;
    this->m_path = path;
    this->m_windowSize = windowSize;
    this->m_title = title;
    this->m_displayTitle = true;
    this->pathClicked = "";
    this->itemClicked = "";
    this->m_icon = icon;
    this->windowFocus = true;
    this->m_is_open = false;
    this->m_show_files = show_file;
    this->m_FontScale = fontscale;
    this->disableWindow();
}

// Public methods

void spe::FileDialog::disableWindow()
{
    this->pathClicked = "";
    this->windowFocus = true;
    this->m_displayTitle = true;
    this->itemClicked = "";
    this->m_closeWindow = true;
}

void spe::FileDialog::displayNodes()
{
    if (this->m_closeWindow)
        return;
    ImGui::Begin("##FileDialoge", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(this->m_FontScale);

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
        if (spe::Style::DisplaySymbolInTreeNode(ICON_FA_FOLDER, this->m_firstNodeText, true))
        {
            this->openFile(this->m_path.c_str());
            ImGui::TreePop();
        }
    }
    ImGui::SetWindowFontScale(this->m_FontScale);

    ImGui::SetWindowSize(this->m_windowSize);
    ImGui::End();
}

void spe::FileDialog::enableWindow()
{
    this->windowFocus = true;
    this->m_closeWindow = false;
    this->m_is_open = true;
}


void spe::FileDialog::enableWindow(const std::string& title)
{
    this->enableWindow();
    this->m_title = title;
}
void spe::FileDialog::update()
{
    if (this->m_is_open)
    {
        if (this->pathClicked == ""
            && !this->isWindowClosed())
        {
            this->enableWindow();
            this->displayNodes();
        }
    }
}

bool spe::FileDialog::IsItemSelected()
{
    return this->pathClicked != "";
}


// private methods

void spe::FileDialog::openFile(const char* dir_path)
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
        if (std::string(entry->d_name) == "cheses")
        {
            return;
        }

        // Compute the full path of the entry
        std::string path = dir_path + std::string(entry->d_name);

        // If the entry is a directory, recursively traverse it

        // clicked on the icon of the right
        const std::string BUTTON_NAME = this->m_icon + "##" + path;

        // TODO: May be wrong and need some work
        if(entry->d_type == DT_DIR && entry->d_name[0] != '.'
            || this->m_show_files)
        {
            if (this->displaySymbol(BUTTON_NAME, this->m_windowSize.x))
            {
                this->pathClicked = path;
                this->itemClicked = entry->d_name;
            }
        }

        std::string name = " " + std::string(entry->d_name);

        if (entry->d_type == DT_DIR && entry->d_name[0] != '.')
        {
            if (!spe::FileDialog::checkIfADirHasSubItems(path, this->m_show_files))
            {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                spe::Style::DisplaySymbolInMenuItemWithText(ICON_FA_FOLDER, entry->d_name, 30);
            }

            else if (spe::Style::DisplaySymbolInTreeNode(ICON_FA_FOLDER, entry->d_name, false))
            {
                path += "\\";
                openFile(path.c_str());
                ImGui::TreePop();
            }

        }
        else if (m_show_files)
        {
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);

            spe::Style::DisplaySymbolInMenuItemWithText(ICON_FA_FILE, entry->d_name, 30);
        }
    }

    // Close the directory
    closedir(dir);
    this->m_displayTitle = true;
}

// static methods

bool spe::FileDialog::displaySymbol(const std::string& icon, float windowSizeX)
{

    bool clicked = false;
    ImVec2 old = ImGui::GetCursorPos();
    ImVec2 cursorPos = ImVec2(windowSizeX - 55, ImGui::GetCursorPosY() - 6);

    // The icon right of the window displayed, when it got clicked the
    // 'pathClicked' prop will be set to 'dir_path'
    clicked = spe::Style::DisplaySmybolAsButton(icon.c_str(), cursorPos);

    ImGui::SetCursorPos(old);
    return clicked;
}


std::string spe::FileDialog::getEmptyStringBetween(const std::string& content, const std::string& name, float padding)
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

bool spe::FileDialog::checkIfADirHasSubItems(const std::string& dirPath, bool show_files)
{
    bool value = false;
    DIR* dir = opendir(dirPath.c_str());
    if (dir == NULL)
        return false;

    // Read each entry in the directory
    dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (!show_files)
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
        else
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

