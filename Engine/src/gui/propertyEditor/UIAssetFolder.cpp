#include "UIAssetFolder.h"

s2d::UIAssetFolder::UIAssetFolder()
{
    this->currentPath = "..\\Assets\\assets";
    this->currentName = "Assets";
    this->m_iconSize = 75;
    this->m_padding = 130;

    this->isHovered = false;
    this->isHovered = false;
    this->m_interacted = false;
    this->m_draggingItem = false;
    this->m_hoveredOverItem = false;
}

//Public functions

void s2d::UIAssetFolder::createAssetLinkerWindow()
{
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.1f);
    ImGui::Begin("Assets", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
    this->render();
    ImGui::End() ;

    ImGui::PopStyleVar();

}

//private functions

void s2d::UIAssetFolder::render()
{
    this->goBackToBeforeFolder();
    ImGui::Dummy(ImVec2(0, 25));
    this->beginColumns();
    this->getAllFilesInDir(this->currentPath.c_str(), this->currentName.c_str());

    ImGui::SetWindowPos(ImVec2(250, 820));
    ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);
    ImGui::SetWindowSize(ImVec2(1280, 260));

    this->isHovered = ImGui::IsWindowHovered();
}


void s2d::UIAssetFolder::getAllFilesInDir(const char* path, const char* name)
{   
    struct dirent* entry;
    DIR* dir = opendir(path);
    if (dir == NULL) {
        return;
    }
    m_interacted = false;
 
    while ((entry = readdir(dir)) != NULL)
    {
        const char* str = entry->d_name;
        std::string std_name(str);

        //Checks if the string has only chars like ../../ ..
        if (!std::isStringValid(std_name))
        {
            continue;
        }

        //We need to know if we got a folder or not for the recursion
        std::string icon = std::getFileExtension(std_name);
        std::string newPath = std::string(path) + "\\" + std_name;

        //The name of the ImageButton
        std::string name = "##" + std::string(str);

        // Init icon texture
        ImTextureID id = this->data.getId(icon);
        bool isFolder = (icon == "folder");

        if (ImGui::ImageButton(name.c_str(), id, ImVec2(this->m_iconSize, this->m_iconSize)))
        {
            //Display the item if its a folder
            if (isFolder)
            {
                this->currentPath = newPath;
                this->currentName = str;
            }        
        }
        this->setDragAndDrop(newPath);

        ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);
        ImGui::TextWrapped(str);
        ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);

        //next column to have it inline
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
        if (props[i] == "#")
        {
            break;
        }
        if (props[i] == "..")
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
        if(i != props.size() - 1)
            ImGui::Text("->");
        ImGui::SameLine();
    }
}

void s2d::UIAssetFolder::setDragAndDrop(std::string path)
{
    //Check if we hover over the menu item used later on for drag and drop

    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0) && !this->m_interacted && dragAndDropPath == " ")
    {
        this->m_draggingItem = true;
        s2d::UIAssetFolder::dragAndDropPath = path;
    }
    if (ImGui::IsMouseReleased(0))
    {
        this->m_interacted = true;
        this->m_draggingItem = false;
        this->m_hoveredOverItem = false;
        s2d::UIAssetFolder::dragAndDropPath = " ";
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


std::string s2d::UIAssetFolder::dragAndDropPath = " ";
