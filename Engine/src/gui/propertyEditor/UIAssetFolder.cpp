#include "UIAssetFolder.h"

s2d::UIAssetFolder::UIAssetFolder()
{
    // Load icons
    texture.loadFromFile("EngineAssets\\Icons\\assetFolder.png");
    textureId = (ImTextureID)texture.generateMipmap();

    this->currentPath = "..\\Assets\\assets";
    this->currentName = "Assets";
    this->m_iconSize = 50;
    this->m_padding = 80;

    this->isHovered = false;
    this->isHovered = false;
    this->m_interacted = false;
    this->m_draggingItem = false;
    this->m_hoveredOverItem = false;
}

s2d::UIAssetFolder::~UIAssetFolder()
{ 
    for (s2d::FileData* data : this->m_filesInAssets)
    {
        delete data;
    }
}

//Public functions

void s2d::UIAssetFolder::createAssetLinkerWindow()
{
    /* if (!s2d::UIInfo::srenderAssetFolder)
    {
        return;
    }*/
    //Pushing transperany
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
    this->getAllFilesInDir(this->currentPath.c_str(), this->currentName.c_str(), m_filesInAssets);
    
    ImGui::SetWindowPos(ImVec2(250, 820));
    ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);
    ImGui::SetWindowSize(ImVec2(1280, 260));

    this->isHovered = ImGui::IsWindowHovered();
}


void s2d::UIAssetFolder::getAllFilesInDir(const char* path, const char* name, std::vector<s2d::FileData*>& vec)
{   
    struct dirent* entry;
    DIR* dir = opendir(path);
    if (dir == NULL) {
        return;
    }
    m_interacted = false;
 
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
            std::string name = "##" + std::string(str);
            if (ImGui::ImageButton(name.c_str(), textureId, ImVec2(this->m_iconSize, this->m_iconSize)))
            {
                this->currentPath = newPath;
                this->currentName = str;
            }
            ImGui::TextWrapped(str);
            const char* ch = newPath.c_str();
        }

        std::string f_path(path);
        f_path = f_path.erase(0, 50);
        int splitSize = std::getStringSplittedSize(f_path, "\\");
        if (!folder)
        {
            ImGui::MenuItem(std_name.c_str());

            this->setDragAndDrop(newPath);
        }
        vec.push_back(new s2d::FileData(folder, splitSize, std_name, f_path));

        //next column to have it inline
        ImGui::NextColumn();
    }

    closedir(dir);
}

void s2d::UIAssetFolder::goBackToBeforeFolder()
{
    std::string* props = std::splitString(this->currentPath, "\\");

    for (int i = 0; i < props->length(); i++)
    {
        std::cout << props[i].c_str() << std::endl;
        ImGui::Button(props[i].c_str());
        ImGui::SameLine();
        if(i != props->length() - 1)
            ImGui::Text("->");
        ImGui::SameLine();
    }


    delete[] props;
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
