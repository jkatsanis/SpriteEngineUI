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
    this->m_iconSize = 75;
    this->m_padding = 130;

    this->isHovered = false;
    this->m_interacted = false;
    this->m_draggingItem = false;
    this->m_hoveredOverItem = false;

    this->m_windowSize = ImVec2(1280 + 250, 350);
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
    if (ImGui::Begin("Assets", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
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
    ImGui::BeginChild("##file-displayer");

 /*   this->goBackToBeforeFolder();
    this->beginColumns();
    this->getAllFilesInDir(this->currentPath.c_str(), this->currentName.c_str());*/

    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::SetWindowPos(ImVec2(0, 730));
    ImGui::SetWindowFontScale(0.8f);
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
    float adder = 95 + FOLDER_HIERACHY_PADDING;
    if (*this->m_ptr_repo->isFullScreened)
    {
        adder = FOLDER_HIERACHY_PADDING;
    }
    ImGui::SetCursorPos(ImVec2(FOLDER_HIERACHY_PADDING, FOLDER_HIERACHY_PADDING));
    ImGui::BeginChild("##folder-hierarchy", ImVec2(UIASSET_FOLDER_WIDTH, this->m_windowSize.y - adder));
    this->renderFolderHierarchyRecursiv(std::string(PATH_TO_USER_ASSET_FOLDER).c_str(), "Assets");
    ImGui::EndChild();
}

void s2d::UIAssetFolder::renderCloseRectangle()
{
    ImGui::SetCursorPosX(FOLDER_HIERACHY_PADDING);
    ImGui::SetCursorPosY(0);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.085, 0.085, 0.085, 1));
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

        //Checks if the string has only chars like ../../ ..
        if (!std::isStringValid(std_name))
        {
            continue;
        }

        const std::string icon = std::getFileExtension(std_name);
        const std::string newPath = std::string(path) + "\\" + std_name;
        const std::string name = "##" + std::string(str);
        const ImTextureID id = this->m_data.getId(icon);
        const bool isFolder = (icon == "folder");

        if (ImGui::ImageButton(name.c_str(), id, ImVec2(float(this->m_iconSize), float(this->m_iconSize))))
        {
            //Display the item if its a folder
            if (isFolder)
            {
                this->currentPath = newPath;
                this->currentName = str;
            }
        }
        if (!isFolder)
            this->setDragAndDrop(newPath, str);

        ImGui::TextWrapped(str);
        ImGui::NextColumn();
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