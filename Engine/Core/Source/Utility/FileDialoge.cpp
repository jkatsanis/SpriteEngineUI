#include "FileDialoge.h"

// Constructor

spe::FileDialog::FileDialog()
{
    this->m_ShowFiles = true;
    this->m_IsOpen = false;
    this->m_CloseWindow = false;
    this->m_DisplayTitle = true;
    this->PathClicked = "";
    this->ItemClicked = "";
    this->WindowFocus = true;
    this->m_FontScale = 1;
    this->DisableWindow();
}

spe::FileDialog::FileDialog(std::string path, std::string icon, std::string title, ImVec2 windowSize, bool show_file, float fontscale)
{
    this->m_CloseWindow = false;
    this->m_FirstNodeText = path;
    this->m_Path = path;
    this->m_WindowSize = windowSize;
    this->m_Title = title;
    this->m_DisplayTitle = true;
    this->PathClicked = "";
    this->ItemClicked = "";
    this->m_Icon = icon;
    this->WindowFocus = true;
    this->m_IsOpen = false;
    this->m_ShowFiles = show_file;
    this->m_FontScale = fontscale;
    this->DisableWindow();
}

// Public methods

void spe::FileDialog::DisableWindow()
{
    this->PathClicked = "";
    this->WindowFocus = true;
    this->m_DisplayTitle = true;
    this->ItemClicked = "";
    this->m_CloseWindow = true;
}

void spe::FileDialog::DisplayNodes()
{
    if (this->m_CloseWindow)
        return;
    ImGui::Begin("##FileDialoge", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(this->m_FontScale);

    this->m_DisplayTitle = true;
    auto displayTitle = [](const std::string& title)
    {
        const float PADDING_Y = 5;
        ImGui::Text(title.c_str());
        ImGui::Dummy(ImVec2(0, PADDING_Y));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, PADDING_Y));
    };


    std::string name = "x" + std::string("##") + this->m_Path;
    ImVec2 old = ImGui::GetCursorPos();
    ImVec2 closeCursorPos = ImVec2(ImGui::GetCursorPosX() + this->m_WindowSize.x - 60, ImGui::GetCursorPosY() - 5);
    ImGui::SetCursorPos(closeCursorPos);

    // Clicked at the "x", stop displaying the file dialoge
    if (ImGui::Button(name.c_str()))
    {
        this->DisableWindow();
    }

    ImGui::SetCursorPos(old);

    if (this->m_DisplayTitle)
    {
        displayTitle(this->m_Title);
        this->m_DisplayTitle = false;

        const std::string BUTTON_NAME = this->m_Icon + "##" + this->m_Path;

        // clicked on the icon of the right
        if (this->DisplaySymbol(BUTTON_NAME, this->m_WindowSize.x))
        {
            this->PathClicked = this->m_Path;
        }
        if (spe::Style::DisplaySymbolInTreeNode(ICON_FA_FOLDER, this->m_FirstNodeText, true))
        {
            this->OpenFile(this->m_Path.c_str());
            ImGui::TreePop();
        }
    }
    ImGui::SetWindowFontScale(this->m_FontScale);

    ImGui::SetWindowSize(this->m_WindowSize);
    ImGui::End();
}

void spe::FileDialog::EnableWindow()
{
    this->WindowFocus = true;
    this->m_CloseWindow = false;
    this->m_IsOpen = true;
}


void spe::FileDialog::EnableWindow(const std::string& title)
{
    this->EnableWindow();
    this->m_Title = title;
}
void spe::FileDialog::Update()
{
    if (this->m_IsOpen)
    {
        if (this->PathClicked == ""
            && !this->IsWindowClosed())
        {
            this->EnableWindow();
            this->DisplayNodes();
        }
    }
}

bool spe::FileDialog::IsItemSelected()
{
    return this->PathClicked != "";
}


// private methods

void spe::FileDialog::OpenFile(const char* dir_path)
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
        const std::string BUTTON_NAME = this->m_Icon + "##" + path;

        // TODO: May be wrong and need some work
        if(entry->d_type == DT_DIR && entry->d_name[0] != '.'
            || this->m_ShowFiles)
        {
            if (this->DisplaySymbol(BUTTON_NAME, this->m_WindowSize.x))
            {
                this->PathClicked = path;
                this->ItemClicked = entry->d_name;
            }
        }

        std::string name = " " + std::string(entry->d_name);

        if (entry->d_type == DT_DIR && entry->d_name[0] != '.')
        {
            if (!spe::FileDialog::CheckIfADirHasSubItems(path, this->m_ShowFiles))
            {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                spe::Style::DisplaySymbolInMenuItemWithText(ICON_FA_FOLDER, entry->d_name, 30);
            }

            else if (spe::Style::DisplaySymbolInTreeNode(ICON_FA_FOLDER, entry->d_name, false))
            {
                path += "\\";
                OpenFile(path.c_str());
                ImGui::TreePop();
            }

        }
        else if (m_ShowFiles)
        {
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);

            spe::Style::DisplaySymbolInMenuItemWithText(ICON_FA_FILE, entry->d_name, 30);
        }
    }

    // Close the directory
    closedir(dir);
    this->m_DisplayTitle = true;
}

// static methods

bool spe::FileDialog::DisplaySymbol(const std::string& icon, float windowSizeX)
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


std::string spe::FileDialog::GetEmptyStringBetween(const std::string& content, const std::string& name, float padding)
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

bool spe::FileDialog::CheckIfADirHasSubItems(const std::string& dirPath, bool show_files)
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

