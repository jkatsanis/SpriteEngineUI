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
    fs::path dir(dir_path);
    if (!fs::exists(dir) || !fs::is_directory(dir))
        return;

    for (const auto& entry : fs::directory_iterator(dir))
    {
        const auto& path = entry.path();
        const std::string filename = path.filename().string();

        // Skip "." and ".." and files containing '$'
        if (filename == "." || filename == ".." || filename.find('$') != std::string::npos)
            continue;

        if (filename == "cheses")
            return;

        const std::string BUTTON_NAME = this->m_Icon + "##" + path.string();

        bool isDir = entry.is_directory();

        // Show button for directories or files
        if ((isDir && filename[0] != '.') || this->m_ShowFiles)
        {
            if (this->DisplaySymbol(BUTTON_NAME, this->m_WindowSize.x))
            {
                this->PathClicked = path.string();
                this->ItemClicked = filename;
            }
        }

        // Display folder icon
        if (isDir && filename[0] != '.')
        {
            if (!spe::FileDialog::CheckIfADirHasSubItems(path.string(), this->m_ShowFiles))
            {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                spe::Style::DisplaySymbolInMenuItemWithText(ICON_FA_FOLDER, filename.c_str(), 30);
            }
            else if (spe::Style::DisplaySymbolInTreeNode(ICON_FA_FOLDER, filename.c_str(), false))
            {
                OpenFile(path.string().c_str());  // Recursive call
                ImGui::TreePop();
            }
        }
        else if (this->m_ShowFiles)  // Display file icon
        {
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
            spe::Style::DisplaySymbolInMenuItemWithText(ICON_FA_FILE, filename.c_str(), 30);
        }
    }

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
    fs::path dir(dirPath);

    if (!fs::exists(dir) || !fs::is_directory(dir))
        return false;

    try {
        for (const auto& entry : fs::directory_iterator(dir)) {
            const std::string name = entry.path().filename().string();

            if (name == "." || name == "..") {
                continue;
            }

            if (!show_files) {
                if (entry.is_directory()) {
                    return true;
                }
            }
            else {
                return true;
            }
        }
    }
    catch (...) {
        return false;
    }


    return false;  // No matching items found
}
