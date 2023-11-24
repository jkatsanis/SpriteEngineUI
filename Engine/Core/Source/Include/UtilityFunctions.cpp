#include "UtilityFunctions.h"

bool spe::Utility::Contains(const std::string& str, const std::vector<std::string>& arr)
{
    for (size_t i = 0; i < arr.size(); i++)
    {
        if (str == arr[i])
        {
            return true;
        }
    }
    return false;
}

void spe::Utility::SetCurrentDir(const std::string& path)
{
    if (!SetCurrentDirectoryA(path.c_str()))
    {
        throw std::runtime_error("Couldn't set directory");
    }
}

std::string spe::Utility::getUserProjectPathSeperatetFromEnginePath(const std::string& path)
{
    std::string userProject = spe::EngineData::s_NameOfUser;
    int idx = (int)path.find(userProject);
    std::string finalStr = "";
    for (int i = idx + (int)userProject.size() + 1; i < path.size(); i++)
    {
        finalStr += path[i];
    }
    return finalStr;
}

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

std::string spe::Utility::GetFileExtension(const std::string& file)
{
    for (int i = 0; i < file.size(); i++)
    {
        if (file[i] == '.')
        {
            bool isValid = true;
            std::string extension = "";
            for (int j = i + 1; j < file.size(); j++)
            {
                extension += file[j];
                if (file[j] == '.')
                {
                    isValid = false;
                    break;
                }
            }
            if (isValid)
            {
                return extension;
            }
        }
    }
    return "folder";
}

std::string spe::Utility::GetDefaultDir()
{
    spe::Log::LogString("Calling GetDefaultDir()..");
    char NPath[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, NPath);
    std::string path(NPath);
    std::vector<std::string> parts = spe::Utility::Split(path, '\\');

    std::string projectString = "";

    for (size_t i = 0; i < parts.size() - 2; i++)
    {
        projectString += (i == parts.size() - 3) ? parts[i] : parts[i] + "\\";
    }
    return projectString;
}

std::string spe::Utility::GetCurrentDir()
{
    char NPath[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, NPath);
    std::string path(NPath);
    return path;
}

std::string spe::Utility::CopyDir(const std::string& inputDir, const std::string& outputdir, const std::string& name)
{
    std::string mkdir = "mkdir \"" + outputdir + std::string(name.c_str()) + "\"";

    system(mkdir.c_str());

    std::string copy = "xcopy \"" + inputDir + "\" \"" + outputdir + "\\" + std::string(name.c_str()) + "\" /E";

    system(copy.c_str());

    return outputdir + std::string(name.c_str());
}