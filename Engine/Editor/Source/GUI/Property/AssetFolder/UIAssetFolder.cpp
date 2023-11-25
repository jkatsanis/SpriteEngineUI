#include "UIAssetFolder.h"

void spe::UIAssetFolder::Init()
{
    this->m_tools = spe::UIAssetTools(&this->m_current_path, &this->m_to_hover_item_name);

    this->m_current_path = USER_FOLDER_NAME;
    this->m_current_name = USER_FOLDER_NAME;

    this->m_interacted = false;
    this->m_dragging_item = false;
    this->m_hovered_over_item = false;
    this->m_file_content_padding = 15;

    this->m_Size = ASSET_FOLDER_DEFAULT_WINDOW_SIZE;
    this->m_to_hover_item_name = "";
    this->m_is_asset_folder_tree_node_open = true;

    this->m_ptr_GUIRepo->AssetFolderData.ptr_Size = &this->m_Size;
}
 
//Public functions

void spe::UIAssetFolder::Render()
{
    float temp = ((this->m_ptr_GUIRepo->InspectorData.ptr_Size->x) - 390);
    this->m_Size.x = ASSET_FOLDER_DEFAULT_WINDOW_SIZE.x - temp;
    if (spe::UIUtility::HandleCloseAndReloadWindow(this->m_ptr_GUIRepo->InspectorData, this->Hovered, ASSET_FOLDER_DEFAULT_WINDOW_SIZE))
    {
        return;
    }


    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.1f);
    if (ImGui::Begin(USER_FOLDER_NAME, NULL,
        DEFAULT_FLAGS
        | ImGuiWindowFlags_NoScrollbar 
        | ImGuiWindowFlags_NoScrollWithMouse))
    {
        if (this->Hovered)
        {
            this->addPrefab();
            this->m_tools.update();
        }
        else
        {
            this->m_to_hover_item_name = "";
        }
        this->resizeWindow();

        this->renderFolderHierarchy();
        this->renderCloseRectangle();
        this->renderContentBrowser();

        this->Hovered = spe::UIUtility::IsHovered(ImVec2(0, 1080 - this->m_Size.y), this->m_Size);

        ImGui::End();
    }
    ImGui::PopStyleVar();

    if (this->m_ptr_GUIRepo->DragAndDropPath != " ")
    {
        if (ImGui::Begin("##Drag-Path", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar))
        {
            const ImVec2 window_pos = ImGui::GetMousePos();
            ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
            ImGui::Text(this->m_ptr_GUIRepo->DragAndDropName.c_str());
            ImGui::SetWindowPos(ImVec2(window_pos.x - 15, window_pos.y - 15));
            ImGui::End();
        }
    }
}

//private functions
 
void spe::UIAssetFolder::renderContentBrowser()
{
    ImGui::SetCursorPos(ImVec2(UIASSET_FOLDER_WIDTH + FOLDER_HIERARCHY_PADDING * 1.5f, FOLDER_HIERARCHY_PADDING));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(FILE_DISPLAYER_COLOR / 255.0f, FILE_DISPLAYER_COLOR / 255.0f, FILE_DISPLAYER_COLOR / 255.0f, 255.0f));
    ImGui::BeginChild("##file-displayer-container", ImVec2(this->m_Size.x - (FOLDER_HIERARCHY_PADDING * 1.8f + UIASSET_FOLDER_WIDTH), this->m_Size.y), false, ImGuiWindowFlags_NoScrollbar);

    this->goBackToBeforeFolder();
    ImGui::SetCursorPos(ImVec2(5, this->m_file_content_padding));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.0f); // Set rounding to 5 pixels
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 3.0f)); // Add some padding for visual clarity
    this->m_file_filter.Draw("Search", 200);
    ImGui::Dummy(ImVec2(10, 10));
    ImGui::PopStyleVar(2);

    this->beginColumns();
    this->getAllFilesInDir(this->m_current_path.c_str(), this->m_current_name.c_str());
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::SetWindowPos(ImVec2(0, 1080 - this->m_Size.y));
    ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
    ImGui::SetWindowSize(this->m_Size);

}

void spe::UIAssetFolder::setCurrentPath(const std::string& path, const std::string& name)
{
    this->m_current_name = name;
    this->m_current_path = path;
}

