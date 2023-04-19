#include "UIAssetFolder.h"

s2d::UIAssetFolder::UIAssetFolder()
{
    this->init();
}


void s2d::UIAssetFolder::init()
{
    this->m_tools = s2d::UIAssetTools(&this->currentPath);

    this->currentPath = s2d::EngineData::s_pathToUserProject + "\\assets";
    this->currentName = "assets";

    this->isHovered = false;
    this->m_interacted = false;
    this->m_draggingItem = false;
    this->m_hoveredOverItem = false;
    this->m_fileContentPadding = 15;

    this->m_windowSize = ImVec2(1280 + 250, 400);
    this->m_toHoverItemName = "";
    this->m_isAssetFolderTreeNodeOpen = true;
}
 
//Public functions

void s2d::UIAssetFolder::createAssetLinkerWindow()
{
    if (!s2d::UIInfo::s_isAssetFolderActive)
    {
        this->isHovered = false;
        return;
    }
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.1f);
    if (ImGui::Begin("Assets", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        this->resizeWindow();
        this->addPrefab();
        this->m_tools.update(isHovered);
        this->render();
        ImGui::End();
    }
    ImGui::PopStyleVar();

    if (this->m_ptr_repo->assetFolderData.dragAndDropPath != " ")
    {
        const ImVec2 cursor = ImGui::GetCursorPos();
        const ImVec2 pos = ImVec2(float(sf::Mouse::getPosition().x - 100), float(sf::Mouse::getPosition().y + 10));

        if (ImGui::Begin("##Drag", NULL, ImGuiWindowFlags_NoTitleBar))
        {
            ImGui::SetWindowPos(pos);
            ImGui::SetWindowFontScale(s2d::UIInfo::s_defaultFontSize);

            ImGui::Text(this->m_ptr_repo->assetFolderData.dragAndDropName.c_str());
            ImGui::End();
        }
    }
}

//private functions

void s2d::UIAssetFolder::render()
{
    this->renderFolderHierarchy();
    this->renderCloseRectangle();
    this->renderContentBrowser();

    this->isHovered = ImGui::IsWindowHovered(
        ImGuiHoveredFlags_AllowWhenBlockedByActiveItem
        | ImGuiHoveredFlags_AllowWhenBlockedByPopup
        | ImGuiHoveredFlags_ChildWindows);
}

void s2d::UIAssetFolder::renderContentBrowser()
{
    ImGui::SetCursorPos(ImVec2(UIASSET_FOLDER_WIDTH + FOLDER_HIERACHY_PADDING * 1.5f, FOLDER_HIERACHY_PADDING));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(FILE_DISPLAYER_COLOR / 255.0f, FILE_DISPLAYER_COLOR / 255.0f, FILE_DISPLAYER_COLOR / 255.0f, 255.0f));
    ImGui::BeginChild("##file-displayer-container", ImVec2(this->m_windowSize.x - (FOLDER_HIERACHY_PADDING * 1.8f + UIASSET_FOLDER_WIDTH), this->m_windowSize.y), false, ImGuiWindowFlags_NoScrollbar);

    this->goBackToBeforeFolder();
    ImGui::SetCursorPos(ImVec2(5, this->m_fileContentPadding));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.0f); // Set rounding to 5 pixels
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 3.0f)); // Add some padding for visual clarity
    this->m_fileFilter.Draw("Search", 200);
    ImGui::Dummy(ImVec2(10, 10));
    ImGui::PopStyleVar(2);

    this->beginColumns();
    this->getAllFilesInDir(this->currentPath.c_str(), this->currentName.c_str());
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::SetWindowPos(ImVec2(0, 1080 - this->m_windowSize.y));
    ImGui::SetWindowFontScale(s2d::UIInfo::s_defaultFontSize);
    ImGui::SetWindowSize(this->m_windowSize);

}

void s2d::UIAssetFolder::setCurrentPath(const std::string& path, const std::string& name)
{
    this->currentName = name;
    this->currentPath = path;
}

