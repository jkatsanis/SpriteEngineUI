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
}

//Public functions

void s2d::UIAssetFolder::createAssetLinkerWindow()
{
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.1f);
    if (ImGui::Begin("Assets", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        this->render();
        this->addPrefab();
        this->m_tools.update(isHovered);
        ImGui::End();
    }
    ImGui::PopStyleVar();

    if (s2d::UIAssetFolder::dragAndDropPath != " ")
    {
        const ImVec2 cursor = ImGui::GetCursorPos();
        const ImVec2 pos = ImVec2(float(sf::Mouse::getPosition().x - 100), float(sf::Mouse::getPosition().y + 10));

        if (ImGui::Begin("##Drag", NULL, ImGuiWindowFlags_NoTitleBar))
        {
            ImGui::SetWindowPos(pos);
            ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);

            ImGui::Text(s2d::UIAssetFolder::dragAndDropName.c_str());
            ImGui::End();
        }
    }
}

//private functions

void s2d::UIAssetFolder::render()
{
    this->goBackToBeforeFolder();
    this->beginColumns();
    this->getAllFilesInDir(this->currentPath.c_str(), this->currentName.c_str());

    ImGui::SetWindowPos(ImVec2(250, 730));
    ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);
    ImGui::SetWindowSize(ImVec2(1280, 350));

    this->isHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenBlockedByPopup);
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

    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(0) && !this->m_interacted && dragAndDropPath == " ")
    {
        this->m_draggingItem = true;
        s2d::UIAssetFolder::dragAndDropPath = path;
        s2d::UIAssetFolder::dragAndDropName = name;
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
std::string s2d::UIAssetFolder::dragAndDropName = " ";
