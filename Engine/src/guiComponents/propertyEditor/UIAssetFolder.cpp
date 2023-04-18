#include "UIAssetFolder.h"

s2d::UIAssetFolder::UIAssetFolder()
{
    this->init();
}

void s2d::UIAssetFolder::init()
{
    this->m_tools = s2d::UIAssetTools(&this->currentPath);

    this->currentPath = s2d::EngineData::s_pathToUserProject + "\\assets";
    this->currentName = "Assets";
    this->m_iconSize = 70;
    this->m_padding = 130;

    this->isHovered = false;
    this->m_interacted = false;
    this->m_draggingItem = false;
    this->m_hoveredOverItem = false;
    this->m_fileContentPadding = 15;

    this->m_windowSize = ImVec2(1280 + 250, 400);
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
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar))
    {
        this->render();
        this->addPrefab();
        this->m_tools.update(isHovered);
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

    ImGui::SetCursorPos(ImVec2(UIASSET_FOLDER_WIDTH + FOLDER_HIERACHY_PADDING * 1.5f, FOLDER_HIERACHY_PADDING));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(FILE_DISPLAYER_COLOR / 255.0f, FILE_DISPLAYER_COLOR / 255.0f, FILE_DISPLAYER_COLOR / 255.0f, 255.0f));
    ImGui::BeginChild("##file-displayer", ImVec2(this->m_windowSize.x - (FOLDER_HIERACHY_PADDING * 1.8f + UIASSET_FOLDER_WIDTH), this->m_windowSize.y ), false, ImGuiWindowFlags_NoScrollbar);
    
     /*   this->goBackToBeforeFolder();*/
    ImGui::SetCursorPos(ImVec2(this->m_fileContentPadding + 10, this->m_fileContentPadding));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.0f); // Set rounding to 5 pixels
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 3.0f)); // Add some padding for visual clarity
    this->m_fileFilter.Draw("Search", 200);
    ImGui::Dummy(ImVec2(0, 10));
    ImGui::PopStyleVar(2);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + this->m_fileContentPadding);
    this->beginColumns();
    this->getAllFilesInDir(this->currentPath.c_str(), this->currentName.c_str());

    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::SetWindowPos(ImVec2(0, 1080 - this->m_windowSize.y));
    ImGui::SetWindowFontScale(s2d::UIInfo::s_defaultFontSize);
    ImGui::SetWindowSize(this->m_windowSize);

    this->isHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem
        | ImGuiHoveredFlags_AllowWhenBlockedByPopup
        | ImGuiHoveredFlags_ChildWindows);
}


void s2d::UIAssetFolder::addPrefab()
{
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
    ImGui::BeginChild("##folder-hierarchy", ImVec2(UIASSET_FOLDER_WIDTH, this->m_windowSize.y));
    this->renderFolderHierarchyRecursiv(std::string(PATH_TO_USER_ASSET_FOLDER).c_str(), "Assets");
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

void s2d::UIAssetFolder::renderFolderHierarchyRecursiv(const char* path, const char* name)
{
    struct dirent* entry;
    DIR* dir = opendir(path);
    if (dir == NULL) {
        return;
    }
    m_interacted = false;

    if (!s2d::FileDialog::checkIfADirHasSubDirs(path))
    {
        s2d::FontManager::displaySymbolInMenuItem(ICON_FA_FOLDER, name);
        return;
    }
    if (s2d::FontManager::displaySymbolInTreeNode(ICON_FA_FOLDER, name))
    {
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
                renderFolderHierarchyRecursiv(ch, str);
            }
        }
        ImGui::TreePop();
    }
    closedir(dir);
}

void s2d::UIAssetFolder::getAllFilesInDir(const char* path, const char* name)
{
    struct dirent* entry;
    DIR* dir = opendir(path);
    if (dir == NULL) {
        return;
    }
    this->m_interacted = false;

    while ((entry = readdir(dir)) != NULL)
    {
        const char* str = entry->d_name;
        const std::string std_name(str);
        const ImVec2 textSize = ImGui::CalcTextSize(str);
        float itemWidth = float(this->m_iconSize);

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
      
            ImVec2 child_size = ImVec2(120, 200);

            const std::string fileChildWindow = "##" + std::string(entry->d_name);
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.095f, 0.095f, 0.095f, 1.0f));
            ImGui::BeginChild(fileChildWindow.c_str(), child_size, false, ImGuiWindowFlags_NoBackground);
            
            ImVec2 windowPos = ImGui::GetWindowPos();
            ImVec2 windowSize = ImGui::GetWindowSize();

            ImDrawList* drawList = ImGui::GetWindowDrawList();
            float rounding = 10.0f; // set the rounding value

            ImVec2 topLeft = windowPos;
            ImVec2 bottomRight = ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y);

            // Draw the rounded rectangle shape
            drawList->AddRectFilled(topLeft, bottomRight, ImColor(24, 24, 24), rounding);
            drawList->AddRect(topLeft, bottomRight, ImColor(0, 0, 0), rounding);


            if (ImGui::ImageButton(name.c_str(), (ImTextureID)id, 
                ImVec2(120, 185)))
            {
                // Display the item if its a folder
                if (isFolder)
                {
                    this->currentPath = newPath;
                    this->currentName = str;
                }
            }
            if (!isFolder)
                this->setDragAndDrop(newPath, str);

            ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 20, ImGui::GetCursorPosY() - 80));
            ImGui::TextWrapped(std::removeExtension(str).c_str());
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
            ImGui::SetCursorPosY(0 + child_size.y - 30);
            ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), std::getFileExtension(str).c_str());
            ImGui::EndChild(); 
            ImGui::PopStyleColor();

            ImGui::NextColumn();
        }
    }

    closedir(dir);
}

void s2d::UIAssetFolder::goBackToBeforeFolder()
{
    auto split = [](const std::string& str, char delimiter)
    {
        std::vector<std::string> tokens;
        std::string::size_type start = 0;
        std::string::size_type end = 0;
        while ((end = str.find_first_of(delimiter, start)) != std::string::npos) {
            tokens.push_back(str.substr(start, end - start));
            start = str.find_first_not_of(delimiter, end);
        }
        if (start != std::string::npos) {
            tokens.push_back(str.substr(start));
        }
        return tokens;
    };


    std::vector<std::string> props;
    props = split(this->currentPath, '\\');

    for (int i = 0; i < props.size(); i++)
    {
        // Alot of hard code path shit done here, gonna change in future
        if (props[i] == "#")
        {
            break;
        }
        if (props[i] == ".." || props[i] == "Assets")
        {
            continue;
        }


        // Set current path to the folder clicked
        if (ImGui::Button(props[i].c_str()))
        {
            this->currentPath = "";
            this->currentName = props[i];
            for (int j = 0; j <= i; j++)
            {
                currentPath += (j != i) ? props[j] + "\\" : props[j];
            }

            break;
        }

        ImGui::SameLine();
        if (i != props.size() - 1)
            ImGui::Text(">");
        ImGui::SameLine();
    }
    ImGui::Dummy(ImVec2(0, 30));
    ImGui::Separator();

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
    int columnCount = (int)(panelWidth / this->m_padding);

    if (columnCount < 1)
    {
        columnCount = 1;
    }

    ImGui::Columns(columnCount, 0, false);
}