void spe::UIAssetFolder::resizeWindow()
{
    bool pop_style = false;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 200);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
    if (this->m_resize_window_data.clicked_on_resize_button)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 1));
        pop_style = true;
    }
    spe::Style::DisplaySmybolAsButton(ICON_FA_ARROW_UP);
    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
    {
        this->m_resize_window_data.additinal_add = 1080 - spe::UIUtility::GUICursor.y - this->m_Size.y;
        this->m_resize_window_data.clicked_on_resize_button = true;
    }
    if (this->m_resize_window_data.clicked_on_resize_button && ImGui::IsMouseDown(0))
    {
        const float new_size = 1080 - spe::UIUtility::GUICursor.y - this->m_resize_window_data.additinal_add;
        if (new_size > 150
            && new_size < 1031)
        {
            this->m_Size.y = new_size;
        }
    }
    else
    {
        this->m_resize_window_data.clicked_on_resize_button = false;
    }
    if (pop_style)
    {
        ImGui::PopStyleColor(2);
    }

}

void spe::UIAssetFolder::addPrefab()
{
    if (this->m_ptr_Repo == nullptr)
    {
        return;
    }
    if (this->m_ptr_GUIRepo->child_to_parent != nullptr && ImGui::IsMouseReleased(0) && this->Hovered)
    {
        const std::string pathToFile = this->m_current_path + "\\" + this->m_ptr_GUIRepo->child_to_parent->name + EXTENSION_PREFAB_FILE;
        
        this->m_ptr_GUIRepo->child_to_parent->prefab.updateProps(pathToFile, pathToFile, this->m_ptr_GUIRepo->child_to_parent->name + EXTENSION_PREFAB_FILE);

        spe::Savesystem::CreateOrUpdatePrefabFile(this->m_ptr_GUIRepo->child_to_parent, pathToFile, pathToFile);
    }
}

void spe::UIAssetFolder::renderFolderHierarchy()
{
    ImGui::SetCursorPos(ImVec2(FOLDER_HIERARCHY_PADDING, FOLDER_HIERARCHY_PADDING));
    ImGui::BeginChild("##folder-hierarchy", ImVec2(UIASSET_FOLDER_WIDTH, this->m_Size.y), false);
    this->renderFolderHierarchyRecursiv("Assets", "assets", this->m_is_asset_folder_tree_node_open);
    this->m_is_asset_folder_tree_node_open = false;
    ImGui::EndChild();
}

void spe::UIAssetFolder::renderCloseRectangle()
{
    this->m_ptr_GUIRepo->AssetFolderData.IsOpen = spe::UIUtility::RenderCloseRectangle(
        FOLDER_HIERARCHY_PADDING, ICON_FA_FILE_CODE, "##close-rectangle-assets", "assets", 0);
}

void spe::UIAssetFolder::renderFolderHierarchyRecursiv(const char* path, const char* name, bool openNextTreeNode)
{
    struct dirent* entry;
    DIR* dir = opendir(path);
    if (dir == NULL) {
        return;
    }
    this->m_interacted = false;

    if (!spe::FileDialog::checkIfADirHasSubItems(path, false))
    {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 3.5f );
        if (spe::Style::DisplaySymbolInMenuItemWithText(ICON_FA_FOLDER, name))
        {
            this->setCurrentPath(path, name);
        }
        closedir(dir);

        return;
    }
    const std::string newname = std::string("##") + name;

    bool entered = false;

    if (spe::Style::DisplaySymbolInTreeNode(ICON_FA_FOLDER, newname.c_str(), openNextTreeNode))
    {
        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 33, ImGui::GetCursorPosY() - 23));
        if (ImGui::MenuItem(name))
        {
            this->setCurrentPath(path, name);
        }
        entered = true;

        while ((entry = readdir(dir)) != NULL)
        {
            bool folder = true;
            const char* str = entry->d_name;

            std::string std_name(str);

            //Checks if the string has only chars like ../../ ..
            if (!spe::Utility::IsStringValid(std_name))
            {
                continue;
            }

            //We need to know if we got a folder or not for the recursion
            folder = spe::Utility::IsFolder(std_name);

            //Recursivly calling
            std::string newPath = std::string(path) + "\\" + std_name;

            if (folder)
            {
                const char* ch = newPath.c_str();
                this->renderFolderHierarchyRecursiv(ch, str, false);
            }
        }
        ImGui::TreePop();
    }
    if (!entered)
    {
        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 52, ImGui::GetCursorPosY() - 23));
        if (ImGui::MenuItem(name))
        {
            this->setCurrentPath(path, name);
        }
    }


    closedir(dir);
}

