#include "UIAssetFolder.h"

spe::UIAssetFolder::UIAssetFolder()
{
    this->m_Tools = spe::UIAssetTools(&this->m_CurrentPath, &this->m_HoverItemName);
    this->m_IsItemHovered = false;
    this->m_CurrentPath = USER_FOLDER_NAME;
    this->m_CurrentName = USER_FOLDER_NAME;
    this->m_IsItemHovered = false;
    this->m_Interacted = false;
    this->m_DraggingItem = false;
    this->m_HoveredOverItem = false;
    this->m_FileContentPadding = 15;

    this->m_Size = ASSET_FOLDER_DEFAULT_WINDOW_SIZE;
    this->m_HoverItemName = "";
    this->m_IsAssetFolderOpen = true;
}

void spe::UIAssetFolder::Init()
{
    this->m_ptr_GUIRepo->AssetFolderData.ptr_Size = &this->m_Size;

    DIR* d = opendir("Assets");
    this->m_AlreadyOpenedPaths["Assets"] = d;
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
            this->AddPrefab();
        }
        else
        {
            this->m_HoverItemName = "";
        }

        this->m_Tools.Update(this->Hovered);

        this->ResizeWindow();

        this->RenderFolderHierarchy();
        this->RenderCloseRectangle();
        this->RenderContentBrowser();

        if (!this->Hovered)
        {
            this->Hovered = spe::UIUtility::IsHovered(ImVec2(0, 1080 - this->m_Size.y), this->m_Size);
        }
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
 
void spe::UIAssetFolder::RenderContentBrowser()
{
    ImGui::SetCursorPos(ImVec2(UIASSET_FOLDER_WIDTH + FOLDER_HIERARCHY_PADDING * 1.5f, FOLDER_HIERARCHY_PADDING));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(FILE_DISPLAYER_COLOR / 255.0f, FILE_DISPLAYER_COLOR / 255.0f, FILE_DISPLAYER_COLOR / 255.0f, 255.0f));
    ImGui::BeginChild("##file-displayer-container", ImVec2(this->m_Size.x - (FOLDER_HIERARCHY_PADDING * 1.8f + UIASSET_FOLDER_WIDTH), this->m_Size.y), false, ImGuiWindowFlags_NoScrollbar);

    this->GoBackToBeforeFolder();
    ImGui::SetCursorPos(ImVec2(5, this->m_FileContentPadding));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.0f); // Set rounding to 5 pixels
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 3.0f)); // Add some padding for visual clarity
    this->m_FileFilter.Draw("Search", 200);
    ImGui::Dummy(ImVec2(10, 10));
    ImGui::PopStyleVar(2);

    this->BeginColumns();
    this->GetAllFilesInDir(this->m_CurrentPath.c_str());
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::SetWindowPos(ImVec2(0, 1080 - this->m_Size.y));
    ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
    ImGui::SetWindowSize(this->m_Size);

}

void spe::UIAssetFolder::SetCurrentPath(const std::string& path, const std::string& name)
{
    this->m_CurrentName = name;
    this->m_CurrentPath = path;
}

void spe::UIAssetFolder::ResizeWindow()
{
    bool pop_style = false;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 200);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
    if (this->m_ResizeWindow.ClickedOnResizeButton)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 1));
        pop_style = true;
    }
    spe::Style::DisplaySmybolAsButton(ICON_FA_ARROW_UP);
    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
    {
        this->m_ResizeWindow.AdditionalAdd = 1080 - spe::UIUtility::GUICursor.Position.Y - this->m_Size.y;
        this->m_ResizeWindow.ClickedOnResizeButton = true;
    }
    if (this->m_ResizeWindow.ClickedOnResizeButton && ImGui::IsMouseDown(0))
    {
        const float new_size = 1080 - spe::UIUtility::GUICursor.Position.Y - this->m_ResizeWindow.AdditionalAdd;
        if (new_size > 150
            && new_size < 1031)
        {
            this->m_Size.y = new_size;
        }
    }
    else
    {
        this->m_ResizeWindow.ClickedOnResizeButton = false;
    }
    if (pop_style)
    {
        ImGui::PopStyleColor(2);
    }

}

void spe::UIAssetFolder::AddPrefab()
{
    if (this->m_ptr_Repo == nullptr)
    {
        return;
    }
    if (this->m_ptr_GUIRepo->ChildToParent != nullptr && ImGui::IsMouseReleased(0) && this->Hovered)
    {
        const std::string pathToFile = this->m_CurrentPath + "\\" + this->m_ptr_GUIRepo->ChildToParent->Name + EXTENSION_PREFAB_FILE;
        
        this->m_ptr_GUIRepo->ChildToParent->Prefab.UpdateProps(pathToFile, pathToFile, this->m_ptr_GUIRepo->ChildToParent->Name + EXTENSION_PREFAB_FILE);

        spe::Savesystem::CreateOrUpdatePrefabFile(this->m_ptr_GUIRepo->ChildToParent, pathToFile, pathToFile);
    }
}

void spe::UIAssetFolder::RenderFolderHierarchy()
{
    ImGui::SetCursorPos(ImVec2(FOLDER_HIERARCHY_PADDING, FOLDER_HIERARCHY_PADDING));
    ImGui::BeginChild("##folder-hierarchy", ImVec2(UIASSET_FOLDER_WIDTH, this->m_Size.y), false);
    this->RenderFolderHierarchyRecursiv("assets", "assets", this->m_IsAssetFolderOpen);
    this->m_IsAssetFolderOpen = false;
    ImGui::EndChild();
}

