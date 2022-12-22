#include "UIAssetFolder.h"

s2d::UIAssetFolder::UIAssetFolder()
{
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
    getAllFilesInDir("..\\Assets\\assets", "Assets", m_filesInAssets);

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
    if (ImGui::TreeNode(name))
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
                getAllFilesInDir(ch, str, vec);
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
        }
        ImGui::TreePop();
    }
    closedir(dir);
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

std::string s2d::UIAssetFolder::dragAndDropPath = " ";