void s2d::UIAssetFolder::resizeWindow()
{
    bool popStyle = false;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 200);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
    if (this->m_clickedOnResizeButton)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 1));
        popStyle = true;
    }
    s2d::FontManager::displaySmybolAsButton(ICON_FA_ARROW_UP);
    if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
    {
        this->m_clickedOnResizeButton = true;
    }
    if (this->m_clickedOnResizeButton && ImGui::IsMouseDown(0))
    {
        float movedy = 0;
        if (s2d::UI::s_guiCorsor.posiitonChanged)
        {
            s2d::Vector2 moved = s2d::UI::s_guiCorsor.lastPos - s2d::UI::s_guiCorsor.position;
            movedy = moved.y;
        }
        this->m_windowSize.y += movedy;
    }
    else
    {
        this->m_clickedOnResizeButton = false;
    }
    if (popStyle)
    {
        ImGui::PopStyleColor();
    }
}

void s2d::UIAssetFolder::addPrefab()
{
    if (this->m_ptr_repo == nullptr)
    {
        return;
    }
    if (this->m_ptr_repo->child_to_parent != nullptr && ImGui::IsMouseReleased(0 && this->isHovered))
    {
        std::cout << "y";
        const std::string pathToFile = this->currentPath + "\\" + this->m_ptr_repo->child_to_parent->name + EXTENSION_PREFAB_FILE;

        this->m_ptr_repo->child_to_parent->prefab.updateProps(
            pathToFile, s2d::UI::getUserProjectPathSeperatetFromEnginePath(pathToFile), pathToFile, this->m_ptr_repo->child_to_parent->name + EXTENSION_PREFAB_FILE
        );
        s2d::flc::createOrUpdatePrefabFile(this->m_ptr_repo->child_to_parent, pathToFile, this->m_ptr_repo->child_to_parent->prefab.pathToOldFile);
    }
}

void s2d::UIAssetFolder::renderFolderHierarchy()
{
    ImGui::SetCursorPos(ImVec2(FOLDER_HIERACHY_PADDING, FOLDER_HIERACHY_PADDING));
    ImGui::BeginChild("##folder-hierarchy", ImVec2(UIASSET_FOLDER_WIDTH, this->m_windowSize.y), false);
    this->renderFolderHierarchyRecursiv(std::string(PATH_TO_USER_ASSET_FOLDER).c_str(), "Assets", this->m_isAssetFolderTreeNodeOpen);
    this->m_isAssetFolderTreeNodeOpen = false;
    ImGui::EndChild();
}

void s2d::UIAssetFolder::renderCloseRectangle()
{
    ImGui::SetCursorPosX(FOLDER_HIERACHY_PADDING);
    ImGui::SetCursorPosY(0);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.085f, 0.085f, 0.085f, 1.0f));
    ImGui::BeginChild("##close-rectangle-assets", CLOSE_RECTANGLE_SIZE);
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + CLOSE_RECTANGLE_INNER_PADDING, ImGui::GetCursorPosY() + CLOSE_RECTANGLE_INNER_PADDING));

    s2d::FontManager::displaySmybolAsText(ICON_FA_FILE_CODE);
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 30
        , ImGui::GetCursorPosY() - ImGui::CalcTextSize(ICON_FA_FILE_CODE).y - 1));
    ImGui::Text("Assets");
    ImGui::SetCursorPos(ImVec2(CLOSE_RECTANGLE_SIZE.x - 30,
        ImGui::GetCursorPosY() - ImGui::CalcTextSize("Assets").y - 10));
    if (ImGui::Button("x"))
    {
        s2d::UIInfo::s_isAssetFolderActive = false;
    }
    ImGui::PopStyleColor();
    ImGui::EndChild();
}

