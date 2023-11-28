#include "UtilityFunctions.h"

std::string spe::Utility::BoolToStr(bool b)
{
    return b ? "True" : "False";
}

void spe::Utility::CreateFileWithContent(const std::string& content, const std::string& pathAndName)
{
    std::ofstream file;

    file.open(pathAndName, std::ios::out | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("Couldn't open file to write to");
    }

    file << content;

    file.close();
}

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

std::string spe::Utility::VectorToString(const std::vector<std::string>& vec, char del, const std::string& ext)
{
    std::string rs = "";

    for (size_t i = 0; i < vec.size(); i++)
    {
        rs += vec[i];
        if (i + 1 < vec.size())
        {
            rs += del;
        }
    }

    return rs + ext;
}

std::string spe::Utility::RenamePartOnPath(const std::string& path, const std::string& new_name, char del, const std::string& ext, uint32_t pos)
{
    std::vector<std::string> parts = spe::Utility::Split(path, del);
    parts[parts.size() - pos] = new_name;
    return VectorToString(parts, del, ext);
}

void spe::Utility::SetCurrentDir(const std::string& path)
{
    spe::Log::LogString("Setting dir..");
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

std::string spe::Utility::GetNamePathSplit(std::string path)
{
    std::vector<std::string> pick;

    std::vector<std::string> splittetSring = spe::Utility::Split(path, '\\');

    return splittetSring[splittetSring.size() - 1];
}

void spe::Utility::Delete(const std::string& path)
{
    if (!std::filesystem::exists(path))
    {
        throw std::runtime_error("There is no file!");
        return;
    }
    std::filesystem::remove_all(path);
}

std::string spe::Utility::GetDefaultDir(uint32_t depth)
{
    spe::Log::LogString("Calling GetDefaultDir()..");
    char NPath[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, NPath);
    std::string path(NPath);
    std::vector<std::string> parts = spe::Utility::Split(path, '\\');

    std::string projectString = "";

    for (size_t i = 0; i < parts.size() - depth; i++)
    {
        projectString += (i == parts.size() - depth - 1) ? parts[i] : parts[i] + "\\";
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

bool spe::Utility::IsStringValid(const std::string& path)
{
    int cnt = 0;
    for (const char c : path)
    {
        if (c == '.' || c == '/' || c == '$')
        {
            cnt++;
        }
    }
    return !(cnt == path.size());
}

bool spe::Utility::IsFolder(const std::string& path)
{
    for (const char c : path)
    {
        if (c == '.')
        {
            return false;
        }
    }
    return true;
}

std::string spe::Utility::RemoveExtension(const std::string& file)
{
    std::string newFileName = "";
    for (int i = 0; i < file.size(); i++)
    {
        if (file[i] == '.')
        {
            break;
        }
        newFileName.push_back(file[i]);
    }
    return newFileName;
}

std::string spe::Utility::CopyDir(const std::string& inputDir, const std::string& outputdir, const std::string& name)
{
    std::string mkdir = "mkdir \"" + outputdir + std::string(name.c_str()) + "\"";

    system(mkdir.c_str());

    std::string copy = "xcopy \"" + inputDir + "\" \"" + outputdir + "\\" + std::string(name.c_str()) + "\" /E /I";

    system(copy.c_str());

    return outputdir + std::string(name.c_str());
}