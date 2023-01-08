#include "fileDialogs.h"


s2d::FileDialog::FileDialog()
{
    this->pathClicked = "";
}

void s2d::FileDialog::openFile(const char* dir_path, const std::string& icon)
{
    // Open the directory
    DIR* dir = opendir(dir_path);
    if (dir == NULL)
        return;

    // Read each entry in the directory
    dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0
            || std::string(entry->d_name).find('$') != std::string::npos)
            
            continue;

        // Compute the full path of the entry
        char path[1024];
        snprintf(path, 1024, "%s/%s", dir_path, entry->d_name);

        // If the entry is a directory, recursively traverse it
        if (entry->d_type == DT_DIR)
        {
            // Add a tree node for the directory
            ImVec2 old = ImGui::GetCursorPos();
            ImVec2 cursorPos = ImVec2(740, ImGui::GetCursorPosY() - 8 );

            // ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 200);
            s2d::FontManager::displaySmybolAsButton(icon.c_str(), cursorPos);

            ImGui::SetCursorPos(old);
            if (!checkIfADirHasSubDirs(path))
            {
                std::string name = "       " + std::string(entry->d_name);
                ImGui::MenuItem(name.c_str());
            }
            else if (ImGui::TreeNode(entry->d_name))
            {
                openFile(path, icon);
                ImGui::TreePop();
            }
    
        }
    }

    // Close the directory
    closedir(dir);

}

// private methods

bool s2d::FileDialog::checkIfADirHasSubDirs(const std::string& dirPath)
{
    bool value = false;
    DIR* dir = opendir(dirPath.c_str());
    if (dir == NULL)
        return false;

    // Read each entry in the directory
    dirent* entry;
    int cnt = 0;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) 
            {
                continue;
            }
            value = true;
        }
    }

    closedir(dir);

    return value;
}

// static methods

std::string s2d::FileDialog::getEmptyStringBetween(const std::string& content, const std::string& name, float padding)
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