void s2d::UIAssetFolder::renderFolderHierarchyRecursiv(const char* path, const char* name, bool openNextTreeNode)
{
    struct dirent* entry;
    DIR* dir = opendir(path);
    if (dir == NULL) {
        return;
    }
    m_interacted = false;

    if (!s2d::FileDialog::checkIfADirHasSubDirs(path))
    {
        if (s2d::FontManager::displaySymbolInMenuItem(ICON_FA_FOLDER, name))
        {
            this->setCurrentPath(path, name);
        }
        return;
    }
    if (s2d::FontManager::displaySymbolInTreeNode(ICON_FA_FOLDER, name, openNextTreeNode))
    {
        if (ImGui::IsItemClicked())
        {
            this->setCurrentPath(path, name);
        }
        while ((entry = readdir(dir)) != NULL)
        {
            bool folder = true;
            const char* str = entry->d_name;

            std::string std_name(str);

            //Checks if the string has only chars like ../../ ..
            if (!std::isStringValid(std_name))
            {
                continue;
            }

            //We need to know if we got a folder or not for the recursion
            folder = std::isFolder(std_name);

            //Recursivly calling
            std::string newPath = std::string(path) + "\\" + std_name;

            if (folder)
            {
                const char* ch = newPath.c_str();
                renderFolderHierarchyRecursiv(ch, str, false);
            }
        }
        ImGui::TreePop();
    }
    closedir(dir);
}

void s2d::UIAssetFolder::renderFilesWithChildWindow(const std::string& name, const std::string& newPath, const std::string& entryPath, bool isFolder, uint32_t textureId, uint8_t columCnt)
{
    bool resetHoveredItem = false;
    float rounding = 10.0f;
    ImVec2 child_size = ImVec2(120, 220);

    // The multiplyer for the color (hover effect)
    int adder = 1;
    if (this->m_toHoverItemName == name)
    {
        this->m_toHoverItemName = "";
        this->m_isItemHovered = false;
        adder = 2;
    }
    const std::string fileChildWindow = "##" + std::string(entryPath);

    // Drawing a rounded window
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(adder + 0.095f, adder + 0.095f, adder + 0.095f, 1.0f));
    ImGui::BeginChild(fileChildWindow.c_str(), child_size, false, ImGuiWindowFlags_NoBackground);

    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    ImVec2 topLeft = windowPos;
    ImVec2 bottomRight = ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y);

    // Draw the rounded rectangle shape
    drawList->AddRectFilled(topLeft, bottomRight, ImColor(adder * 24, adder * 24, adder * 24), rounding);
    drawList->AddRect(topLeft, bottomRight, ImColor(0, 0, 0), rounding);

    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    if (ImGui::ImageButton(name.c_str(), (ImTextureID)textureId,
        ImVec2(120, 183)))
    {
        // Display the item if its a folder
        if (isFolder)
        {
            resetHoveredItem = true;
            this->setCurrentPath(newPath, entryPath);
        }
    }

    // Setting current item which is hovered
    if (m_toHoverItemName == "")
    {
        this->m_isItemHovered = ImGui::IsItemHovered();
        if (this->m_isItemHovered)
            m_toHoverItemName = name;
    }
    ImGui::PopStyleColor();

    // Make the file "selectable"
    if (!isFolder)
        this->setDragAndDrop(newPath, entryPath);

    // Adding - for to long names
    std::string withoutExt = std::removeExtension(entryPath);
    float textSizeX = ImGui::CalcTextSize(withoutExt.c_str()).x;
    std::string newTextWraped = "";
    if (textSizeX > 80)
    {
        // To big text size
        for (int i = 0; i < withoutExt.size() / 2; i++)
        {
            newTextWraped += withoutExt[i];
        }
        newTextWraped += " -\n";
        for (int i = withoutExt.size() / 2; i < withoutExt.size(); i++)
        {
            newTextWraped += withoutExt[i];
        }
    }
    else
    {
        newTextWraped = withoutExt;
    }

    // Info text at the bottom
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 20, ImGui::GetCursorPosY() - 80));
    ImGui::TextWrapped(newTextWraped.c_str());
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
    ImGui::SetCursorPosY(0 + child_size.y - 35);
    ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), std::getFileExtension(entryPath).c_str());
    ImGui::EndChild();
    ImGui::PopStyleColor();

    // Setting spacing for rows
    if (columCnt >= MAX_COLUMNS)
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + PADDING_BETWEEN_ROWS);
    }
    this->m_isItemHovered = false;

    if (resetHoveredItem)
    {
        this->m_toHoverItemName = "";
    }

    ImGui::NextColumn();
}