void spe::UIAssetFolder::RenderCloseRectangle()
{
    this->m_ptr_GUIRepo->AssetFolderData.IsOpen = spe::UIUtility::RenderCloseRectangle(
        FOLDER_HIERARCHY_PADDING, ICON_FA_FILE_CODE, "##close-rectangle-assets", "assets", 0);
}

void spe::UIAssetFolder::RenderFolderHierarchyRecursiv(const char* path, const char* name, bool openNextTreeNode)
{
    struct dirent* entry;
    DIR* dir = opendir(path);
    if (dir == NULL) {
        return;
    }
    this->m_Interacted = false;

    if (!spe::FileDialog::CheckIfADirHasSubItems(path, false))
    {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 3.5f );
        if (spe::Style::DisplaySymbolInMenuItemWithText(ICON_FA_FOLDER, name, 30))
        {
            this->SetCurrentPath(path, name);
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
            this->SetCurrentPath(path, name);
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
                this->RenderFolderHierarchyRecursiv(ch, str, false);
            }
        }
        ImGui::TreePop();
    }
    if (!entered)
    {
        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 52, ImGui::GetCursorPosY() - 23));
        if (ImGui::MenuItem(name))
        {
            this->SetCurrentPath(path, name);
        }
    }


    closedir(dir);
}

void spe::UIAssetFolder::RenderFilesWithChildWindow(const std::string& name, const std::string& new_path, const std::string& entryPath, bool isFolder, uint32_t textureId, uint8_t columCnt)
{
    bool resetHoveredItem = false;
    float rounding = 10.0f;
    ImVec2 child_size = ImVec2(120, 220);

    // The multiplyer for the color (hover effect)
    int adder = 1;
    if (this->m_HoverItemName == name)
    {
        if (!this->m_Tools.IsPopUpOpen())
        {
            this->m_HoverItemName = "";
        }
        this->m_IsItemHovered = false;
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
            this->SetCurrentPath(new_path, entryPath);
        }
    }

    // Setting current item which is hovered
    if (this->m_HoverItemName == "")
    {
        this->m_IsItemHovered = ImGui::IsItemHovered();
        if (this->m_IsItemHovered)
            m_HoverItemName = name;
    }
    ImGui::PopStyleColor();

    // Make the file "selectable"
    if (!isFolder)
        this->SetDragAndDrop(new_path, entryPath);

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
    this->m_IsItemHovered = false;

    if (resetHoveredItem)
    {
        this->m_HoverItemName = "";
    }

    ImGui::NextColumn();
}

void spe::UIAssetFolder::GetAllFilesInDir(const char* path)
{
    uint8_t cnt = 0;

    DIR* dir = opendir(path);

    struct dirent* entry;
    if (dir == NULL) {
        return;
    }
    this->m_Interacted = false;
    while ((entry = readdir(dir)) != NULL)
    {
        cnt++;
        const char* str = entry->d_name;
        const std::string std_name(str);
        const ImVec2 textSize = ImGui::CalcTextSize(str);

        // Checks if the string has only chars like ../../ ..
        if (!spe::Utility::IsStringValid(std_name))
        {
            continue;
        }

        const std::string icon = spe::Utility::GetFileExtension(std_name);
        const std::string newPath = std::string(path) + "\\" + std_name;
        const std::string name = "##" + std::string(str);
        const uint32_t id = this->m_IconData.GetId(icon);
        const bool isFolder = (icon == "folder");

        if (this->m_FileFilter.PassFilter(name.c_str()))
        {
            this->RenderFilesWithChildWindow(name, newPath, std_name, isFolder, id, cnt);
        }
    }

    closedir(dir);

}

void spe::UIAssetFolder::GoBackToBeforeFolder()
{
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 300, ImGui::GetCursorPosY() + 10));
    const std::vector<std::string> pathParts = spe::Utility::Split(this->m_CurrentPath, '\\');
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
        if (this->m_CurrentName == validParts[i])
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 1));
            popStyle = true;
        }
        if (ImGui::Button(validParts[i].c_str()) && this->m_CurrentName != validParts[i])
        {
            this->m_CurrentName = validParts[i];
            this->m_CurrentPath = "";
            for (int j = 0; j <= i; j++)
            {
                 this->m_CurrentPath += (j - 1 == j) 
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

void spe::UIAssetFolder::SetDragAndDrop(std::string path, std::string name)
{
    //Check if we hover over the menu item used later on for drag and drop
    if (this->m_ptr_GUIRepo->ChildToParent != nullptr)
    {
        return;
    }
    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0) && !this->m_Interacted && this->m_ptr_GUIRepo->DragAndDropPath == " ")
    {
        this->m_DraggingItem = true;
        this->m_ptr_GUIRepo->DragAndDropPath = path;
        this->m_ptr_GUIRepo->DragAndDropName = name;
    }
    if (ImGui::IsMouseReleased(0))
    {
        this->m_Interacted = true;
        this->m_DraggingItem = false;
        this->m_HoveredOverItem = false;
        this->m_ptr_GUIRepo->DragAndDropPath = " ";
        this->m_ptr_GUIRepo->DragAndDropName = " ";
    }
}

void spe::UIAssetFolder::BeginColumns()
{
    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = (int)(panelWidth / PADDING_BETWEEN_COLUMS);

    if (columnCount < 1)
    {
        columnCount = 1;

    }
    ImGui::Columns(columnCount, 0, false);
}