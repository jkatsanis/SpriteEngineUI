#include "UtilityFunctions.h"

std::vector<std::string> spe::Utility::Split(const std::string& s,char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

void spe::Utility::WriteFile(const std::string& content, const std::string& path)
{
    std::ofstream file;

    file.open(path, std::ios::out | std::ios::binary);

    file << content;

    file.close();
    
}

std::string spe::Utility::GetDefaultPath()
{
    char NPath[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, NPath);
    std::string path(NPath);
    std::vector<std::string> parts = spe::Utility::Split(path, '\\');

    std::string projectString = "";

    for (size_t i = 0; i < parts.size() - 2; i++)
    {
        projectString += parts[i] + "\\";
    }
    return projectString;
}