void s2d::UIAssetFolder::getAllFilesInDir(const char* path, const char* name)
{
    uint8_t cnt = 0;

    struct dirent* entry;
    DIR* dir = opendir(path);
    if (dir == NULL) {
        return;
    }
    this->m_interacted = false;
    while ((entry = readdir(dir)) != NULL)
    {
        cnt++;
        const char* str = entry->d_name;
        const std::string std_name(str);
        const ImVec2 textSize = ImGui::CalcTextSize(str);
        float itemWidth = ICONS_SIZE;

        // Checks if the string has only chars like ../../ ..
        if (!std::isStringValid(std_name))
        {
            continue;
        }

        const std::string icon = std::getFileExtension(std_name);
        const std::string newPath = std::string(path) + "\\" + std_name;
        const std::string name = "##" + std::string(str);
        const uint32_t id = this->m_data.getId(icon);
        const bool isFolder = (icon == "folder");

        if (this->m_fileFilter.PassFilter(name.c_str()))
        {
            this->renderFilesWithChildWindow(name, newPath, std_name, isFolder, id, cnt);
        }
    }

    closedir(dir);
}

void s2d::UIAssetFolder::goBackToBeforeFolder()
{
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 300, ImGui::GetCursorPosY() + 10));
    const std::vector<std::string> pathParts = std::splitString(this->currentPath, "\\");
    std::vector<std::string> validParts;
    for (int i = 0; i < pathParts.size(); i++)
    {
        if (std::isStringValid(pathParts[i]) && pathParts[i] != "Assets" && pathParts[i] != s2d::EngineData::s_nameOfUserProject)
        {
            validParts.push_back(pathParts[i]);
        }
    }

    for (int i = 0; i < validParts.size(); i++)
    {
        bool popStyle = false;
        // Set current path to the folder clicked
        if (this->currentName == validParts[i])
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 1));
            popStyle = true;
        }
        if (ImGui::Button(validParts[i].c_str()) && this->currentName != validParts[i])
        {
            this->currentPath = "";
            this->currentName = validParts[i];
            currentPath = s2d::EngineData::s_pathToUserProject;
            for (int j = 0; j <= i; j++)
            {
                 currentPath += + "\\" + validParts[j];
            }

            break;
        }
        if (popStyle)
        {
            ImGui::PopStyleColor();
        }
        ImGui::SameLine();
        if (i != validParts.size() - 1)
            ImGui::Text(">");
        ImGui::SameLine();
    }
    

}

void s2d::UIAssetFolder::setDragAndDrop(std::string path, std::string name)
{
    //Check if we hover over the menu item used later on for drag and drop

    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0) && !this->m_interacted && this->m_ptr_repo->assetFolderData.dragAndDropPath == " ")
    {
        this->m_draggingItem = true;
        this->m_ptr_repo->assetFolderData.dragAndDropPath = path;
        this->m_ptr_repo->assetFolderData.dragAndDropName = name;
    }
    if (ImGui::IsMouseReleased(0))
    {
        this->m_interacted = true;
        this->m_draggingItem = false;
        this->m_hoveredOverItem = false;
        this->m_ptr_repo->assetFolderData.dragAndDropPath = " ";
    }
}

void s2d::UIAssetFolder::beginColumns()
{
    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = (int)(panelWidth / PADDING_BETWEEN_COLUMS);

    if (columnCount < 1)
    {
        columnCount = 1;

    }
    ImGui::Columns(columnCount, 0, false);
}