void spe::UIAssetFolder::renderFilesWithChildWindow(const std::string& name, const std::string& new_path, const std::string& entryPath, bool isFolder, uint32_t textureId, uint8_t columCnt)
{
    bool resetHoveredItem = false;
    float rounding = 10.0f;
    ImVec2 child_size = ImVec2(120, 220);

    // The multiplyer for the color (hover effect)
    int adder = 1;
    if (this->m_to_hover_item_name == name)
    {
        if (!this->m_tools.isPopUpOpen())
        {
            this->m_to_hover_item_name = "";
        }
        this->m_is_item_hovered = false;
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
            this->setCurrentPath(new_path, entryPath);
        }
    }

    // Setting current item which is hovered
    if (this->m_to_hover_item_name == "")
    {
        this->m_is_item_hovered = ImGui::IsItemHovered();
        if (this->m_is_item_hovered)
            m_to_hover_item_name = name;
    }
    ImGui::PopStyleColor();

    // Make the file "selectable"
    if (!isFolder)
        this->setDragAndDrop(new_path, entryPath);

    // Adding - for to long names
    std::string withoutExt = spe::Utility::RemoveExtension(entryPath);
    float textSizeX = ImGui::CalcTextSize(withoutExt.c_str()).x;
    std::string newTextWraped = "";
    if (textSizeX > 80)
    {
        // To big text size
        for (size_t i = 0; i < withoutExt.size() / 2; i++)
        {
            newTextWraped += withoutExt[i];
        }
        newTextWraped += " -\n";
        for (size_t i = withoutExt.size() / 2; i < withoutExt.size(); i++)
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
    ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 1.0f), spe::Utility::GetFileExtension(entryPath).c_str());
    ImGui::EndChild();
    ImGui::PopStyleColor();

    // Setting spacing for rows
    if (columCnt >= MAX_COLUMNS)
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + PADDING_BETWEEN_ROWS);
    }
    this->m_is_item_hovered = false;

    if (resetHoveredItem)
    {
        this->m_to_hover_item_name = "";
    }

    ImGui::NextColumn();
}

void spe::UIAssetFolder::getAllFilesInDir(const char* path, const char* name)
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
        if (!spe::Utility::IsStringValid(std_name))
        {
            continue;
        }

        const std::string icon = spe::Utility::GetFileExtension(std_name);
        const std::string newPath = std::string(path) + "\\" + std_name;
        const std::string name = "##" + std::string(str);
        const uint32_t id = this->m_data.getId(icon);
        const bool isFolder = (icon == "folder");

        if (this->m_file_filter.PassFilter(name.c_str()))
        {
            this->renderFilesWithChildWindow(name, newPath, std_name, isFolder, id, cnt);
        }
    }

    closedir(dir);
}

void spe::UIAssetFolder::goBackToBeforeFolder()
{
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 300, ImGui::GetCursorPosY() + 10));
    const std::vector<std::string> pathParts = spe::Utility::Split(this->m_current_path, '\\');
    std::vector<std::string> validParts;
    for (int i = 0; i < pathParts.size(); i++)
    {
        if (spe::Utility::IsStringValid(pathParts[i]) && pathParts[i] != spe::EngineData::s_NameOfUser)
        {
            validParts.push_back(pathParts[i]);
        }
    }


    for (int i = 0; i < validParts.size(); i++)
    {
        bool popStyle = false;
        // Set current path to the folder clicked
        if (this->m_current_name == validParts[i])
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 1));
            popStyle = true;
        }
        if (ImGui::Button(validParts[i].c_str()) && this->m_current_name != validParts[i])
        {
            this->m_current_name = validParts[i];
            this->m_current_path = "";
            for (int j = 0; j <= i; j++)
            {
                 this->m_current_path += (j - 1 == j) 
                     ? validParts[j]
                     : validParts[j] + "\\";
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

void spe::UIAssetFolder::setDragAndDrop(std::string path, std::string name)
{
    //Check if we hover over the menu item used later on for drag and drop
    if (this->m_ptr_GUIRepo->child_to_parent != nullptr)
    {
        return;
    }
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0) && !this->m_interacted && this->m_ptr_GUIRepo->DragAndDropPath == " ")
    {
        this->m_dragging_item = true;
        this->m_ptr_GUIRepo->DragAndDropPath = path;
        this->m_ptr_GUIRepo->DragAndDropName = name;
    }
    if (ImGui::IsMouseReleased(0))
    {
        this->m_interacted = true;
        this->m_dragging_item = false;
        this->m_hovered_over_item = false;
        this->m_ptr_GUIRepo->DragAndDropPath = " ";
        this->m_ptr_GUIRepo->DragAndDropName = " ";
    }
}

void spe::UIAssetFolder::beginColumns()
{
    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = (int)(panelWidth / PADDING_BETWEEN_COLUMS);

    if (columnCount < 1)
    {
        columnCount = 1;

    }
    ImGui::Columns(columnCount, 0, false);
}